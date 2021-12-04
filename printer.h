#ifndef _PRINTER_
#define _PRINTER_

_Monitor Printer {
    int numStudents, numVendingMachines, numCouriers, numCols;
    //Struct to store data used for printing
    //alternative to building and storing strings
    struct Data {
        bool assigned = false;//used to determine when to flush
        char state;//state (S, D, N, etc.)
        //the variables below are ints passed in print members
        //these dataValues take on the form of whatever data accompanies the print message
        //For example, for a parent's D message, dataValue1 would represent the student s
        //receiving the gift and dataValue2 would represent the amount of gift g
        int dataValue1, int dataValue2;
    };
    Data* buffer;//array to store each Kind's next print data
    void flush();//print a line of data

  public:
	enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
	Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();//added destructor
	void print( Kind kind, char state );
	void print( Kind kind, char state, unsigned int value1 );
	void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
	void print( Kind kind, unsigned int lid, char state );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
};

#endif