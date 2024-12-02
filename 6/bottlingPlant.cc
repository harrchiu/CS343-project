#include "bottlingPlant.h"

BottlingPlant::BottlingPlant(Printer& prt, NameServer& nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments) :
    printer(prt), nameServer(nameServer),
    numVendingMachines(numVendingMachines),
    maxStockPerFlavour(maxStockPerFlavour),
    maxShippedPerFlavour(maxShippedPerFlavour),
    timeBetweenShipments(timeBetweenShipments) {
}

void BottlingPlant::getShipment(unsigned int cargo[]) {
    for (unsigned int i = 0; i < NUM_OF_FLAVOURS; ++i) {
        cargo[i] = production[i];   // copy production to cargo
        production[i] = 0;          // reset production
    }
    truckWaiting.signalBlock();     // wait for new production
}

void BottlingPlant::main() {
    printer.print(Printer::Kind::BottlingPlant, 'S');
    Truck truck(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);    // create a truck

    for (;;) {
        // todo: check if we need to yield on the first run
        yield(timeBetweenShipments);    // perform production run

        unsigned int totalProduced = 0;
        for (unsigned int i = 0; i < NUM_OF_FLAVOURS; ++i) {
            production[i] = prng(maxShippedPerFlavour);
            totalProduced += production[i];
        }
        printer.print(Printer::Kind::BottlingPlant, 'G', totalProduced);

        _Accept(getShipment) {
            printer.print(Printer::Kind::BottlingPlant, 'P');   // log picked up
            truckWaiting.signalBlock(); // signal truck to pick up shipment
        } or _Accept(~BottlingPlant) {  // shutdown
            _Accept(getShipment) {
                _Resume Shutdown() _At truck;   // propagate exception to truck
                truckWaiting.signalBlock();
            }
            break;
        }
    }
    printer.print(Printer::Kind::BottlingPlant, 'F');
}