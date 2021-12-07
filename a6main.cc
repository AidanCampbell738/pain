#include <iostream>
#include "config.h"
#include "MPRNG.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "watcard.h"
#include "watcardoffice.h"
#include "groupoff.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include "bottlingplant.h"
#include "student.h"

using namespace std;

MPRNG mprng(getpid());//global RNG variable

int main(int argc, char* argv[]) {
    const char* inputFile = "soda.config";
    int processors = 1;
    ConfigParms cparms;

    try {//process command line args
        switch(argc) {
            case 4: if(strcmp(argv[3], "d") != 0) {//processors
                if(stoi(argv[3]) <= 0) throw 1;//check that value is legal
                processors = stoi(argv[3]);
            }
            case 3: if(strcmp(argv[2], "d") != 0) {//seed
                if(stoi(argv[2]) <= 0) throw 1;//check that value is legal
                mprng.set_seed(stoi(argv[2]));
            } 
            case 2: if(strcmp(argv[1], "d") != 0) {//filename
                inputFile = argv[1];
            } 
            case 1: break;//avoid error in default
            default: throw 1;//too many arguments
        }
    } catch(...) {//error detected -> output message and terminate program
        cerr << "Error processing command line args!" << endl; 
        exit(EXIT_FAILURE);
    }

    //process config file using given config.h
    processConfigFile(inputFile, cparms);

    uProcessor p[processors - 1]; // number of kernel threads
    if(processors == 1) uThisProcessor().setPreemption(0); // turn off time-slicing for reproducibility

    //Create printer
    Printer printer(cparms.numStudents, cparms.numVendingMachines, cparms.numCouriers);

    //Create bank
    Bank bank(cparms.numStudents);

    //Create parent
    Parent parent(printer, bank, cparms.numStudents, cparms.parentalDelay);

    //Create WATCard office (which creates couriers)
    WATCardOffice cardOffice(printer, bank, cparms.numCouriers);

    //Create groupoff
    Groupoff groupoff(printer, cparms.numStudents, cparms.sodaCost, cparms.groupoffDelay);

    //Create name server
    NameServer nameServer(printer, cparms.numVendingMachines, cparms.numStudents);

    //Create vending machines
    VendingMachine* vms[cparms.numVendingMachines];
    for(unsigned int i = 0; i < cparms.numVendingMachines; i++) {
        vms[i] = new VendingMachine(printer, nameServer, i, cparms.sodaCost);
    }

    //Create bottling plant (which creates truck)
    BottlingPlant* bp = new BottlingPlant(printer, nameServer, cparms.numVendingMachines, cparms.maxShippedPerFlavour, cparms.maxStockPerFlavour, cparms.timeBetweenShipments);

    //Create students
    Student* students[cparms.numStudents];
    for(unsigned int i = 0; i < cparms.numStudents; i++) {
        students[i] = new Student(printer, nameServer, cardOffice, groupoff, i, cparms.maxPurchases);
    }

    //Wait for students to finish
    for(unsigned int i = 0; i < cparms.numStudents; i++) {
        delete students[i];
    }

    //delete bottling plant
    delete bp;

    //delete vending machines
    for(unsigned int i = 0; i < cparms.numStudents; i++) {
        delete vms[i];
    }
}