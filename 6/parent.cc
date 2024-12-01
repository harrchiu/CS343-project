
#include "parent.h"
#include "printer.h"
#include "bank.h"

#include <uPRNG.h>          // money and studentid

Parent::Parent(Printer& prt, Bank& bank, unsigned int numStudents, unsigned int parentalDelay) :
    printer(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {
    printer.print(Printer::Kind::Parent, 'S');  // starting print
}

// main task loop, donate and yield
void Parent::main() {
    for (;;) {      // yield busy wait
        _Accept(~Parent) {  // must check for destructor to know when to terminate
            break;
        } _Else{            // terminating else on accept statement (do not block)
            yield(parentalDelay);                        // yield before gift is transferred
            unsigned int money = prng(1, 3);             // [1,3]
            unsigned int studentId = prng(numStudents);  // [0,numStudents)
            giftsGiven += 1;
            bank.deposit(studentId, money);              // infinite money....
            printer.print(Printer::Kind::Parent, 'D', studentId, money);  // finishing print
        }
    }

    printer.print(Printer::Kind::Parent, 'F', giftsGiven);  // finishing print
}