#include "groupoff.h"

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) : prt( prt ), numStudents( numStudents ), sodaCost( sodaCost ), groupoffDelay( groupoffDelay ) {
    requests.resize( numStudents );
}

// Groupoff main task
void Groupoff::main() {
    prt.print( Groupoff, 'S' );
    // Wait for all students to submit a request
    while ( numReceived < numStudents ) {
        _Accept( giftCard );
    }
    // Loop until all requests are processed or the destructor is called
    while ( requests.size() > 0 ) {
        _Accept( ~Groupoff ) {
            // Groupoff was destroyed
            break;
        }
        _Else {
            // Wait for time between processing cards
            yield( groupoffDelay );
            // Choose recipient
            unsigned int i = mprng( 0, requests.size() - 1 );
            WATCard card;
            prt.print( Groupoff, 'D', sodaCost );
            // Deposit the cost of a soda on the card
            card.deposit( sodaCost );
            // Send the card to the student
            requests[i]->card.delivery( card );
            // Remove the request so that other requests can be processed
            delete requests[i];
            requests.remove( i );
        }
    }
    prt.print( Groupoff, 'F' );
}

// Return a future that will provide a WATCard gift card at a later time
// Should only be called once per student
WATCard::FWATCard Groupoff::giftCard() {
    requests[numReceived] = new Request();
    numReceived += 1;
    return requests[numReceived - 1]->card;
}