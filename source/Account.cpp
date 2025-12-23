//
// Created by adi on 11/15/25.
//

#include "../headers/Account.h"
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <sstream>

Account::Account(int cardNumber, int pin, int balance,
                 bool blocked, int dailyWithdrawalLimit,
                 int monthlyWithdrawalLimit, int cardWithdrawalLimit,
                 int dailyWithdrawnAmount, int monthlyWithdrawnAmount,
                 QDate lastWithdrawalDate)
    : cardNumber(cardNumber), pin(pin),
      balance(balance), blocked(blocked),
      dailyWithdrawalLimit(dailyWithdrawalLimit), monthlyWithdrawalLimit(monthlyWithdrawalLimit)
      , cardWithdrawalLimit(cardWithdrawalLimit), dailyWithdrawnAmount(dailyWithdrawnAmount),
      monthlyWithdrawnAmount(monthlyWithdrawnAmount), lastWithdrawalDate(lastWithdrawalDate) {
}

Account::Account(): cardNumber(0), pin(0),
                    balance(0), blocked(0),
                    dailyWithdrawalLimit(0), monthlyWithdrawalLimit(0)
                    , cardWithdrawalLimit(0), dailyWithdrawnAmount(0),
                    monthlyWithdrawnAmount(0), lastWithdrawalDate(QDate::currentDate()) {
}

int Account::getCardNumber() const {
    return cardNumber;
}

int Account::getPin() const {
    return pin;
}

int Account::getBalance() const {
    return balance;
}

bool Account::getBlocked() const {
    return blocked;
}

void Account::block() {
    this->blocked = true;
}

std::istream &operator>>(std::istream &file, Account &acc) {
    std::string line;
    if (std::getline(file >> std::ws, line)) {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> data;
        while (std::getline(ss, segment, ';')) {
            data.push_back(segment);
        }
        if (data.size() >= 10) {
            try {
                acc.cardNumber = std::stoi(data[0]);
                acc.pin = std::stoi(data[1]);
                acc.balance = std::stoi(data[2]);
                acc.blocked = (std::stoi(data[3]) != 0);
                acc.dailyWithdrawalLimit = std::stoi(data[4]);
                acc.monthlyWithdrawalLimit = std::stoi(data[5]);
                acc.cardWithdrawalLimit = std::stoi(data[6]);
                acc.dailyWithdrawnAmount = std::stoi(data[7]);
                acc.monthlyWithdrawnAmount = std::stoi(data[8]);
                acc.lastWithdrawalDate = QDate::fromString(QString::fromStdString(data[9]), "dd-MM-yyyy");
            } catch (...) {
                file.setstate(std::ios::failbit);
            }
        }
    }
    return file;
}

std::vector<Account> Account::pullAccounts() {
    std::vector<Account> accounts;
    std::ifstream file("accounts.txt");
    if (!file.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku" << std::endl;
        return accounts;
    }
    Account temp;
    while (file >> temp) {
        accounts.push_back(temp);
    }
    file.close();
    return accounts;
}

std::ostream &operator<<(std::ostream &file, const Account &acc) {
    std::string sDate = acc.lastWithdrawalDate.toString("dd-MM-yyyy").toStdString();

    file << acc.cardNumber << ";"
            << acc.pin << ";"
            << acc.balance << ";"
            << (acc.blocked ? 1 : 0) << ";"
            << acc.dailyWithdrawalLimit << ";"
            << acc.monthlyWithdrawalLimit << ";"
            << acc.cardWithdrawalLimit << ";"
            << acc.dailyWithdrawnAmount << ";"
            << acc.monthlyWithdrawnAmount << ";"
            << sDate;
    return file;
}

void Account::pushAccounts(const std::vector<Account> &accounts) {
    std::ofstream file("accounts.txt");
    if (!file.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku" << std::endl;
        return;
    }
    for (const Account &acc: accounts) {
        file << acc << std::endl;
    }
    file.close();
}

void Account::checkAndResetLimits() {
    QDate now = QDate::currentDate();
    if (lastWithdrawalDate.month() != now.month() || (
            lastWithdrawalDate.year() != now.year() && lastWithdrawalDate.month() == now.month())) {
        monthlyWithdrawnAmount = 0;
    }
    if (lastWithdrawalDate != now) {
        dailyWithdrawnAmount = 0;
    }
}

WithdrawResult Account::canWithdraw(const int amount) const {
    if (amount > balance)
        return WithdrawResult::InsufficientFunds;
    if (amount > cardWithdrawalLimit)
        return WithdrawResult::CardLimitExceeded;
    if (dailyWithdrawnAmount + amount > dailyWithdrawalLimit)
        return WithdrawResult::DailyLimitExceeded;
    if (monthlyWithdrawnAmount + amount > monthlyWithdrawalLimit)
        return WithdrawResult::MonthlyLimitExceeded;
    if (amount % 10 != 0)
        return WithdrawResult::AmountNotDivisible;
    if (amount < 50)
        return WithdrawResult::AmountTooLow;
    return WithdrawResult::Success;
}

void Account::recordWithdrawal(int amount) {
    balance -= amount;
    dailyWithdrawnAmount += amount;
    monthlyWithdrawnAmount += amount;
    lastWithdrawalDate = QDate::currentDate();
}
