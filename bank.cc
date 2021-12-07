#include "bank.h"

Bank( unsigned int numStudents ) {
    balances.resize( numStudents );
}

// Add specified funds to account with given id
void deposit( unsigned int id, unsigned int amount ) {
    balances[id] += amount;
    // Trigger accounts waiting for deposit to recheck balance
    depositSignal.broadcast();
}

// Remove the specified amount of funds from the account with the given id
// Can block
void withdraw( unsigned int id, unsigned int amount ) {
    // Wait until sufficient funds are present, checking whenever a deposit is made
    while ( balances[id] < amount ) {
        depositSignal.wait();
    }
    balances[id] -= amount;
}