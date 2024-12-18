#include "truck.h"
#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"
#include "vendingMachine.h"
#include <iostream>

Truck::Truck(Printer& prt, NameServer& nameServer, BottlingPlant& plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour) :
    printer(prt), nameServer(nameServer), plant(plant),
    numVendingMachines(numVendingMachines),
    maxStockPerFlavour(maxStockPerFlavour) {

    // initialize the cargo for each flavour
    cargo = new unsigned int[BottlingPlant::Flavours::NUM_OF_FLAVOURS];
    for (int i = 0;i < BottlingPlant::Flavours::NUM_OF_FLAVOURS;i++) {
        cargo[i] = 0;
    }
}

Truck::~Truck() {
    delete[] cargo;
}

void Truck::main() {
    printer.print(Printer::Kind::Truck, 'S');
    VendingMachine** vendingMachines = nameServer.getMachineList();     // get list of vending machines
    unsigned int curMachine = 0;                        // current machine to restock

    struct done_error {};
    try {
        for (;;) {
            yield(prng(1, 10));                         // yield for random time [1, 10]

            try {
                _Enable{
                    plant.getShipment(cargo);           // get shipment from bottling plant
                };
            } _CatchResume(BottlingPlant::Shutdown&) {  // shutdown plant
                printer.print(Printer::Kind::Truck, 'F');

                throw done_error();                     // throw to quit main
            }

            unsigned int numBottlesTotal = 0;           // for return print
            for (unsigned int i = 0; i < BottlingPlant::Flavours::NUM_OF_FLAVOURS; i++) {
                numBottlesTotal += cargo[i];
            }
            printer.print(Printer::Kind::Truck, 'P', numBottlesTotal);

            for (unsigned int i = 0; numBottlesTotal > 0 && i < numVendingMachines; i++) {              // while there is soda on the truck and the truck has not made a complete cycle
                unsigned int* machineStock = vendingMachines[curMachine]->inventory();
                printer.print(Printer::Kind::Truck, 'd', curMachine, numBottlesTotal);

                unsigned int unsuccessful = 0;
                for (unsigned int i = 0; i < BottlingPlant::Flavours::NUM_OF_FLAVOURS; i++) {  // restock each flavour
                    unsigned int flavourRestocked = maxStockPerFlavour - machineStock[i];

                    if (flavourRestocked > cargo[i]) {      // if the machine needs more than what is on the truck, restock the machine with what is on the truck
                        numBottlesTotal -= cargo[i];
                        machineStock[i] += cargo[i];
                        unsuccessful += flavourRestocked - cargo[i];  // add the amount that was not restocked to the unsuccesful amount
                        cargo[i] = 0;
                        continue;
                    }

                    numBottlesTotal -= flavourRestocked;         // if the machine needs less than what is on the truck, subtract the whole amount
                    machineStock[i] = maxStockPerFlavour;       // restock the machine with the max amount
                    cargo[i] -= flavourRestocked;
                }

                if (unsuccessful > 0) printer.print(Printer::Kind::Truck, 'U', curMachine, unsuccessful);   // unsuccesful restock

                vendingMachines[curMachine]->restocked();           // restock the machine complete
                printer.print(Printer::Kind::Truck, 'D', curMachine, numBottlesTotal);
                curMachine = (curMachine + 1) % numVendingMachines; // move to the next machine
            }

            if (prng(100) == 0) {   // 1 in 100 chance of having a flat tire after delivery
                printer.print(Printer::Kind::Truck, 'W');
                yield(10);          // yield 10 times to simulate fixing the flat tire
            }
        }
    }
    catch (done_error&) {}   // catch to be escape 
}