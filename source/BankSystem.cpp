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

LoginResult BankSystem::enterCard(int cardNumber) {
    pendingAccount = nullptr;
    failedAttempts = 0;
    auto it = std::ranges::find_if(accounts, [cardNumber](const Account &acc) {
        return acc.getCardNumber() == cardNumber;
    });
    if (it == accounts.end())
        return LoginResult::InvalidCardNumber;
    if (it->getBlocked())
        return LoginResult::Blocked;
    pendingAccount = &(*it);
    return LoginResult::Success;
}

LoginResult BankSystem::enterPin(int pin) {
    if (pendingAccount == nullptr) return LoginResult::AccountError;
    if (pendingAccount->getPin() == pin) {
        currentAccount = pendingAccount;
        pendingAccount = nullptr;
        currentAccount->checkAndResetLimits();
        return LoginResult::Success;
    }
    failedAttempts++;
    if (failedAttempts >= 3) {
        pendingAccount->block();
        Account::pushAccounts(accounts);
        pendingAccount = nullptr;
        return LoginResult::Blocked;
    }
    return LoginResult::InvalidPin;
}

int BankSystem::getFailedAttempts() {
    return failedAttempts;
}

WithdrawResult BankSystem::withdraw(int amount, std::map<int, int> &outNotes) {
    if (!currentAccount->canWithdraw(amount))
        return WithdrawResult::InvalidAmount;
    if (!atm.canPayOut(amount, outNotes))
        return WithdrawResult::ATMError;
    currentAccount->recordWithdrawal(amount);
    atm.commitPayOut(outNotes);
    return WithdrawResult::Success;
}

int BankSystem::getBalance() {
    if (currentAccount == nullptr) return 0;
    return currentAccount->getBalance();
}
