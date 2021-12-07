#include "nameserver.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ) {
    machines.resize( numVendingMachines );
    assignedMachines.resize( numStudents );
    // Calculate initial vending machine assignments for each student
    for ( unsigned int i = 0; i < numStudents; i += 1 ) {
        assignedMachines[i] = i % numVendingMachines;
    }
}

// Main task for NameServer
void NameServer::main() {
    prt.print( NameServer, 'S' );
    // Wait for all vending machines to register
    for ( ; nextid < numVendingMachines; ) {
        _Accept( VMregister );
    }
    for ( ;; ) {
        _Accept( ~NameServer ) {
            // NameServer has been deleted, stop
            prt.print( NameServer, 'F' );
            break;
        }
        // Process requests
        or _Accept( getMachine, getMachineList );
    }
}

// Register a vending machine with this server
void VMregister( VendingMachine * vendingmachine ) {
    // Add machine to list
    prt.print( NameServer, 'R', vendingmachine->getId() );
    machines[nextId] = vendingmachine;
    nextId += 1;
}

// Assign a vending machine to a given student
// Starts at id % numVendingMachines, an increments by 1 with each request
VendingMachine * getMachine( unsigned int id ) {
    // Return assigned machine and increment
    unsigned int machine = assignedMachines[id];
    assignedMachines[id] = ( machine + 1 ) % numVendingMachines;
    prt.print( NameServer, 'N', id, machine );
    return machine;
}

// Get the list of all registered vending machines
VendingMachine ** getMachineList() {
    return &( machines[0] );
}