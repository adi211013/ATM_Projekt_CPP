//
// Created by adi on 12/19/25.
//

#include "../headers/BankSystem.h"

#include <fstream>

#include "../headers/TransactionLog.h"

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
std::vector<int> BankSystem::getCardsNumber() {
    std::vector<int> cardNumber;
    for (const auto &account : accounts) {
        cardNumber.push_back(account.getCardNumber());
    }
    return cardNumber;
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

WithdrawResult BankSystem::withdraw(int amount, std::map<int, int>& outNotes) {
    std::ofstream logFile("history.log", std::ios::app);
    if (currentAccount == nullptr) return WithdrawResult::AuthError;
    int cardNum = currentAccount->getCardNumber();
    WithdrawResult accStatus = currentAccount->canWithdraw(amount);
    if (accStatus != WithdrawResult::Success) {
        if (logFile.is_open()) {
            std::string msg = "FAIL: Nieznany blad";
            switch (accStatus) {
            case WithdrawResult::InsufficientFunds: msg = "FAIL: Brak srodkow"; break;
            case WithdrawResult::DailyLimitExceeded: msg = "FAIL: Limit dzienny"; break; 
            case WithdrawResult::MonthlyLimitExceeded: msg = "FAIL: Limit miesieczny"; break;
            case WithdrawResult::CardLimitExceeded: msg = "FAIL: Limit karty"; break;
            case WithdrawResult::AmountNotDivisible: msg = "FAIL: Kwota niepodzielna przez 10"; break;
            case WithdrawResult::AmountTooLow: msg = "FAIL: Kwota ponizej 50zl"; break;
            default: break;
            }
            logFile << TransactionLog(cardNum, amount, msg);
        }
        return accStatus;
    }
    if (!atm.canPayOut(amount, outNotes)) {
        if (logFile.is_open())
            logFile << TransactionLog(cardNum, amount, "FAIL: Brak banknotow w maszynie");
        return WithdrawResult::ATMError;
    }
    currentAccount->recordWithdrawal(amount);
    atm.commitPayOut(outNotes);
    if (logFile.is_open()) {
        logFile << TransactionLog(cardNum, amount, "SUCCESS: Wyplata zrealizowana");
    }
    return WithdrawResult::Success;
}

int BankSystem::getBalance() {
    if (currentAccount == nullptr) return 0;
    return currentAccount->getBalance();
}
