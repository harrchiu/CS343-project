#ifndef _vending_machine_h_
#define _vending_machine_h_

#include "bottlingPlant.h"
#include "watcard.h"

_Monitor Printer;

_Task NameServer;   // fwd reference

_Task VendingMachine{
    Printer & printer;
    NameServer& nameServer;
    unsigned const int id;      // constant after assignment
    unsigned const int sodaCost;

    bool isBeingRestocked = false;  // need a state to keep track if the VM is currently being stocked


    // inventory of all flavours - should be constant on runtime
    // get index from enum
    unsigned int sodaInventory[BottlingPlant::Flavours::NUM_OF_FLAVOURS];

    // for customer's order to be linked back to main(), where the logic should happen
    BottlingPlant::Flavours customerFlavour;
    WATCard& customerCard;
    uCondition buyCond; // to block or allow blocked buyer to continue
    enum BuyException { FUNDS, STOCK, FREE, NONE }; // communicate which error happened back to buyer
    BuyException buyException = NONE;

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