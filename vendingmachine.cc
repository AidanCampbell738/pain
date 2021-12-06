#include "vendingmachine.h"

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );

void VendingMachine::main() {
    for ( ;; ) {
        _Accept( inventory ) {
            _Accept( restocked );
        }
        or _Accept( buy );
    }
}

void VendingMachine::buy( Flavours flavour, WATCard & card ) {
    if ( sodaInventory[flavour] == 0 ) {
        _Throw Stock;
    }
    else if ( card.getBalance() < sodaCost ) {
        _Throw Funds;
    }
    else if ( mprng( 1, 5 ) == 1 ) {
        _Throw Free;
    }
    else {
        card.withdraw( sodaCost );
        sodaInventory[flavour] -= 1;
    }
}

unsigned int * VendingMachine::inventory() {
    return &( sodaInventory[0] );
}

void VendingMachine::restocked() {

}

_Nomutex unsigned int VendingMachine::cost() const {
    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() const {
    return id;
}