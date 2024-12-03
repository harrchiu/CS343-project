#ifndef _student_h_
#define _student_h_

_Monitor Printer;

#include "WATcard.h"
#include "WATcardoffice.h"
#include "groupoff.h"   // giftcard
#include "nameServer.h"

_Task Student{
    Printer & printer;
    NameServer& nameServer;
    WATCardOffice& cardOffice;
    Groupoff& groupoff;
    unsigned int id;
    unsigned int maxPurchases;

    void main();
  public:
    Student(Printer& prt, NameServer& nameServer, WATCardOffice& cardOffice,
              Groupoff& groupoff,
              unsigned int id, unsigned int maxPurchases);
};

#endif  // _student_h_
