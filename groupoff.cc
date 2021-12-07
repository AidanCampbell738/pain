#include "groupoff.h"

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) : prt( prt ), numStudents( numStudents ), sodaCost( sodaCost ), groupoffDelay( groupoffDelay ) {
    requests.resize( numStudents );
}

void Groupoff::main() {
    while ( numReceived < numStudents ) {
        _Accept( giftCard );
    }
    while ( requests.size() > 0 ) {
        _Accept( ~Groupoff ) {
            break;
        }
        _Else {
            yield( groupoffDelay );
            unsigned int i = mprng( 0, requests.size() - 1 );
            WATCard card;
            card.deposit( sodaCost );
            requests[i]->card.delivery( card );
            delete requests[i];
            requests.remove( i );
        }
    }
}

WATCard::FWATCard Groupoff::giftCard() {
    requests[numReceived] = new Request();
    numReceived += 1;
    return requests[numReceived - 1]->card;
}