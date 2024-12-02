
#include "parent.h"
#include "printer.h"
#include "bank.h"

#include <uPRNG.h>          // money and studentid
#include <iostream>          // money and studentid

Parent::Parent(Printer& prt, Bank& bank, unsigned int numStudents, unsigned int parentalDelay) :
    printer(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {
    printer.print(Printer::Kind::Parent, 'S');  // starting print
}

// Parent::~Parent(){}

// main task loop, donate and yield
void Parent::main() {
    for (;;) {      // yield busy wait
        _Accept(~Parent) {  // must check for destructor to know when to terminate
            std::cout << "parent dtor called" << std::endl;
            printer.print(Printer::Kind::Parent, 'F', giftsGiven);  // finishing print
            return;
        } _Else{            // terminating else on accept statement (do not block)
            yield(parentalDelay);                        // yield before gift is transferred
            unsigned int money = prng(1, 3);             // [1,3]
            unsigned int studentId = prng(numStudents);  // [0,numStudents)
            giftsGiven += 1;
            bank.deposit(studentId, money);              // infinite money....
            // std::cout << "Parent deposited " << money << " to student " << studentId << std::endl;
            printer.print(Printer::Kind::Parent, 'D', studentId, money);  // finishing print
        }
    }
}