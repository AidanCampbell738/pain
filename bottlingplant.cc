#include "bottlingplant.h"
#include "truck.h"
#include <algorithm>
#include "MPRNG.h"

//Wait for timeBetweenShipments
void BottlingPlant::produceShipment() {
    // Wait before producing another shipment
    yield( timeBetweenShipments );
    // Produce a new shipment
    unsigned int bottlesProduced = 0;
    for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ ) {
        // Generate random bottle production
        shipment[i] = mprng( 0, maxShippedPerFlavour );
        bottlesProduced += shipment[i];
    }
    prt.print( Printer::BottlingPlant, 'G', bottlesProduced );
    //Simplify (locks aren't necessary)
    // Signal truck to pick up shipment
    //shipmentReady = true;
    //shipmentLock.signal();
    //truckLock.wait();
}

// Main task for BottlingPlant
void BottlingPlant::main() {
    prt.print( Printer::BottlingPlant, 'S' );
    // Create truck for deliveries
    Truck truck( prt, nameServer, *this, numVendingMachines, maxStockPerFlavour );
    produceShipment();//produce initial shipment (delay must happen here)
    for ( ;; ) {
        // Check if plant is shutting down
        _Accept( ~BottlingPlant ) {
            shutdown = true;
            try {
                _Accept(getShipment); //wait for truck and shut it down
            } catch( uMutexFailure::RendezvousFailure & ) {}
            break;
        //I replaced _Else with this accept
        //According to Piazza 1056, we can't use an _Else here
        } or _Accept(getShipment) {
            produceShipment();
        }
    }
    prt.print( Printer::BottlingPlant, 'F' );
}


// Wait for, and retrieve, a shipment of bottles from the Plant, copying the shipment into cargo
// Can block
void BottlingPlant::getShipment( unsigned int cargo[ ] ) {
    if ( shutdown ) {
        // Plant is shutting down
        _Throw Shutdown();
    }
    //Locks aren't necessary
    // Shipment isn't ready yet, wait until it is
    /*if ( !shipmentReady ) {
        shipmentLock.wait();
    }
    // Signal a new production run to begin
    truckLock.signal();*/
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
