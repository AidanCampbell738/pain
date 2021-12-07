#pragma once

#include <uFuture.h>
#include <vector>
#include "bank.h"
#include "watcard.h"
#include "printer.h"

_Task WATCardOffice {

	struct Job {							// marshalled arguments and return future
		unsigned int sid, oldBalance, amount;//student id, old watcard balance, amount to add to old balance
		WATCard::FWATCard result;			// return future
		Job(unsigned int sid, unsigned int oldBalance, unsigned int amount) : 
            sid(sid), oldBalance(oldBalance), amount(amount) {}//constructor
	};

	_Task Courier { // communicates with bank
        Printer &printer;
        unsigned int id;//courier id (used in printing)
        bool shouldStop = false;//true when time to terminate
        Bank &bank;//reference to bank
        WATCardOffice &office;
        void main();//task main

        public:
            Courier(Printer &printer, unsigned int id, Bank &bank, WATCardOffice& office) : printer(printer), id(id), bank(bank), office(office) {}//constructor
            void stop();//call to terminate courier
    };

    Printer &printer;//reference to printer
    unsigned int numCouriers;//number of couriers
    std::vector<Job*> requests;//vector of requests (a vector is allowed as per piazza @1069)
    Courier** couriers;//array of couriers
    uCondition requestingWork;//condition that couriers block on when requesting a job

	void main();//task main
    WATCard::FWATCard cardHelper(unsigned int sid, unsigned int oldBalance, unsigned int amount);//helper to reduce code duplication
    void sendJob();//helper to reduce code duplication

  public:
	_Event Lost {};//lost watcard
	WATCardOffice(Printer& prt, Bank& bank, unsigned int numCouriers);//constructor
    ~WATCardOffice();//destructor
	WATCard::FWATCard create(unsigned int sid, unsigned int amount);//create watcard
	WATCard::FWATCard transfer(unsigned int sid, unsigned int amount, WATCard * card);//transfer from bank to watcard
	Job* requestWork();//courier calls to get job
};
