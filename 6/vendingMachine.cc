#include "vendingMachine.h"
#include "printer.h"
#include "WATcard.h"
#include "nameServer.h"     // register

VendingMachine::VendingMachine(Printer& prt, NameServer& nameServer, unsigned int id, unsigned int sodaCost) :
    printer(prt), nameServer(nameServer), id(id), sodaCost(sodaCost) {

    for (unsigned int i = 0; i < BottlingPlant::Flavours::NUM_OF_FLAVOURS; ++i) {
        sodaInventory[i] = 0;
    }

    // has an lid
    printer.print(Printer::Kind::Vending, id, 'S', sodaCost);  // starting print
}

void VendingMachine::buy(BottlingPlant::Flavours flavour, WATCard& card) {
    customerFlavour = flavour;
    customerCard = &card; // ref
    buyCond.wait();     // let main figure out the error, then come back here to actually raise it

    switch (buyException) {
    case FUNDS: _Throw Funds();
    case STOCK: _Throw Stock();
    case FREE:  _Throw Free();
    default:    break; // No exception
    }
}


// a truck is beginning to restock, give it the pointer to the array with soda inventory
unsigned int* VendingMachine::inventory() {
    return sodaInventory;
}

// truck is signalling done restocking this VM
void VendingMachine::restocked() {

}

// getters that are read-only
_Nomutex unsigned int VendingMachine::cost() const {
    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() const {
    return id;
}


// main logic should be in main for sync purposes with customer
void VendingMachine::main() {
    // starting print in ctor

    nameServer.VMregister(this); // register with nameServer

    for (;;) {
        _Accept(~VendingMachine) {
            break;
        }
        or _Accept(inventory) { // should happen FIRST
            isBeingRestocked = true;
            printer.print(Printer::Kind::Vending, id, 'r');  // start reloading by truck
        }
        or _Accept(restocked) {    // need this reset flip after inventory
            isBeingRestocked = false;
            printer.print(Printer::Kind::Vending, id, 'R');  // complete reloading by truck
        }
        or _When(!isBeingRestocked) _Accept(buy) {  // cannot accept buy calls during restocking
            // need main to handle the exception
            // check first if sufficient funds, then if avail
            if (customerCard->getBalance() < sodaCost) {
                buyException = FUNDS;
            }
            else if (sodaInventory[customerFlavour] == 0) {
                buyException = STOCK;
            }
            else {  // purchase is possible
                if (prng(5) == 0) { // free
                    buyException = FREE;
                    printer.print(Printer::Kind::Vending, id, 'A');  // free drink
                }
                else {
                    buyException = NONE;
                    customerCard->withdraw(sodaCost);   // withdraw
                }
                sodaInventory[customerFlavour] -= 1;    // customer takes drink
            }
            buyCond.signalBlock();  // block so buy can throw exception
        }
    }   // for

    printer.print(Printer::Kind::Vending, id, 'F');  // finishing print
}