#include "config.h"
#include "MPRNG.h"
#include <iostream>

using namespace std;

MPRNG mprng(getpid());//global RNG variable

int main(int argc, char* argv[]) {
    char* inputFile = "soda.config";
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

    //Create bank

    //Create parent

    //Create WATCard office (which creates couriers)

    //Create groupoff

    //Create name server

    //Create vending machines

    //Create bottling plant (which creates truck)

    //Create students
}