#include "parent.h"
#include "MPRNG.h"

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
            unsigned int student = mprng( 0, numStudents - 1 );
            unsigned int amount = mprng( 1, 3 );
            prt.print( Printer::Parent, 'D', student, amount );
            // Deposit amount to student
            bank.deposit( student, amount );
        }
    }
}

Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay ) : prt( prt ), bank( bank ), numStudents( numStudents ), parentalDelay( parentalDelay ) {}