#include "nameserver.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ) {
    machines.resize( numVendingMachines );
    assignedMachines.resize( numStudents );
}

void NameServer::main() {
    prt.print( NameServer, 'S' );
    for ( ; nextid < numVendingMachines; ) {
        _Accept( VMregister );
    }
    for ( ;; ) {
        _Accept( ~NameServer ) {
            prt.print( NameServer, 'F' );
            break;
        }
        or _Accept( getMachine, getMachineList );
    }
}

void VMregister( VendingMachine * vendingmachine ) {
    prt.print( NameServer, 'R', vendingmachine->getId() );
    machines[nextId] = vendingmachine;
    nextId += 1;
}

VendingMachine * getMachine( unsigned int id ) {
    unsigned int machine = assignedMachines[id];
    assignedMachines[id] = ( machine + 1 ) % numVendingMachines;
    prt.print( NameServer, 'N', id, machine );
    return machine;
}

VendingMachine ** getMachineList() {
    return &( machines[0] );
}