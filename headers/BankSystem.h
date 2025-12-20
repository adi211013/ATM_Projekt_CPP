//
// Created by adi on 12/19/25.
//

#pragma once
#include <vector>

#include "Account.h"
#include "ATM.h"
#include "LoginResult.h"
#include "WithdrawResult.h"


class BankSystem {
    std::vector<Account> accounts;
    ATM atm;
    Account* currentAccount;
    Account* pendingAccount;
    int failedAttempts;
    public:
    BankSystem();
    ~BankSystem();
    LoginResult enterCard(int cardNumber);
    LoginResult enterPin(int pin);
    int getFailedAttempts();
    WithdrawResult withdraw(int amount,std::map<int, int>& outNotes);
    int getBalance();

};



