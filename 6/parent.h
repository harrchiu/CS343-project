#ifndef _parent_h_
#define _parent_h_

_Monitor Bank;
_Monitor Printer;

_Task Parent{
    Printer & printer;
    Bank& bank;
    unsigned int numStudents, parentalDelay;
    unsigned int moneyGiven = 0;    // money given to all students
    void main();
  public:
    Parent(Printer& prt, Bank& bank, unsigned int numStudents, unsigned int parentalDelay);
};


#endif // ifndef