#include "bottlingplant.h"

void BottlingPlant::main() {
    Truck truck( prt, nameServer, *this, numVendingMachines, maxStockPerFlavour );
    for ( ;; ) {
        _Accept( ~BottlingPlant ) {
            shutdown = true;
            break;
        }
        _Else {
            if ( !shipmentSignal.empty() ) {
                shipmentSignal.signal();
            }
            truckSignal.wait();
            yield( timeBetweenShipments );
        }
    }
}

void BottlingPlant::getShipment( unsigned int cargo[ ] ) {
    if ( shutdown ) {
        _Throw Shutdown;
    }
    if ( truckSignal.empty() ) {
        shipmentSignal.wait();
    }
    for ( unsigned int i = 0; i < NUM_FLAVOURS; i++ ) {
        cargo[i] = maxShippedPerFlavour;
    }
    truckSignal.signal();
}

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments ) : prt( prt ), nameServer( nameServer ), numVendingMachines( numVendingMachines ), maxShippedPerFlavour( maxShippedPerFlavour ), maxStockPerFlavour( maxStockPerFlavour ), timeBetweenShipments( timeBetweenShipments ) {}