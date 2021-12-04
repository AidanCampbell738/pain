#include <iostream>
#include "printer.h"

using namespace std;

Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers) : 
    numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers) {
    numCols = 6 + numStudents + numVendingMachines + numCouriers;
    buffer = new Data[numCols];//create buffer array

    //Print all titles
    cout << "Parent\tGropoff\tWATOff\tNames\tTruck\tPlant";
    for(int i = 0; i < numStudents; i++) {//students
        cout << "\tStud" << i;
    }
    for(int i = 0; i < numVendingMachines; i++) {//vending machines
        cout << "\tMach" << i;
    }
    for(int i = 0; i < numCouriers; i++) {//courriers
        cout << "\tCour" << i;
    }
    cout << endl;

    //Print *******'s
    for(int i = 0; i < voters; i++) {
        if(i > 0) cout << "\t";
        cout << "*******";
    }
    cout << endl;
}

//Non-trivial destructor
//delete the buffer, print last values, and print final message
Printer::~Printer() {
    for(int i = 0; i < voters; i++) {
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
    for(int i = 0; i < numCols; i++) {
        if(buffer[i].assigned) {//only print stuff for voters with data to print
            for(int j = 0; j < blankSpaces + 1 && i > 0; j++) {
                cout << "\t";//print tab characters
            }
            blankSpaces = 0;
            switch(buffer[i].state) {//print data based on state
                case Voter::Start: cout << "S"; break;
                case Voter::Vote: cout << "V " << buffer[i].vote.picture << "," << buffer[i].vote.statue << "," << buffer[i].vote.giftshop; break;
                case Voter::Block: cout << "B " << buffer[i].numBlocked; break;
                case Voter::Unblock: cout << "U " << buffer[i].numBlocked; break;
                case Voter::Barging: cout << "b " << buffer[i].numBlocked << " " << buffer[i].group; break;
                case Voter::Done: cout << "D"; break;
                case Voter::Complete: cout << "C " << buffer[i].tour.tourkind; break;
                case Voter::Going: cout << "G " << buffer[i].tour.tourkind << " " << buffer[i].tour.groupno; break;
                case Voter::Failed: cout << "X"; break;
                case Voter::Terminated: cout << "T"; remainingVoters--; break;
            }
        } else {
            blankSpaces++;
        }
        buffer[i].assigned = false;//allow each voter's print data to be overwritten for next flush
    }
    cout << endl;
}


void print( Kind kind, char state );
void print( Kind kind, char state, unsigned int value1 );
void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
void print( Kind kind, unsigned int lid, char state );
void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );