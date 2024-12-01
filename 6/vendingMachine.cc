#include "printer.h"
#include "vendingMachine.h"

VendingMachine::VendingMachine(Printer& prt, NameServer& nameServer, unsigned int id, unsigned int sodaCost) :
    printer(prt), nameServer(nameServer), id(id), sodaCost(sodaCost) {

    // has an lid
    printer.print(Printer::Kind::Vending, 'S', id, sodaCost);  // starting print


}

void VendingMachine::buy(BottlingPlant::Flavours flavour, WATCard& card) {

}

unsigned int* VendingMachine::inventory() {
    return sodaInventory;
}

void VendingMachine::restocked() {

}

// getters that are read-only
_Nomutex unsigned int VendingMachine::cost() const {
    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() const {
    return id;
}

void VendingMachine::main() {
    for (;;) {

    }

    printer.print(Printer::Kind::Vending, 'F', id);  // starting print
}