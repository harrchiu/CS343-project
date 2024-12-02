#include "truck.h"
#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"
#include "vendingMachine.h"

Truck::Truck(Printer& prt, NameServer& nameServer, BottlingPlant& plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour) :
    printer(prt), nameServer(nameServer), plant(plant),
    numVendingMachines(numVendingMachines),
    maxStockPerFlavour(maxStockPerFlavour) {
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
    unsigned int curMachine = 0;    // current machine to restock

    for (;;) {
        yield(prng(1, 10));     // yield for a random time between 1 and 10

        try {
            plant.getShipment(cargo);   // get shipment from bottling plant
        } _Catch(BottlingPlant::Shutdown&) {  // shutdown plant
            printer.print(Printer::Kind::Truck, 'F');
            return;
        }

        unsigned int totalStock = 0;
        for (unsigned int i = 0; i < BottlingPlant::Flavours::NUM_OF_FLAVOURS; i++) {
            totalStock += cargo[i];
        }
        printer.print(Printer::Kind::Truck, 'P', totalStock);

        unsigned int startingMachine = curMachine;  // save the starting machine
        while (totalStock > 0 && curMachine != startingMachine) {   // while there is soda on the truck and the truck has not made a complete cycle
            unsigned int* machineStock = vendingMachines[curMachine]->inventory();
            printer.print(Printer::Kind::Truck, 'd', curMachine, totalStock);

            unsigned int unsuccessful = 0;
            for (unsigned int i = 0; i < BottlingPlant::Flavours::NUM_OF_FLAVOURS; i++) {  // restock each flavour
                unsigned int flavourRestocked = maxStockPerFlavour - machineStock[i];
                if (flavourRestocked > cargo[i]) {      // if the machine needs more than what is on the truck, restock the machine with what is on the truck
                    totalStock -= cargo[i];
                    unsuccessful += (flavourRestocked - cargo[i]);  // add the amount that was not restocked to the unsuccesful amount
                    cargo[i] = 0;
                    continue;
                }
                totalStock -= flavourRestocked;  // if the machine needs less than what is on the truck, subtract the whole amount
                cargo[i] -= flavourRestocked;
            }

            if (unsuccessful > 0) printer.print(Printer::Kind::Truck, 'U', curMachine, unsuccessful);   // unsuccesful restock

            vendingMachines[curMachine]->restocked();   // restock the machine complete
            printer.print(Printer::Kind::Truck, 'D', curMachine, totalStock);
            curMachine = (curMachine + 1) % numVendingMachines; // move to the next machine
        }

        if (prng(100) == 0) {   // 1 in 100 chance of having a flat tire after delivery
            printer.print(Printer::Kind::Truck, 'W');
            yield(10);   // yield 10 times to simulate fixing the flat tire
        }
    }
}