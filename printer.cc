#include <iostream>
#include "printer.h"

using namespace std;

Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers) : 
    numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers) {
    numCols = 6 + numStudents + numVendingMachines + numCouriers;
    buffer = new Data[numCols];//create buffer array

    //Print all titles
    cout << "Parent\tGropoff\tWATOff\tNames\tTruck\tPlant";
    for(unsigned int i = 0; i < numStudents; i++) {//students
        cout << "\tStud" << i;
    }
    for(unsigned int i = 0; i < numVendingMachines; i++) {//vending machines
        cout << "\tMach" << i;
    }
    for(unsigned int i = 0; i < numCouriers; i++) {//courriers
        cout << "\tCour" << i;
    }
    cout << endl;

    //Print *******'s
    for(unsigned int i = 0; i < numCols; i++) {
        if(i > 0) cout << "\t";
        cout << "*******";
    }
    cout << endl;
}

//Non-trivial destructor
//delete the buffer, print last values, and print final message
Printer::~Printer() {
    for(unsigned int i = 0; i < numCols; i++) {
        if(buffer[i].assigned) {//print any messages that haven't been printed yet
            flush();
            break;
        }
    }
    delete[] buffer;//clean up
    cout << "*****************" << endl;//final message
}

//This method will print a line of text
//this method is called after print attempts to write to a buffer index
//whose "assigned" boolean is already true
void Printer::flush() {
    int blankSpaces = -1;//used to configure whitespace (blankSpace represents the number of blank columns in the line)
    for(unsigned int i = 0; i < numCols; i++) {
        if(buffer[i].assigned) {//only print stuff for voters with data to print
            for(int j = 0; j < blankSpaces + 1 && i > 0; j++) {
                cout << "\t";//print tab characters
            }
            blankSpaces = 0;

            //print state data with no datavalues
            //a "datavalue" referes to the integer printed after the state letter
            //ex: Ds,g -> s and g are datavalues
            switch(buffer[i].state) {
                //break if kind's version of state message has different # of datavalues
                //for example: break for Student's "S" message since it has two values (f and b) instead of 0
                case 'S': if(buffer[i].kind == Kind::Student || buffer[i].kind == Kind::Vending) break;
                case 'F':
                case 'W':
                case 'X':
                case 'P': if(buffer[i].kind == Kind::Truck) break;
                case 'L': if(buffer[i].kind == Kind::Courier) break;
                case 'R': if(buffer[i].kind == Kind::NameServer) break;
                case 'A': if(buffer[i].kind == Kind::Student) break;
                case 'r': cout << buffer[i].state;
            }

            //print state data with one datavalue
            switch(buffer[i].state) {
                case 'D': if(buffer[i].kind == Kind::Parent || buffer[i].kind == Kind::Truck) break;
                case 'R': if(buffer[i].kind == Kind::Vending) break;
                case 'P': if(buffer[i].kind == Kind::BottlingPlant) break;
                case 'G': if(buffer[i].kind == Kind::Student) break;
                case 'L': if(buffer[i].kind == Kind::Student) break;
                case 'S': if(buffer[i].kind != Kind::Vending) break;
                case 'V': cout << buffer[i].state << buffer[i].dataValue1;
            }

            //print state data with two datavalues
            switch(buffer[i].state) {
                case 'D': if(buffer[i].kind == Kind::Groupoff) break;
                case 'C':
                case 'T':
                case 'N':
                case 'd':
                case 'U':
                case 'S': if(buffer[i].kind != Kind::Student) break;
                case 'G': if(buffer[i].kind == Kind::BottlingPlant) break;
                case 'a':
                case 'B':
                case 'A': if(buffer[i].kind == Kind::Vending) break;
                case 't': cout << buffer[i].dataValue1 << "," << buffer[i].dataValue2;
            }
        } else {
            blankSpaces++;
        }
        buffer[i].assigned = false;//allow each column's print data to be overwritten for next flush
    }
    cout << endl;
}

//returns the index in the buffer array corresponding to the given kind and lid values
unsigned int Printer::getBufferIdFromKind(Kind kind, int lid) {
    switch(kind) {
        case Kind::Parent: return 0;
        case Kind::Groupoff: return 1;
        case Kind::WATCardOffice: return 2;
        case Kind::NameServer: return 3;
        case Kind::Truck: return 4;
        case Kind::BottlingPlant: return 5;
        case Kind::Student: return 6 + lid;
        case Kind::Vending: return 6 + numStudents + lid;
        case Kind::Courier: return 6 + numStudents + numVendingMachines + lid;
        default: return -1;
    }
}

//Helper function to prevent code duplication
//sets basic values of the message data and will flush all data if necessary
void Printer::printHelper(int id, Kind kind, char state) {
    if(buffer[id].assigned) flush();
    buffer[id].state = state;
    buffer[id].kind = kind;
    buffer[id].assigned = true;
}

//Add new message to buffer
void Printer::print(Kind kind, char state) {
    printHelper(getBufferIdFromKind(kind, 0), kind, state);
}

//Add new message to buffer
void Printer::print(Kind kind, char state, unsigned int value1) {
    print(kind, state);
    buffer[getBufferIdFromKind(kind, 0)].dataValue1 = value1;
}

//Add new message to buffer
void Printer::print(Kind kind, char state, unsigned int value1, unsigned int value2) {
    print(kind, state, value1);
    buffer[getBufferIdFromKind(kind, 0)].dataValue2 = value2;
}

//Add new message to buffer
void Printer::print(Kind kind, unsigned int lid, char state) {
    printHelper(getBufferIdFromKind(kind, lid), kind, state);
}

//Add new message to buffer
void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1) {
    print(kind, lid, state);
    buffer[getBufferIdFromKind(kind, lid)].dataValue1 = value1;
}

//Add new message to buffer
void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2) {
    print(kind, lid, state, value1);
    buffer[getBufferIdFromKind(kind, lid)].dataValue2 = value2;
}