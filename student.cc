#include "student.h"
#include "printer.h"
#include "nameserver.h"
#include "watcardoffice.h"
#include "groupoff.h"
#include "vendingmachine.h"
#include "MPRNG.h"

using namespace std;

//Helper function to prevent code duplication
//Will get a vending machine from nameserver and print the appropriate message
VendingMachine * Student::getVM() {
    VendingMachine * vm = nameServer.getMachine(id);//get machine
    printer.print(Printer::Student, id, 'V', vm->getId());//print message
    return vm;
}

//Student Task main
//It will first initialize the number of soda purchases, the favourite flavour, the watcard, giftcard, and initial vending machine
//After, it will loop to purchase numPurchases favourite soda
//A purchase attempt consists of a second for loop that will iterate until a buy is successful
//An unsuccessful buy attempt will be handled by the relevant catch block (lost, free, funds or stock)
//and then another attempt will be made (without exiting the inner for loop)
void Student::main() {
    int numPurchases = mprng(1, maxPurchases);//number of sodas to buy
    int favFlavour = mprng(3);//favourite flavour
    printer.print(Printer::Student, id, 'S', favFlavour, numPurchases);

    WATCard::FWATCard myWatcard = cardOffice.create(id, 5);//create watcard
    WATCard::FWATCard myGiftcard = groupoff.giftCard();//get giftcard
    VendingMachine* vendingmachine = getVM();//get initial vending machine

    //Buy numPurchases number of favourite soda
    for(int i = 0; i < numPurchases; i++) {
        yield(mprng(1, 10));//randomly yield before attempting a purchase

        //Keep looping until a purchase has been made
        for(;;) {
            try {
                //Choose a giftcard, if available
                if(myGiftcard.available()) {
                    vendingmachine->buy((VendingMachine::Flavours)favFlavour, *myGiftcard());//try to buy
                    printer.print(Printer::Student, id, 'G', favFlavour, myGiftcard()->getBalance());
                    myGiftcard.reset();
                } else {
                    vendingmachine->buy((VendingMachine::Flavours)favFlavour, *myWatcard());//try to buy
                    printer.print(Printer::Student, id, 'B', favFlavour, myWatcard()->getBalance());
                }
                break;//exit for loop on successful purchase
            } _Catch(WATCardOffice::Lost&) {//Watcard was lost by courier
                printer.print(Printer::Student, id, 'L');//print message
                myWatcard = cardOffice.create(id, 5);//get new watcard
            } _Catch(VendingMachine::Free&) {//free soda
                if(myGiftcard.available()) {//print message
                    printer.print(Printer::Student, id, 'a', favFlavour, myGiftcard()->getBalance());
                } else {
                    printer.print(Printer::Student, id, 'A', favFlavour, myWatcard()->getBalance());
                }
                yield(4);//drink free soda
            } _Catch(VendingMachine::Funds&) {//not enough money to buy soda
                myWatcard = cardOffice.transfer(id, vendingmachine->cost() + 5, myWatcard());//get more money
                yield(mprng(1, 10));//yield before trying again
            } _Catch(VendingMachine::Stock&) {//favourite soda out of stock
                vendingmachine = getVM();//try a different vending machine
                yield(mprng(1, 10));//yield before trying again
            }
        }
    }
    printer.print(Printer::Student, id, 'F');
}