#pragma once 

#include "printer.h"
#include "bank.h"

_Task Parent {
    void main();
    Printer & prt;
    Bank & bank;
    unsigned int numStudents;
    unsigned int parentalDelay;
  public:
    Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
};