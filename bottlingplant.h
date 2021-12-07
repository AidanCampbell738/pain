#pragma once

#include "printer.h"
#include "nameserver.h"
#include <vector>

_Task BottlingPlant {
    Printer & prt;
    NameServer & nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;
    std::vector<unsigned int> shipment;
    bool shutdown = false;
    bool shipmentReady = false;
    uCondition shipmentLock;
    uCondition truckLock;
    void main();
  public:
    _Event Shutdown {}; // shutdown plant
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[ ] );
};