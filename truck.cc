#include "truck.h"
#include <algorithm>

bool isEmpty( unsigned int cargo[], unsigned int size ) {
    for ( unsigned int i = 0; i < size; i++ ) {
        if ( cargo[i] != 0 ) {
            return false;
        }
    }
    return true;
}

unsigned int getShipmentSize( unsigned int cargo[], unsigned int size ) {
    unsigned int total = 0;
    for ( unsigned int i = 0; i < size; i += 1 ) {
        total += cargo[i];
    }
    return total;
}

Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant, 
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) : prt( prt ), nameServer( nameServer ), plant( plant ), numVendingMachines( numVendingMachines ), maxStockPerFlavour( maxStockPerFlavour ) {}

void Truck::main() {
    prt.print( Truck, 'S' );
    VendingMachine ** vendingMachineList = nameServer.getMachineList();
    int nextToService = 0;
    for ( ;; ) {
        int firstToService = nextToService;
        unsigned int cargo[NUM_FLAVOURS];
        unsigned int bottlesLeft;
        _Enable {
            try {
                plant.getShipment( cargo );
                bottlesLeft = getShipmentSize( cargo, NUM_FLAVOURS )
                prt.print( Truck, 'P', bottlesLeft );
            }
            _Catch ( BottlingPlant::Shutdown ) {
                break;
            }
        }
        
        yield( mprng( 1, 10 ) );
        do {
            if ( bottlesLeft == 0 ) {
                break;
            }
            prt.print( Truck, 'd', nextToService, bottlesLeft );
            unsigned int * inventory = vendingMachineList[nextToService]->inventory();
            for ( unsigned int i = 0; i != NUM_FLAVOURS; i++ ) {
                if ( cargo[i] < maxStockPerFlavour - inventory[i] ) {
                    inventory[i] += cargo[i];
                    prt.print( Truck, 'U', nextToService, maxStockPerFlavour - inventory[i] );
                    cargo[i] = 0;
                }
                else {
                    cargo[i] -= maxStockPerFlavour - inventory[i];
                    inventory[i] = maxStockPerFlavour;
                }
            }
            vendingMachineList[nextToService]->restocked();
            bottlesLeft = getShipmentSize( cargo, NUM_FLAVOURS );
            prt.print( Truck, 'D', nextToService, bottlesLeft );
            nextToService += 1;
            nextToService %= numVendingMachines;
            if ( mprng( 1, 100 ) == 42 ) {
                prt.print( Truck, 'X' );
                yield( 10 );
            }
        } while ( firstToService != nextToService );
    }
    prt.print( Truck, 'F' );
}