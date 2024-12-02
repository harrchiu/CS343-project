#ifndef _groupoff_h_
#define _groupoff_h_

#include "WATcard.h"

_Monitor Printer;

_Task Groupoff{
    Printer & printer;
    unsigned int numStudents, sodaCost, groupoffDelay;
    unsigned int giftCardsMade = 0; // need this to be numStudents before proceeding
    WATCard::FWATCard* cards;       // each students needs to get one
    void main();
  public:
    Groupoff(Printer& prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay);
    ~Groupoff();
    WATCard::FWATCard giftCard(unsigned int id);    // gift cards are just watcards
};


#endif // ifndef