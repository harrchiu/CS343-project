#include "bank.h"

Bank::Bank(unsigned int numStudents) {
    bankAccounts = new unsigned int[numStudents];
    memset(bankAccounts, 0, numStudents * sizeof(*bankAccounts));  // init all balances to 0
}
Bank::~Bank() {
    delete[] bankAccounts;
}

// put the money in the bag
void Bank::deposit(unsigned int id, unsigned int amount) {
    bankAccounts[id] += amount;
}
void Bank::withdraw(unsigned int id, unsigned int amount) {
    while (amount > bankAccounts[id]) {
        // cannot withdraw if the balance is below amount
        // wait until deposit finishes with _Accept until checking again
        _Accept(deposit);
    }
    // enough
    bankAccounts[id] -= amount;
}