#pragma once

#include "printer.h"
#include "nameserver.h"

_Task BottlingPlant {
    uCondLock shipmentSignal;
    uCondLock truckSignal;
    Printer & prt;
    NameServer & nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;
    void main();
  public:
    _Event Shutdown {}; // shutdown plant
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[ ] );
};