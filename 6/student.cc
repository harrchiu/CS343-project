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
    printer.print(Printer::Kind::Student, id, 'S', favouriteFlavour, purchaseGoal);

    WATCard::FWATCard watCardFuture = cardOffice.create(id, 5);     // create a watcard with $5
    WATCard::FWATCard giftCardFuture = groupoff.giftCard(id);

    VendingMachine* machine = nameServer.getMachine(id);  // get first vending machine
    printer.print(Printer::Kind::Student, id, 'V', machine->getId());

    unsigned int bottlesPurchased = 0;
    unsigned int freeSodas = 0;

    // for (int q = 0;q < 10;q++) {
    while (bottlesPurchased < purchaseGoal) {
        std::cout << "s " << id << " purchased: " << bottlesPurchased << "out of " << purchaseGoal << std::endl;
        yield(prng(1, 10));  // yield for a random time between 1 and 10

        // for (int w = 0;w < 1;w++) {  // busy wait to buy soda
        for (;;) {  // busy wait to buy soda
            // std::cout << "inside s " << id << " purchased: " << bottlesPurchased << std::endl;
            try {
                _Select(giftCardFuture) {  // if giftcard is ready
                    // std::cout << "in giftcardfuture" << std::endl;
                    machine->buy(favouriteFlavour, *giftCardFuture());
                    std::cout << "resetting..." << std::endl;   // not getting here
                    printer.print(Printer::Kind::Student, id, 'G', favouriteFlavour, giftCardFuture()->getBalance());

                    giftCardFuture.reset();    // delete the gift card??

                    bottlesPurchased++;         // did get bought!
                    break;
                } or _Select(watCardFuture) {  // if watcard is ready
                    std::cout << "in watcardfuture" << std::endl;
                    machine->buy(favouriteFlavour, *watCardFuture());
                    printer.print(Printer::Kind::Student, id, 'B', favouriteFlavour, watCardFuture()->getBalance());
                    bottlesPurchased++;
                    break;
                }

            } _Catch(VendingMachine::Funds&) {   // insufficient funds
                std::cout << "inside funds error" << std::endl;
                watCardFuture = cardOffice.transfer(id, machine->cost() + 5, watCardFuture());
            } _Catch(WATCardOffice::Lost&) {  // if card is lost
                printer.print(Printer::Kind::Student, id, 'L');
                watCardFuture = cardOffice.create(id, 5);  // create a new card
            } _Catch(VendingMachine::Free&) {  // if vending machine gives a free soda
                freeSodas++;
                printer.print(Printer::Kind::Student, id, 'A', favouriteFlavour);
                if (prng(1, 2) == 1) {  // 50% chance of watching an ad
                    yield(4);
                }
                else {
                    printer.print(Printer::Kind::Student, id, 'X'); // did not watch ad
                }
            } _Catch(VendingMachine::Stock&) {  // if vending machine is out of stock
                machine = nameServer.getMachine(id);  // get a new vending machine
                printer.print(Printer::Kind::Student, id, 'V', machine->getId());
            }
        }
    }

    printer.print(Printer::Kind::Student, id, 'F', bottlesPurchased, freeSodas);
}