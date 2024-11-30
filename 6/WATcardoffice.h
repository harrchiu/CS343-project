#ifndef _WAT_card_office_h_
#define _WAT_card_office_h_

#include "WATcard.h"
#include "bank.h"
#include "printer.h"
#include <queue>             // for queue of watcard creation jobs

struct WATCardOffice::Args {
  // args to create the card
  unsigned int sid;       // passed in
  unsigned int amount;    // starting amount
  WATCard* card;        // the card
}

_Task WATCardOffice{
    struct Job {							// marshalled arguments and return future
        Args args; 							// call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;			// return future
        Job(Args args) : args(args) {}
    };
    _Task Courier {
      void main()
      public:
        Courier()

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
    queue <

    Courier* couriers;
};


#endif // #ifndef
