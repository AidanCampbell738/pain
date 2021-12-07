#pragma once 

#include "printer.h"
#include "watcard.h"

_Task Groupoff {
    struct Request {
        WATCard::FWATCard card;
    };
    std::vector<Request *> requests;
    unsigned int numReceived = 0;
    Printer & prt;
    unsigned int numStudents;
    unsigned int sodaCost;
    unsigned int groupoffDelay;
    void main();
  public:
    Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    WATCard::FWATCard giftCard();
};
