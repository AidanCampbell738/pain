#pragma once

#include "nameserver.h"
#include "printer.h"

_Task VendingMachine {
    Printer & prt;
    NameServer & nameServer;
    unsigned int id;
    unsigned int sodaCost;
    void main();
  public:
    enum Flavours { 
        FLAVOUR0 = 0,
        FLAVOUR1 = 1,
        FLAVOUR2 = 2,
        FLAVOUR3 = 3,
        NUM_FLAVOURS = 4
    }; // flavours of soda (YOU DEFINE)
    _Event Free {}; // free, advertisement
    _Event Funds {}; // insufficient funds
    _Event Stock {}; // flavour out of stock
    VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
    void buy( Flavours flavour, WATCard & card );
    unsigned int * inventory();
    void restocked();
    _Nomutex unsigned int cost() const;
    _Nomutex unsigned int getId() const;
};
