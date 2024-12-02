#include "nameServer.h"
#include "printer.h"
#include "vendingMachine.h"
#include <iostream>

NameServer::NameServer(Printer& prt, unsigned int numVendingMachines, unsigned int numStudents) :
    printer(prt), numVendingMachines(numVendingMachines), numStudents(numStudents) {

    printer.print(Printer::Kind::NameServer, 'S');  // starting print

    modulos = new unsigned int[numStudents];
    memset(modulos, 0, numStudents * sizeof(*modulos));  // init all mods to 0

    // initialize space to hold all the vending machines (in registration order)
    vendingMachines = new VendingMachine * [numVendingMachines];    // empty for now
}

NameServer::~NameServer() {
    // delete array that holds the VMs
    delete[] vendingMachines;
    delete[] modulos;
    // TODO: ensure that the VMs themselves are deleted elsewhere... or that main does it...

    printer.print(Printer::Kind::NameServer, 'F');  // finishing print
}

void NameServer::VMregister(VendingMachine* vendingmachine) {
    // register by adding to current place in array
    printer.print(Printer::Kind::NameServer, 'R', vendingmachine->getId());
    vendingMachines[numRegisteredMachines] = vendingmachine;
    numRegisteredMachines += 1; // update for next
}

// student calls this to find a machine
VendingMachine* NameServer::getMachine(unsigned int id) {
    // the starting vm is the student id
    // now add the student's modulo so we can get their current cycle progress
    unsigned int vmIndex = (id + modulos[id]) % numVendingMachines;
    VendingMachine* vm = vendingMachines[vmIndex];

    // update so the student can cycle through all the vms
    modulos[id] += 1;
    modulos[id] %= numVendingMachines;

    printer.print(Printer::Kind::NameServer, 'N', id, vm->getId());

    return vm;
}

VendingMachine** NameServer::getMachineList() {
    return vendingMachines;
}

void NameServer::main() {
    // must register all vending machines before accepting calls to other members
    while (numRegisteredMachines < numVendingMachines) {
        _Accept(VMregister) {
            // print done in register
        }
    }

    for (;;) {
        _Accept(~NameServer) {
            return;
        }
        or _Accept(getMachineList) {}
        or _Accept(getMachine) {}
    }
    // finishing print in dtor;
}