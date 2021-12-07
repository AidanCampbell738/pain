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

Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant, 
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) : prt( prt ), nameServer( nameServer ), plant( plant ), numVendingMachines( numVendingMachines ), maxStockPerFlavour( maxStockPerFlavour ) {}

void Truck::main() {
    VendingMachine ** vendingMachineList = nameServer.getMachineList();
    int nextToService = 0;
    for ( ;; ) {
        int firstToService = nextToService;
        unsigned int cargo[NUM_FLAVOURS];
        _Enable {
            try {
                plant.getShipment( cargo );
            }
            _Catch ( BottlingPlant::Shutdown ) {
                break;
            }
        }
        
        yield( mprng( 1, 10 ) );
        do {
            unsigned int * inventory = vendingMachineList[nextToService]->inventory();
            for ( unsigned int i = 0; i != NUM_FLAVOURS; i++ ) {
                unsigned int toStock = std::min( maxStockPerFlavour - inventory[i], cargo[i] );
                cargo[i] -= toStock;
                inventory[i] += toStock;
            }
            vendingMachineList[nextToService]->restocked();
            nextToService += 1;
            nextToService %= numVendingMachines;
            if ( mprng( 1, 100 ) == 42 ) {
                yield( 10 );
            }
        } while ( firstToService != nextToService && !isEmpty( cargo ) );
    }
}