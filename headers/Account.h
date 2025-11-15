//
// Created by adi on 11/15/25.
//

#ifndef ACCOUNTS_H
#define ACCOUNTS_H
#include <vector>


class Account {
    int cardNumber;
    int pin;
    int balance;
    bool blocked;
    public:
    Account(int cardNumber, int pin,int balance,bool blocked);
    Account(int cardNumber, int pin);
    Account(int cardNumber);
    int getCardNumber() const;
    int getPin() const;
    int getBalance() const;
    bool getBlocked() const;
    static std::vector<Account> pullAccounts();
    void block();
    static void pushAccounts(const std::vector<Account> &accounts);
};



#endif //ACCOUNTS_H
