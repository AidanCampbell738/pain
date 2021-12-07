#include "parent.h"

void Parent::main() {
    for ( ;; ) {
        _Accept( ~Parent ) {
            break;
        }
        _Else {
            yield( parentalDelay );
            bank.deposit( mprng( 0, numStudents - 1 ), mprng( 1, 3 ) );
        }
    }
}

Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay ) : prt( prt ), bank( bank ), numStudents( numStudents ), parentalDelay( parentalDelay ) {}