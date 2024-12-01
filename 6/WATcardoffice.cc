#include "WATcardoffice.h"
#include "WATcard.h"
#include "bank.h"
#include "printer.h"

#include <queue>  
#include <uPRNG.h>          // lost watcard

using namespace std;

WATCardOffice::WATCardOffice(Printer& prt, Bank& bank, unsigned int numCouriers) :
    printer(prt), bank(bank), numCouriers(numCouriers) {

    printer.print(Printer::Kind::WATCardOffice, 'S');    // starting

    // create the couriers  // here or main?
    couriers = new Courier * [numCouriers];
    for (unsigned int i = 0;i < numCouriers;i++) {
        // give ptr to this office
        couriers[i] = new Courier(i, *this, printer, bank);
    }
}
// office dtor
WATCardOffice::~WATCardOffice() {

    // clean up pending jobs
    while (!courierJobQueue.empty()) {
        delete courierJobQueue.front();
        courierJobQueue.pop();
    }
    // call requestWork until no more couriers (need to do this numCourier times)
    // to make sure couriers exit their main()
    for (unsigned int i = 0; i < numCouriers; ++i) {
        _Accept(requestWork);
    }

    // delete allocated memory
    // have to loop since we used ctor on each
    for (unsigned int i = 0;i < numCouriers;i++) {
        delete couriers[i];
    }
    printer.print(Printer::Kind::WATCardOffice, 'F');    // finished
}

// create() and transfer() have the same function of getting funds from the bank
WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
    // "student calls this async to create a "real" watCard with an initial balance"
    // the Job object contains this watCard (pointer) in args
    // when the job is finished (starting from requestWork), we update the 
    // watCard pointer with the future one for the client to receive
    Args args = { sid, amount, new WATCard() }; // args to be passed in to creation job

    Job* job = new Job(args);
    courierJobQueue.push(job);     // add job to queue
    printer.print(Printer::Kind::WATCardOffice, 'C', sid, amount);
    return job->result; // fwd pointer
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard* card) {
    // same thing as create()
    Args args = { sid, amount, card }; // args to be passed in to creation job

    Job* job = new Job(args);
    courierJobQueue.push(job);     // add job to queue
    printer.print(Printer::Kind::WATCardOffice, 'T', sid, amount);
    return job->result; // fwd pointer
}

// pop work from queue for a courier
WATCardOffice::Job* WATCardOffice::requestWork() {
    // search for next job
    if (courierJobQueue.empty()) {
        return nullptr;  // do nothing, let the courier main wait again
    }
    Job* nextJob = courierJobQueue.front();
    courierJobQueue.pop();
    printer.print(Printer::Kind::WATCardOffice, 'W');   // request work call complete
    return nextJob;
}

// watcard office main loop
void WATCardOffice::main() {
    // start printed in ctor
    for (;;) {
        _Accept(~WATCardOffice) {
            break;  // stop this loop
        }
        or _When(!courierJobQueue.empty()) _Accept(requestWork) {}
        or _Accept(create || transfer) {}
    }   // for
    // finish is printed in destructor
}

/* ----------  COURIER ----------- */
// handles getting funds from the bank to a given WATCard pointer, returns future
WATCardOffice::Courier::Courier(unsigned int courierId, WATCardOffice& watCardOffice, Printer& printer, Bank& bank) :
    courierId(courierId), watCardOffice(watCardOffice), printer(printer), bank(bank) {

    // need to print with ID
    printer.print(Printer::Kind::Courier, courierId, 'S'); // starting
};

// main for courier to transfer funds from student bank to WATCard (provided)
void WATCardOffice::Courier::main() {
    // start printer in ctor
    for (;;) {  // main loop, request work and fulfill it
        // get the latest job for this courier
        Job* currentJob = watCardOffice.requestWork();
        if (nullptr == currentJob) {
            // stop executing once there are no more jobs
            // this gets called after the office dtor runs
            // and cleans out the job queue, so this exits
            break;
        }

        // always a transfer job withd rawing from the bank
        Args jobArgs = currentJob->args;
        unsigned int studentId = jobArgs.sid;
        unsigned int amount = jobArgs.amount;
        WATCard* watCard = jobArgs.card;        // already created at this point

        printer.print(Printer::Kind::Courier, courierId, 't', studentId, amount); // starting transfer
        bank.withdraw(studentId, amount);       // always contacts bank first
        watCard->deposit(amount);

        // courier loses it 1/6th of the time! ("after the update")
        // set the job result accordingly
        if (prng(6) == 0) {        // generates [0,5] - lost
            // currentJob->result.exception(new Lost());
            currentJob->result.delivery(new Lost());   // exception is deprecated? if this doesn't work then
            printer.print(Printer::Kind::Courier, courierId, 'L', studentId); // lost watcard
        }
        else {  // went through 
            currentJob->result.delivery(watCard);
            printer.print(Printer::Kind::Courier, courierId, 'T', studentId, amount); // complete transfer
        }

        delete currentJob; // job is done and popped already
    }

    printer.print(Printer::Kind::Courier, courierId, 'F'); // finishing
}
