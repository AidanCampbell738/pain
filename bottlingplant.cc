#include "bottlingplant.h"
#include <algorithm>

void BottlingPlant::main() {
    prt.print( BottlingPlant, 'S' );
    Truck truck( prt, nameServer, *this, numVendingMachines, maxStockPerFlavour );
    for ( ;; ) {
        _Accept( ~BottlingPlant ) {
            shutdown = true;
            break;
        }
        _Else {
            unsigned int bottlesProduced = 0;
            for ( unsigned int i = 0; i < NUM_FLAVOURS; i++ ) {
                cargo[i] = mprng( 0, maxShippedPerFlavour );
                bottlesProduced += cargo[i];
            }
            prt.print( BottlingPlant, 'G', bottlesProduced );
            shipmentReady = true;
            shipmentLock.signal();
            truckLock.wait();
            yield( timeBetweenShipments );
        }
    }
    prt.print( BottlingPlant, 'F' );
}

void BottlingPlant::getShipment( unsigned int cargo[ ] ) {
    if ( shutdown ) {
        _Throw Shutdown;
    }
    if ( !shipmentReady ) {
        shipmentLock.wait();
    }
    truckLock.signal();
    for ( unsigned int i = 0; i < NUM_FLAVOURS; i += 1 ) {
        cargo[i] = shipment[i];
    }
    prt.print( BottlingPlant, 'P' );
    shipmentReady = false;
}

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments ) : prt( prt ), nameServer( nameServer ), numVendingMachines( numVendingMachines ), maxShippedPerFlavour( maxShippedPerFlavour ), maxStockPerFlavour( maxStockPerFlavour ), timeBetweenShipments( timeBetweenShipments ) {
    shipment.resize( NUM_FLAVOURS );
}