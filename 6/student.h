#ifndef _STUDENT_H_
#define _STUDENT_H_

#include "WATcard.h"
#include "groupoff.h"   // giftcard
using namespace std;

_Task Student{
    unsigned int id;
    unsigned int numPurchases, favFlavour;
    WATCard watCard;
    Groupoff groupoff;

    // wtf we need all these things first


        void main();
      public:
        Student(Printer& prt, NameServer& nameServer, WATCardOffice& cardOffice, Groupoff& groupoff,
                 unsigned int id, unsigned int maxPurchases);
};


#endif  // ifndef