#include "groupoff.h"
#include "printer.h"
#include "WATcard.h"

Groupoff::Groupoff(Printer& prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay) :
    printer(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {

    cards = new WATCard::FWATCard[numStudents];     // deleted by the students?
    printer.print(Printer::Kind::Groupoff, 'S');  // starting print
}

void Groupoff::main() {
    // begins by first accepting a call from all students to obtain a future giftCard
    while (giftCardsMade < numStudents) {
        _Accept(giftCard) {
            giftCardsMade += 1;
        }
    }

    // we need to randomly select a future card
    // since we iterate numStudents time, just shuffle the array of ids
    // and fulfill the gift card request in that order (order doesn't matter?)
    unsigned int replacementOrder[numStudents];
    for (unsigned int i = 0;i < numStudents;i++) {   // fill the array incrementing
        replacementOrder[i] = i;
    }
    // now randomly swap
    for (unsigned int counter = 0; counter < numStudents; counter += 1) {
        // swap(replacementOrder[0], replacementOrder[prng(numStudents)]);
        // i mean this supposedly produces a more uniform shuffle than the
        // one used in the prev assn so idk
        std::swap(replacementOrder[counter], replacementOrder[prng(numStudents - 1 - counter)]);
    }

    // stop looping after giving out all gift cards
    for (unsigned int i = 0;i < numStudents;i++) {
        _Accept(~Groupoff) {    // wait for dtor
            break;              // gift cards should be destroyed by students
        } _Else{   // yield must not block dtor call
            yield(groupoffDelay);
            unsigned int studentId = replacementOrder[i];   // get the random sid
            // make new wat card and fulfill the promise
            WATCard* watCard = new WATCard();
            watCard->deposit(sodaCost);         // free money
            cards[studentId].delivery(watCard);

            printer.print(Printer::Kind::Groupoff, 'D', studentId, sodaCost);
        }
    }

    printer.print(Printer::Kind::Groupoff, 'F');  // finishing print
}


// student calls to obtain a (future) gift card
WATCard::FWATCard Groupoff::giftCard(unsigned int id) {
    // just return the future
    return cards[id];
}
