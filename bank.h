#pragma once 

#include <vector>

_Monitor Bank {
    std::vector<unsigned int> balances;
    uCondition ** depositSignals;
    unsigned int numStudents;
  public:
    ~Bank();
    Bank( unsigned int numStudents );
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};