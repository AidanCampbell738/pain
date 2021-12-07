#include "bank.h"

Bank( unsigned int numStudents ) {
    balances.resize( numStudents );
}

void deposit( unsigned int id, unsigned int amount ) {
    balances[id] += amount;
    depositSignal.broadcast();
}

void withdraw( unsigned int id, unsigned int amount ) {
    while ( balances[id] < amount ) {
        depositSignal.wait();
    }
    balances[id] -= amount;
}