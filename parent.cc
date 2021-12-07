#include "parent.h"
#include "MPRNG.h"
#include <iostream>

// Parent main task
void Parent::main() {
    prt.print( Printer::Parent, 'S' );
    for ( ;; ) {
        _Accept( ~Parent ) {
            // Parent has been deleted, stop
            prt.print( Printer::Parent, 'F' );
            break;
        }
        _Else {
            // Delay between deposits
            yield( parentalDelay );
            // Select random student and amount from legal values
            unsigned int amount = mprng( 1, 3 );
            unsigned int student = mprng( 0, numStudents - 1 );
            //prt.print( Printer::Parent, 'D', student, amount );
            // Deposit amount to student
            bank.deposit( student, amount );
        }
    }
}

Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay ) : prt( prt ), bank( bank ), numStudents( numStudents ), parentalDelay( parentalDelay ) {}
