#ifndef _WAT_card_office_h_
#define _WAT_card_office_h_

#include "WATcard.h"
#include "bank.h"
#include "printer.h"
#include <queue>             // for queue of watcard creation jobs

using namespace std;

_Monitor Bank;
_Monitor Printer;

_Task WATCardOffice{
  struct Args { // args to create the card
    unsigned int sid;       // passed in
    unsigned int amount;    // starting amount
    WATCard* card;        // the card
  };
  struct Job {							// marshalled arguments and return future
      Args args; 							// call arguments (YOU DEFINE "Args")
      WATCard::FWATCard result;			// return future
      Job(Args args) : args(args) {}
  };
  // can modify as fit
  _Task Courier{   // talks to bank to create watCard on Office::create()
      unsigned int courierId;
      WATCardOffice& watCardOffice;     // transfer and request work from office
      Printer& printer;
      Bank& bank;                       // make calls on behalf of student (transfer into watcard)
      WATCard* watCard;                 // the watcard in question?
      void main();
    public:
      Courier(unsigned int courierId, WATCardOffice& watCardOffice, Printer& printer, Bank& bank);
      ~Courier();
  };					// communicates with bank
  void main();
public:
  _Exception Lost {};						// lost WATCard
  WATCardOffice(Printer& prt, Bank& bank, unsigned int numCouriers);
  ~WATCardOffice();

  WATCard::FWATCard create(unsigned int sid, unsigned int amount) __attribute__((warn_unused_result));
  WATCard::FWATCard transfer(unsigned int sid, unsigned int amount, WATCard* card) __attribute__((warn_unused_result));
  Job* requestWork() __attribute__((warn_unused_result));

private:
  Printer& printer;
  Bank& bank;
  unsigned int numCouriers;
  queue<Job*> courierJobQueue;

  Courier** couriers; // array of courier pointers
};


#endif // #ifndef
