#pragma once

#include "printer.h"
#include "nameserver.h"
#include "watcardoffice.h"
#include "groupoff.h"

_Task Student {
    Printer &printer;//Printer reference
    NameServer& nameServer;//NameServer reference
    WATCardOffice& cardOffice;//WATCardOffice reference
    Groupoff& groupoff;//Groupoff reference
    unsigned int id, maxPurchases;//Student id, maximum number of soda purchases

    VendingMachine* getVM();//helper function to prevent code duplication    
	  void main();//tast main

  public:
	  Student(Printer& prt, NameServer& nameServer, WATCardOffice& cardOffice, Groupoff& groupoff,
			 unsigned int id, unsigned int maxPurchases) : printer(prt), nameServer(nameServer), 
             cardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases) {}//constructor
};
