#ifndef _bank_h_
#define _bank_h_

_Monitor Bank{
  unsigned int* bankAccounts;
  public:
    Bank(unsigned int numStudents);
    ~Bank();
    void deposit(unsigned int id, unsigned int amount);
    void withdraw(unsigned int id, unsigned int amount);
};

#endif // ifndef