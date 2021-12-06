#include "bottlingplant.h"

void BottlingPlant::main() {
    Truck truck( prt, nameServer, *this, numVendingMachines, maxStockPerFlavour );
    for ( ;; ) {
        shipmentLock.acquire();
        if ( !shipmentSignal.empty() ) {
            shipmentSignal.signal();
        }
        truckSignal.wait( shipmentLock );
        shipmentLock.release();
        yield( timeBetweenShipments );
    }
}

void BottlingPlant::getShipment( unsigned int cargo[ ] ) {
    shipmentLock.acquire();
    if ( truckSignal.empty() ) {
        shipmentSignal.wait( shipmentLock );
    }
    for ( unsigned int i = 0; i < NUM_FLAVOURS; i++ ) {
        cargo[i] = maxShippedPerFlavour;
    }
    truckSignal.signal();
    shipmentLock.release();
}

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments ) : prt( prt ), nameServer( nameServer ), numVendingMachines( numVendingMachines ), maxShippedPerFlavour( maxShippedPerFlavour ), maxStockPerFlavour( maxStockPerFlavour ), timeBetweenShipments( timeBetweenShipments ) {}