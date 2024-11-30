#ifndef _WAT_card_h_
#define _WAT_card_h_

class WATCard {
    WATCard(const WATCard&) = delete; // prevent copying
    WATCard(const WATCard&&) = delete;
    WATCard& operator=(const WATCard&) = delete;
    WATCard& operator=(const WATCard&&) = delete;

    unsigned int balance = 0;   // it has $0 balance on creation
public:
    typedef Future_ISM<WATCard*> FWATCard; // future watcard pointer
    WATCard();
    void deposit(unsigned int amount);
    void withdraw(unsigned int amount);
    unsigned int getBalance();
};


#endif // ifndef
