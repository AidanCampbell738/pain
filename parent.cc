#include "parent.h"

void Parent::main() {
    prt.print( Parent, 'S' );
    for ( ;; ) {
        _Accept( ~Parent ) {
            prt.print( Parent, 'F' );
            break;
        }
        _Else {
            yield( parentalDelay );
            unsigned int student = mprng( 0, numStudents - 1 );
            unsigned int amount = mprng( 1, 3 );
            prt.print( Parent, 'D', student, amount );
            bank.deposit( student, amount );
        }
    }
}

Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay ) : prt( prt ), bank( bank ), numStudents( numStudents ), parentalDelay( parentalDelay ) {}