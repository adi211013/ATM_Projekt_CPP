//
// Created by adi on 12/19/25.
//

#include "../headers/BankSystem.h"

BankSystem::BankSystem() {
    atm = ATM::pullATM();
    accounts = Account::pullAccounts();
    currentAccount = nullptr;
    pendingAccount = nullptr;
    failedAttempts = 0;
}

BankSystem::~BankSystem() {
    ATM::pushATM(atm);
    Account::pushAccounts(accounts);
}

int BankSystem::enterCard(int cardNumber) {
    pendingAccount = nullptr;
    failedAttempts = 0;
    auto it = std::ranges::find_if(accounts, [cardNumber](const Account &acc) {
        return acc.getCardNumber() == cardNumber;
    });
    if (it == accounts.end())
        return 1;
    if (it->getBlocked())
        return 2;
    pendingAccount = &(*it);
    return 0;
}

int BankSystem::enterPin(int pin) {
    if (pendingAccount == nullptr) return 4;
    if (pendingAccount->getPin() == pin) {
        currentAccount = pendingAccount;
        pendingAccount = nullptr;
        currentAccount->checkAndResetLimits();
        return 0;
    }
    failedAttempts++;
    if (failedAttempts >= 3) {
        pendingAccount->block();
        Account::pushAccounts(accounts);
        pendingAccount = nullptr;
        return 2;
    }
    return 1;
}

int BankSystem::getFailedAttempts() {
    return failedAttempts;
}

int BankSystem::withdraw(int amount, std::map<int, int> &outNotes) {
    if (!currentAccount->canWithdraw(amount))
        return 1;
    if (!atm.canPayOut(amount, outNotes))
        return 2;
    currentAccount->recordWithdrawal(amount);
    atm.commitPayOut(outNotes);
    return 0;
}

int BankSystem::getBalance() {
    if (currentAccount == nullptr) return 0;
    return currentAccount->getBalance();
}
