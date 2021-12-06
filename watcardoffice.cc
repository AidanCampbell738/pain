#include "watcardoffice.h"
#include "watcard.h"

using namespace std;

//Constructor
//stores arguments and creates couriers
WATCardOffice::WATCardOffice(Printer& prt, Bank& bank, unsigned int numCouriers) : 
    printer(prt), numCouriers(numCouriers) {
    //Create couriers
    Courier* tempCouriers[numCouriers];
    for(int i = 0; i < numCouriers; i++) {
        tempCouriers[i] = new Courier(i, bank);
    }
    couriers = tempCouriers;//set couriers member array
}

//Destructor
//deletes all couriers (after termination)
WATCardOffice::~WATCardOffice() {
    //delete all couriers
    for(int i = 0; i < numCouriers; i++) {
        delete couriers[i];
    }
}

//Courier Task main
//will request jobs from the office and may block
//after, will get money from that student's bank account
//will create new watcard or will lose watcard (1/6 chance) and raise exception
//will terminate after stop() is called
WATCardOffice::Courier::main() {
    printer.print(Printer::Courier, id, 'S');
    for(;;) {
        if(stop) break;
        Job* currentJob = requestWork();//request job from WATCardOffice (will block)
        if(stop) break;

        //Withdraw money from the bank
        printer.print(Printer::Courier, id, 't', currentJob->sid, currentJob->amount);
        bank.withdraw(currentJob->sid, currentJob->amount);
        printer.print(Printer::Courier, id, 'T', currentJob->sid, currentJob->amount);

        if(mprng(5) == 0) { //lost watcard
            printer.print(Printer::Courier, id, 'L', currentJob->sid);
            currentJob->result = new Lost;//put exception at future spot
        } else {
            //Create WATCard
            WATCard* watcard = new WATCard();
            watcard->deposit(oldBalance + amount);
            currentJob->result = watcard;
        }
        delete currentJob;
    }
    printer.print(Printer::Courier, id, 'F');
}

//stop
//terminate courier
WATCardOffice::Courier::stop() {
    stop = true;
}

//WATCardOffice Task main
//will accept create and transfer requests from students
//will delegate task to calling couriers
//when terminated (passed an sid of -1), will terminate all couriers and unblock waiting couriers
WATCardOffice::main() {
    printer.print(Printer::WATCardOffice, 'S');
    for(;;) {
        _Accept(create) {
            if(requests.front().sid == -1)  break;
            printer.print(Printer::WATCardOffice, 'C', requests.front().sid, requests.front().amount);
        } or _Accept(transfer) {
            if(requests.front().sid == -1)  break;
            printer.print(Printer::WATCardOffice, 'T', requests.front().sid, requests.front().amount);
        }
        requestingWork.signalBlock();
        requests.pop_front();  
        printer.print(Printer::WATCardOffice, 'W');
    }
    //terminate couriers
    for(int i = 0; i < numCouriers; i++) {
        couriers[i].stop();
    }
    //unblock all waiting couriers
    while(!waitForResult.empty()) {
        waitForResult.signalBlock();
    }
    printer.print(Printer::WATCardOffice, 'F');
}

//Helper function to prevent code duplication
//used by both create and transfer to create a new job and add it to the requests vector
WATCard::FWATCard cardHelper(unsigned int sid, unsigned int oldBalance, unsigned int amount) {
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
    return cardHelper(sid, card->getBalance(), amount);
}

//called by couriers to get a job
//will block
WATCardOffice::Job* WATCardOffice::requestWork() {
    requestingWork.wait();
    return requests.front();
}