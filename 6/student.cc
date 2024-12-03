#include <iostream>

#include "student.h"
#include "printer.h"
#include "nameServer.h"
#include "WATcard.h"
#include "bottlingPlant.h"
#include "vendingMachine.h"

Student::Student(Printer& prt, NameServer& nameServer, WATCardOffice& cardOffice, Groupoff& groupoff,
    unsigned int id, unsigned int maxPurchases) : printer(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases) {
}

void Student::main() {

    BottlingPlant::Flavours favouriteFlavour = static_cast<BottlingPlant::Flavours>(prng(0, BottlingPlant::Flavours::NUM_OF_FLAVOURS)); // flavours are in the range [0, 3]
    unsigned int purchaseGoal = prng(1, maxPurchases);
    printer.print(Printer::Kind::Student, id, 'S', favouriteFlavour, purchaseGoal); // starting print

    WATCard::FWATCard giftCardFuture = groupoff.giftCard(id);       // request gift card, to be delivered later
    WATCard::FWATCard watCardFuture = cardOffice.create(id, 5);     // create a watcard with $5 (delivered later)

    VendingMachine* machine = nameServer.getMachine(id);  // get first vending machine
    printer.print(Printer::Kind::Student, id, 'V', machine->getId());

    unsigned int bottlesPurchased = 0;
    unsigned int freeSodas = 0;
    bool giftCardDeleted = false;

    // student needs to purchase max purchaseGoal bottles or quit before
    while (bottlesPurchased < purchaseGoal) {
        yield(prng(1, 10));  // yield for a random time between 1 and 10

        for (;;) {           // busy wait to buy soda
            try {
                _Select(giftCardFuture) {       // if giftcard is ready
                    machine->buy(favouriteFlavour, *giftCardFuture());
                    printer.print(Printer::Kind::Student, id, 'G', favouriteFlavour, giftCardFuture()->getBalance());

                    delete giftCardFuture();    // delete it before we reset since it's studnet responsibility
                    giftCardDeleted = true;

                    giftCardFuture.reset();    // reset gift card because we will never use it again
                    bottlesPurchased++;         // did get bought!
                    break;
                } or _Select(watCardFuture) {   // if watcard is ready
                    machine->buy(favouriteFlavour, *watCardFuture());
                    printer.print(Printer::Kind::Student, id, 'B', favouriteFlavour, watCardFuture()->getBalance());

                    bottlesPurchased++;
                    break;
                }   // _Select - a bottle has been purchased here

            } _Catch(VendingMachine::Funds&) {   // insufficient funds
                watCardFuture = cardOffice.transfer(id, machine->cost() + 5, watCardFuture());
            } _Catch(WATCardOffice::Lost&) {     // if card is lost
                printer.print(Printer::Kind::Student, id, 'L');
                watCardFuture = cardOffice.create(id, 5);  // create a new card with $5
            } _Catch(VendingMachine::Free&) {    // if vending machine gives a free soda
                freeSodas++;
                printer.print(Printer::Kind::Student, id, 'A', favouriteFlavour);
                if (prng(1, 2) == 1) {           // 50% chance of watching an ad
                    yield(4);
                }
                else {
                    printer.print(Printer::Kind::Student, id, 'X'); // did not watch ad
                }
            } _Catch(VendingMachine::Stock&) {          // if vending machine is out of stock
                machine = nameServer.getMachine(id);    // get a new vending machine
                printer.print(Printer::Kind::Student, id, 'V', machine->getId());
            }
        }
    }

    try {
        _Enable{ // there's a chance that watCard is lost so catch the exception (deleted by courier)
            /*
             * there is ALSO a chance that the watCARD hasn't been created yet
             * (the student used a giftcard and fulfilled the 1 bottle, so we exited before).
             *
             * so now we have to wait for the future watcard to deliver so we can delete
             * the watcard object - the student should destruct until the watcard is
             * deleted here
             */
            _Select(watCardFuture) {
                WATCard* watCard = watCardFuture();
                delete watCard;
            }
        };
    } _Catch(WATCardOffice::Lost&) {} // do nothing, but now we can quit

    if (!giftCardDeleted) {         // if gift card was not deleted
        _Select(giftCardFuture) {   // ensure gift card has resolved before deleting
            delete giftCardFuture();
        }
    }

    printer.print(Printer::Kind::Student, id, 'F', bottlesPurchased, freeSodas);
}