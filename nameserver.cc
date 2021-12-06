#include "nameserver.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ) {
    machines.resize( numVendingMachines );
    assignedMachines.resize( numStudents );
    allRegisteredLock.acquire();
}

void VMregister( VendingMachine * vendingmachine ) {
    registerLock.acquire();
    machines[nextId] = vendingmachine;
    nextId += 1;
    registerLock.release();
    if ( nextid == numVendingMachines ) {
        allRegisteredLock.release();
    }
}

VendingMachine * getMachine( unsigned int id ) {
    allRegisteredLock.acquire();
    allRegisteredLock.release();
    unsigned int machine = assignedMachines[id];
    assignedMachines[id] = ( machine + 1 ) % numVendingMachines;
    return machines[id];
}

VendingMachine ** getMachineList() {
    allRegisteredLock.acquire();
    allRegisteredLock.release();
    return &( machines[0] );
}