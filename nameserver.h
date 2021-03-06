#pragma once

#include "printer.h"
#include "vendingmachine.h"
#include <vector>

_Task VendingMachine;

 _Task NameServer {
    std::vector<VendingMachine *> machines;
    std::vector<unsigned int> assignedMachines;
    unsigned int nextId = 0;
    Printer & prt;
    unsigned int numVendingMachines;
    unsigned int numStudents;
    void main();
  public:
    NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
    void VMregister( VendingMachine * vendingmachine );
    VendingMachine * getMachine( unsigned int id );
    VendingMachine ** getMachineList();
};