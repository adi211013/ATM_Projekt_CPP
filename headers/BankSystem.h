//
// Created by adi on 12/19/25.
//

#pragma once
#include <vector>

#include "Account.h"
#include "ATM.h"


class BankSystem {
    std::vector<Account> accounts;
    ATM atm;
    Account* currentAccount;
    Account* pendingAccount;
    int failedAttempts;
    public:
    BankSystem();
    ~BankSystem();
    int enterCard(int cardNumber);
    int enterPin(int pin);
    int getFailedAttempts();
    int withdraw(int amount,std::map<int, int>& outNotes);
    int getBalance();

};



