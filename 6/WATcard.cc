#include "WATcard.h"

WATCard::WATCard() {

}

unsigned int WATCard::getBalance() {
    return balance;
}

void WATCard::deposit(unsigned int amount) {
    balance += amount;
}

void WATCard::withdraw(unsigned int amount) {
    // we ahve a getter to check, so no need to block until adequate balance
    balance -= amount;
}
