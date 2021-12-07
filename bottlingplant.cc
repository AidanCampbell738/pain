#include "bottlingplant.h"
#include "truck.h"
#include <algorithm>
#include "MPRNG.h"
#include <iostream>

// Main task for BottlingPlant
void BottlingPlant::main() {
    prt.print( Printer::BottlingPlant, 'S' );
    // Create truck for deliveries
    Truck truck( prt, nameServer, *this, numVendingMachines, maxStockPerFlavour );
    for ( ;; ) {
        unsigned int bottlesProduced = 0;
        for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ ) {
            // Generate random bottle production
            shipment[i] = mprng( 0, maxShippedPerFlavour );
            bottlesProduced += shipment[i];
        }
        prt.print( Printer::BottlingPlant, 'G', bottlesProduced );
        // Signal truck to pick up shipment
        shipmentReady = true;
        shipmentLock.signal();
        // Check if plant is shutting down
        _Accept( ~BottlingPlant ) {
            shutdown = true;
            break;
        }
        or _Accept( getShipment ) {
            // Wait between production runs
            yield( timeBetweenShipments );
        }
    }
    prt.print( Printer::BottlingPlant, 'F' );
    // Allow truck to receive exception
    _Accept( getShipment );
}

// Wait for, and retrieve, a shipment of bottles from the Plant, copying the shipment into cargo
// Can block
void BottlingPlant::getShipment( unsigned int cargo[ ] ) {
    if ( shutdown ) {
        // Plant is shutting down
        uRendezvousAcceptor();
        _Throw Shutdown();
    }
    // Shipment isn't ready yet, wait until it is
    if ( !shipmentReady ) {
        shipmentLock.wait();
    }
    // Copy shipment into cargo
    for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i += 1 ) {
        cargo[i] = shipment[i];
    }
    prt.print( Printer::BottlingPlant, 'P' );
    shipmentReady = false;
}

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments ) : prt( prt ), nameServer( nameServer ), numVendingMachines( numVendingMachines ), maxShippedPerFlavour( maxShippedPerFlavour ), maxStockPerFlavour( maxStockPerFlavour ), timeBetweenShipments( timeBetweenShipments ) {
    shipment.resize( VendingMachine::NUM_FLAVOURS );
}