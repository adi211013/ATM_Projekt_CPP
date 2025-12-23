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
    std::ofstream logFile("history.log", std::ios::app);
    if (currentAccount == nullptr)
        return WithdrawResult::AuthError;

    if (!atm.canPayOut(amount, outNotes)) {
        if (logFile.is_open())
            logFile << TransactionLog(currentAccount->getCardNumber(), amount, "FAIL: brak banknotow");
        logFile.close();
        return WithdrawResult::ATMError;
    }
	WithdrawResult canWithdrawResult = currentAccount->canWithdraw(amount);
    switch (canWithdrawResult)
    {   
    case WithdrawResult::Success:
         currentAccount->recordWithdrawal(amount);
        atm.commitPayOut(outNotes);
        if (logFile.is_open()) {
            logFile << TransactionLog(currentAccount->getCardNumber(), amount, "SUCCESS: Wyplata zrealizowana");
            logFile.close();
        }
        return WithdrawResult::Success;
        break;
    case WithdrawResult::InsufficientFunds:
        if (logFile.is_open()) {
            logFile << TransactionLog(currentAccount->getCardNumber(), amount, "FAIL: Nie ma wystarczajacych srodkow na koncie");
            logFile.close();
        }
        return WithdrawResult::InsufficientFunds;
        break;
    case WithdrawResult::DailyLimitExceeded:
        if (logFile.is_open()) {
            logFile << TransactionLog(currentAccount->getCardNumber(), amount, "FAIL: Przekroczony dzienny limit wyplat");
            logFile.close();
        }
        return WithdrawResult::DailyLimitExceeded;
        break;
    case WithdrawResult::MonthlyLimitExceeded:
        if (logFile.is_open()) {
            logFile << TransactionLog(currentAccount->getCardNumber(), amount, "FAIL: Przekroczony miesieczny limit wyplat");
            logFile.close();
        }
        return WithdrawResult::MonthlyLimitExceeded;
        break;
    case WithdrawResult::CardLimitExceeded:
        if (logFile.is_open()) {
            logFile << TransactionLog(currentAccount->getCardNumber(), amount, "FAIL: Przekroczono limit jednorazowej wyplaty karty");
            logFile.close();
        }
        return WithdrawResult::CardLimitExceeded;
        break;
    case WithdrawResult::AmountNotDivisible:
        if (logFile.is_open()) {
            logFile << TransactionLog(currentAccount->getCardNumber(), amount, "FAIL: Nie mozna wyplacic kwoty nie dzielacej sie przez 10");
            logFile.close();
        }
        return WithdrawResult::AmountNotDivisible;
        break;
    case WithdrawResult::AmountTooLow:
        if (logFile.is_open()) {
            logFile << TransactionLog(currentAccount->getCardNumber(), amount, "FAIL: Nie mozna wyplacic kwoty mniejszej od 50zl");
            logFile.close();
        }
        return WithdrawResult::AmountTooLow;
        break;
    default:
        return WithdrawResult::AuthError;
        break;
    }
}

int BankSystem::getBalance() {
    if (currentAccount == nullptr) return 0;
    return currentAccount->getBalance();
}
