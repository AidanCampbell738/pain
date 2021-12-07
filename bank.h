#pragma once 

_Monitor Bank {
    std::vector<unsigned int> balances;
    uCondLock depositSignal;
  public:
    Bank( unsigned int numStudents );
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};