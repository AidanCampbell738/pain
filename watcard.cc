#include "watcard.h"

//deposit amount into watcard -> increase balance by amount
void WATCard::deposit(unsigned int amount) {
    balance += amount;
}

//withdraw amount from watcard balance -> decrease balance by amount
void WATCard::withdraw(unsigned int amount) {
    balance -= amount;
}

//getter function for balance
unsigned int WATCard::getBalance() {
    return balance;
}