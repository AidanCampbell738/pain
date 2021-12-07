#include "watcardoffice.h"
#include "watcard.h"
#include "MPRNG.h"

using namespace std;

//Constructor  
//stores arguments and creates couriers
WATCardOffice::WATCardOffice(Printer& prt, Bank& bank, unsigned int numCouriers) : 
    printer(prt), numCouriers(numCouriers) {
    //Create couriers
    couriers = new Courier * [numCouriers];
    for(unsigned int i = 0; i < numCouriers; i++) {
        couriers[i] = new Courier(printer, i, bank, *this);
    }
}

//Destructor
//deletes all couriers (after termination)
WATCardOffice::~WATCardOffice() {
    //delete all couriers
    for(unsigned int i = 0; i < numCouriers; i++) {
        delete couriers[i];
    }
    delete [] couriers;
    for (Job* job : requests) {
        delete job;
    }
}

//Courier Task main
//will request jobs from the office and may block
//after, will get money from that student's bank account
//will create new watcard or will lose watcard (1/6 chance) and raise exception
//will terminate after stop() is called
void WATCardOffice::Courier::main() {
    printer.print(Printer::Courier, id, 'S');
    for(;;) {
        Job* currentJob;
        try {
            currentJob = office.requestWork();//request job from WATCardOffice (will block)
        }
        catch (Stop&) {
            break;
        }

        //Withdraw money from the bank
        printer.print(Printer::Courier, id, 't', currentJob->sid, currentJob->amount);
        bank.withdraw(currentJob->sid, currentJob->amount);

        if(mprng(5) == 0) { //lost watcard
            printer.print(Printer::Courier, id, 'L', currentJob->sid);
            currentJob->result.exception(new Lost);//put exception at future spot
        } else {
            //Create WATCard
            WATCard* watcard = new WATCard();
            watcard->deposit(currentJob->amount + currentJob->oldBalance);
            currentJob->result.delivery(watcard);
            printer.print(Printer::Courier, id, 'T', currentJob->sid, currentJob->amount);
        }
        delete currentJob;
    }
    printer.print(Printer::Courier, id, 'F');
}

//stop
//terminate courier
void WATCardOffice::Courier::stop() {
    shouldStop = true;
}

//helper to prevent code duplication
void WATCardOffice::sendJob() {
    requestingWork.signalBlock();
    requests.erase(requests.begin());
    printer.print(Printer::WATCardOffice, 'W');
}

//WATCardOffice Task main
//will accept create and transfer requests from students
//will delegate task to calling couriers
//when terminated (passed an sid of -1), will terminate all couriers and unblock waiting couriers
void WATCardOffice::main() {
    printer.print(Printer::WATCardOffice, 'S');
    for(;;) {
        _Accept(~WATCardOffice) {
            break;
        }
        or _Accept(create) {
            printer.print(Printer::WATCardOffice, 'C', requests.front()->sid, requests.front()->amount);
            if(!requestingWork.empty()) sendJob();
        } or _Accept(transfer) {
            printer.print(Printer::WATCardOffice, 'T', requests.front()->sid, requests.front()->amount);
            if(!requestingWork.empty()) sendJob();
        } or _Accept(requestWork) {
            if(requests.size() > 0) sendJob();
        }
    }
    //terminate couriers
    stop = true;
    //unblock all waiting couriers
    for(unsigned int i = 0; i < numCouriers; i++) {
        requestingWork.signalBlock();
    }
    printer.print(Printer::WATCardOffice, 'F');
}

//Helper function to prevent code duplication
//used by both create and transfer to create a new job and add it to the requests vector
WATCard::FWATCard WATCardOffice::cardHelper(unsigned int sid, unsigned int oldBalance, unsigned int amount) {
    //create new Job
    Job* j = new Job(sid, oldBalance, amount);
    //add job to list
    requests.push_back(j);
    //return watcard future to student
    return j->result;
}

//return future of new watcard
WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
    return cardHelper(sid, 0, amount);
}

//return future of new watcard after transfer
WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard* card) {
    unsigned int oldBalance = card->getBalance();
    delete card;
    return cardHelper(sid, oldBalance, amount);
}

//called by couriers to get a job
//will block
WATCardOffice::Job* WATCardOffice::requestWork() {
    if (stop){
        _Throw Stop();
    }
    requestingWork.wait();
    if (stop){
        _Throw Stop();
    }
    return requests.front();
}
