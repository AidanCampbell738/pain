#include "truck.h"
#include <algorithm>

// Helper function for calculating the number of bottles in a given shipment
// Cargo: shipment array, size: number of flavours
unsigned int getShipmentSize( unsigned int cargo[], unsigned int size ) {
    unsigned int total = 0;
    for ( unsigned int i = 0; i < size; i += 1 ) {
        total += cargo[i];
    }
    return total;
}

Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant, 
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) : prt( prt ), nameServer( nameServer ), plant( plant ), numVendingMachines( numVendingMachines ), maxStockPerFlavour( maxStockPerFlavour ) {}

// Main task for truck
void Truck::main() {
    prt.print( Truck, 'S' );
    // Retrieve machines to deliver to
    VendingMachine ** vendingMachineList = nameServer.getMachineList();
    int nextToService = 0;
    L0: for ( ;; ) {
        // Maintain index where we started, so we know when we've completed a full loop
        int firstToService = nextToService;
        unsigned int cargo[NUM_FLAVOURS];
        unsigned int bottlesLeft;
        // Grab a coffee
        yield( mprng( 1, 10 ) );
        _Enable {
            try {
                // Retrieve a shipment from the bottling plant
                plant.getShipment( cargo );
                bottlesLeft = getShipmentSize( cargo, NUM_FLAVOURS )
                prt.print( Truck, 'P', bottlesLeft );
            }
            _Catch ( BottlingPlant::Shutdown ) {
                // Plant is shutting down, so we're done
                break L0;
            }
        }
        L1: do {
            if ( bottlesLeft == 0 ) {
                // Shipment is empty, get a new shipment
                break L1;
            }
            prt.print( Truck, 'd', nextToService, bottlesLeft );
            // Retrieve the vending machine inventory, signalling that restocking has started
            unsigned int * inventory = vendingMachineList[nextToService]->inventory();
            // Restock each flavour
            for ( unsigned int i = 0; i != NUM_FLAVOURS; i++ ) {
                if ( cargo[i] < maxStockPerFlavour - inventory[i] ) {
                    // We don't have enough cargo to fully restock
                    inventory[i] += cargo[i];
                    prt.print( Truck, 'U', nextToService, maxStockPerFlavour - inventory[i] );
                    cargo[i] = 0;
                }
                else {
                    // We have enough cargo to fully restock
                    cargo[i] -= maxStockPerFlavour - inventory[i];
                    inventory[i] = maxStockPerFlavour;
                }
            }
            // Signal to vending machine that restocking is done
            vendingMachineList[nextToService]->restocked();
            // Calculate remaining cargo
            bottlesLeft = getShipmentSize( cargo, NUM_FLAVOURS );
            prt.print( Truck, 'D', nextToService, bottlesLeft );
            // Service next vending machine
            nextToService += 1;
            nextToService %= numVendingMachines;
            if ( mprng( 1, 100 ) == 42 ) {
                // Flat tire
                prt.print( Truck, 'X' );
                yield( 10 );
            }
            // Continue until we complete a full loop
        } while ( firstToService != nextToService );
    }
    prt.print( Truck, 'F' );
}