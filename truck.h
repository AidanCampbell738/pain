#pragma once

#include "printer.h"
#include "bottlingplant.h"
#include "nameserver.h"

_Task Truck {
    void main();
  public:
    Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant, 
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};
