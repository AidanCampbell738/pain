#include "bank.h"

Bank::Bank( unsigned int numStudents ) : numStudents( numStudents ) {
    balances.resize( numStudents );
    depositSignals = new uCondition * [numStudents];
    for ( unsigned int i = 0; i < numStudents; i += 1 ) {
        depositSignals[i] = new uCondition();
    }
}

Bank::~Bank() {
    for ( unsigned int i = 0; i < numStudents; i += 1 ) {
        delete depositSignals[i];
    }
    delete [] depositSignals;
}

// Add specified funds to account with given id
void Bank::deposit( unsigned int id, unsigned int amount ) {
    balances[id] += amount;
    // Trigger accounts waiting for deposit to recheck balance
    depositSignals[id]->signal();
}

// Remove the specified amount of funds from the account with the given id
// Can block
void Bank::withdraw( unsigned int id, unsigned int amount ) {
    // Wait until sufficient funds are present, checking whenever a deposit is made
    while ( balances[id] < amount ) {
        depositSignals[id]->wait();
    }
    balances[id] -= amount;
}