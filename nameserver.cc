#include "nameserver.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ) {
    machines.resize( numVendingMachines );
    assignedMachines.resize( numStudents );
}

void NameServer::main() {
    for ( ; nextid < numVendingMachines; ) {
        _Accept( VMregister );
    }
    for ( ;; ) {
        _Accept( ~NameServer ) {
            break;
        }
        or _Accept( getMachine, getMachineList );
    }
}

void VMregister( VendingMachine * vendingmachine ) {
    machines[nextId] = vendingmachine;
    nextId += 1;
}

VendingMachine * getMachine( unsigned int id ) {
    unsigned int machine = assignedMachines[id];
    assignedMachines[id] = ( machine + 1 ) % numVendingMachines;
    return machines[id];
}

VendingMachine ** getMachineList() {
    return &( machines[0] );
}