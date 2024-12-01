#ifndef _vending_machine_h_
#define _vending_machine_h_

_Monitor Printer;

#include "bottlingPlant.h"

_Task VendingMachine{
    Printer & printer;
    NameServer& nameServer;
    unsigned const int id;      // constant after assignment
    unsigned const int sodaCost;

    // inventory of all flavours - should be constant on runtime
    // get index from enum
    unsigned int sodaInventory[BottlingPlant::Flavours::NUM_OF_FLAVOURS];

    void main();
  public:
    _Exception Funds {};					// insufficient funds
    _Exception Stock {};					// flavour out of stock
    _Exception Free {};						// free, advertisement
    VendingMachine(Printer& prt, NameServer& nameServer, unsigned int id, unsigned int sodaCost);
    void buy(BottlingPlant::Flavours flavour, WATCard& card);
    unsigned int* inventory() __attribute__((warn_unused_result));
    void restocked();
    _Nomutex unsigned int cost() const;
    _Nomutex unsigned int getId() const;
};


#endif // ifndef