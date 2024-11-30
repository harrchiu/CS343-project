#include "WATcardoffice.h"
#include "WATcard.h"
#include "bank.h"
#include "printer.h"

#include <queue>  


WATCardOffice::WATCardOffice(Printer& prt, Bank& bank, unsigned int numCouriers) :
    printer(printer), bank(bank), numCouriers(numCouriers) {

    printer.print(Printer::Kind::WATCardOffice, 'S');    // starting

    // create the couriers  // here or main?
    couriers = new Courier[numCouriers];
}
~WATCardOffice::WATCardOffice() {
    delete[] couriers;
    printer.print(Printer::Kind::WATCardOffice, 'F');    // finished
}


WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) __attribute__((warn_unused_result)) {
    Args args = { sid, amount, nullptr }; // args to be passed in to creation job
    // Args args = { sid, amount, WATCard() }; // ??
    Job* job = new Job(args);

    // i have no idea...
    {

    }
    printer.print(Printer::Kind::WATCardOffice, 'C', sid, amount);

    return job->result; // fwd pointer
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard* card) __attribute__((warn_unused_result)) {

}

Job* WATCardOffice::requestWork() __attribute__((warn_unused_result)) {

}

WATCardOffice::main() {

}

/* ----------  COURIER ----------- */
