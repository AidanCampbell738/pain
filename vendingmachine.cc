#include "vendingmachine.h"
#include "MPRNG.h"

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ) : prt( prt ), nameServer( nameServer ), id( id ), sodaCost( sodaCost ) {
    sodaInventory.resize( NUM_FLAVOURS );
}

// VendingMachine main task
void VendingMachine::main() {
    prt.print( Printer::Vending, 'S', sodaCost );
    for ( ;; ) {
        _Accept( ~VendingMachine ) {
            // Vending machine was deleted, stop
            prt.print( Printer::Vending, 'F' );
            break;
        }
        // When restocking begins, block other tasks until restocking ends
        or _Accept( inventory ) {
            _Accept( restocked );
        }
        or _Accept( buy );
    }
}

// Debit the cost of a soda from a student's account, and remove the selected soda from inventory
// Throws: Stock if the specified soda is out of stock, Funds if card lacks sufficient funds, Free if the soda was given for free
void VendingMachine::buy( Flavours flavour, WATCard & card ) {
    if ( sodaInventory[flavour] == 0 ) {
        // Selected flavour is out of stock
        _Throw Stock();
    }
    else if ( card.getBalance() < sodaCost ) {
        // Insufficient funds
        _Throw Funds();
    }
    else if ( mprng( 1, 5 ) == 1 ) {
        // Soda is free, throw an exception and remove soda
        sodaInventory[flavour] -= 1;
        prt.print( Printer::Vending, 'A' );
        _Throw Free();
    }
    else {
        // Debit amount from Watcard
        card.withdraw( sodaCost );
        // Remove soda from inventory
        sodaInventory[flavour] -= 1;
        prt.print( Printer::Vending, 'B', flavour, sodaInventory[flavour] );
    }
}

// Returns the current soda inventory, and prevents buying sodas until restocked() is called
unsigned int * VendingMachine::inventory() {
    prt.print( Printer::Vending, 'r' );
    // Return inventory pointer
    return &( sodaInventory[0] );
}

// Signal that restocking is finished
void VendingMachine::restocked() {
    prt.print( Printer::Vending, 'R' );
}

// Get the price of a soda
_Nomutex unsigned int VendingMachine::cost() const {
    return sodaCost;
}

// Get the id of this machine
_Nomutex unsigned int VendingMachine::getId() const {
    return id;
}