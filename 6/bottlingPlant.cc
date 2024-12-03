#include "bottlingPlant.h"
#include "truck.h"
#include "printer.h"
#include <iostream>

BottlingPlant::BottlingPlant(Printer& prt, NameServer& nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments) :
    printer(prt), nameServer(nameServer),
    numVendingMachines(numVendingMachines),
    maxShippedPerFlavour(maxShippedPerFlavour),
    maxStockPerFlavour(maxStockPerFlavour),
    timeBetweenShipments(timeBetweenShipments) {
}

void BottlingPlant::getShipment(unsigned int cargo[]) {
    for (unsigned int i = 0; i < NUM_OF_FLAVOURS; ++i) {
        cargo[i] = productionStock[i];   // move to cargo
        productionStock[i] = 0;          // get ready for next iter
    }
    truckWaiting.signalBlock();          // block for next
}

void BottlingPlant::main() {
    printer.print(Printer::Kind::BottlingPlant, 'S');       // starting print
    Truck truck(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);

    for (;;) {
        yield(timeBetweenShipments);            // production run

        unsigned int totalProduced = 0;
        for (unsigned int i = 0; i < NUM_OF_FLAVOURS; ++i) {
            productionStock[i] = prng(0, maxShippedPerFlavour);
            totalProduced += productionStock[i];
        }
        printer.print(Printer::Kind::BottlingPlant, 'G', totalProduced);

        _Accept(~BottlingPlant) {               // shutdown
            _Accept(getShipment) {
                _Resume Shutdown() _At truck;   // propagate exception to truck
                truckWaiting.signalBlock();
            }
            break;
        } or _Accept(getShipment) {
            printer.print(Printer::Kind::BottlingPlant, 'P');   // log picked up
            truckWaiting.signalBlock();                         // signal truck to pick up shipment
        }
    }
    printer.print(Printer::Kind::BottlingPlant, 'F');       // finishing print
}