//
// Created by adi on 11/15/25.
//

#include "../headers/Account.h"
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <sstream>
Account::Account(int cardNumber, int pin, int balance,bool blocked,int dailyWithdrawalLimit,int monthlyWithdrawalLimit,int cardWithdrawalLimit,int dailyWithdrawnAmount,int monthlyWithdrawnAmount, QDate lastWithdrawalDate ) {
    this ->cardNumber = cardNumber;
    this->pin = pin;
    this->balance = balance;
    this->blocked = blocked;
    this->dailyWithdrawalLimit = dailyWithdrawalLimit;
    this->monthlyWithdrawalLimit = monthlyWithdrawalLimit;
    this->dailyWithdrawnAmount = dailyWithdrawnAmount;
    this->monthlyWithdrawnAmount = monthlyWithdrawnAmount;
    this->cardWithdrawalLimit = cardWithdrawalLimit;
    this->lastWithdrawalDate = lastWithdrawalDate;

}
Account::Account(int cardNumber, int pin) {
    this->cardNumber = cardNumber;
    this->pin = pin;
}
Account::Account(int cardNumber) {
    this->cardNumber = cardNumber;
}
Account::Account(int cardNumber, int pin,int balance,bool blocked){
        this->cardNumber = cardNumber;
        this->pin = pin;
        this->balance = balance;
        this->blocked = blocked;
    }
int Account::getCardNumber() const{
    return cardNumber;
}
int Account::getPin() const{
    return pin;
}
int Account::getBalance() const{
    return balance;
}
bool Account::getBlocked() const {
    return blocked;
}
void Account::block() {
    this->blocked = true;
}

std::vector<Account> Account::pullAccounts()
    {
        std::string s_cardNumber,sPin,sBalance,sBlocked,sdailyWithdrawalLimit,smonthlyWithdrawalLimit,scardWithdrawalLimit,sdailyWithdrawnAmount,smonthlyWithdrawnAmount,slastWithdrawalDate;
        std::vector<Account> accounts;
        std::ifstream file("accounts.txt");
        if (!file.is_open()) {
            std::cerr<<"Nie mozna otworzyc pliku"<<std::endl;
            return accounts;
        }
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty())
                continue;
            std::stringstream ss(line);
            if (std::getline(ss,s_cardNumber,';')
                && std::getline(ss, sPin, ';')
                && std::getline(ss, sBalance,';')
                &&std::getline(ss, sBlocked,';')
                &&std::getline(ss, sdailyWithdrawalLimit,';')
                &&std::getline(ss, smonthlyWithdrawalLimit,';')
                &&std::getline(ss, scardWithdrawalLimit,';')
                &&std::getline(ss, sdailyWithdrawnAmount,';')
                &&std::getline(ss, smonthlyWithdrawnAmount,';')
                &&std::getline(ss, slastWithdrawalDate)
                ) {
                try {
                    int cardNumber=std::stoi(s_cardNumber);
                    int pin=std::stoi(sPin);
                    int balance=std::stoi(sBalance);
                    bool blocked=std::stoi(sBlocked);
                    int idailyWithdrawalLimit=std::stoi(sdailyWithdrawalLimit);
                    int imonthlyWithdrawalLimit=std::stoi(smonthlyWithdrawalLimit);
                    int icardWithdrawalLimit=std::stoi(scardWithdrawalLimit);
                    int idailyWithdrawnAmount=std::stoi(sdailyWithdrawnAmount);
                    int imonthlyWithdrawnAmount=std::stoi(smonthlyWithdrawnAmount);
                    QString qsslastWithdrawalDate=QString::fromStdString(slastWithdrawalDate);
                    QDate lastWithdrawalDate=QDate::fromString(qsslastWithdrawalDate,"dd-MM-yyyy");
                    accounts.push_back(Account(cardNumber, pin, balance,blocked,idailyWithdrawalLimit,imonthlyWithdrawalLimit,icardWithdrawalLimit,idailyWithdrawnAmount,imonthlyWithdrawnAmount,lastWithdrawalDate));
                }
                catch(const std::exception& e) {
                    std::cerr<<"Wystapil blad: "<<e.what()<<std::endl;
                }
            }
        }
    file.close();
    return accounts;
    }
 void Account::pushAccounts(const std::vector<Account> &accounts) {
    std::ofstream file("accounts.txt");
    if (!file.is_open()) {
        std::cerr<<"Nie mozna otworzyc pliku"<<std::endl;
        return;
    }
    for (const Account& acc:accounts) {
        QString qslastWithdrawalDate=acc.lastWithdrawalDate.toString("dd-MM-yyyy");
        std::string slastWithdrawalDate=qslastWithdrawalDate.toStdString();
        file<<acc.cardNumber<<";"<<acc.pin<<";"<<acc.balance<<";"<<acc.blocked<<";"<<acc.dailyWithdrawalLimit<<";"<<acc.monthlyWithdrawalLimit<<";"<<acc.cardWithdrawalLimit<<";"<<acc.dailyWithdrawnAmount<<";"<<acc.monthlyWithdrawnAmount<<";"<<slastWithdrawalDate<<std::endl;
    }
    file.close();
}
void Account::checkAndResetLimits() {
    QDate now = QDate::currentDate();
    if (lastWithdrawalDate.month()!=now.month() || (lastWithdrawalDate.year()!=now.year() && lastWithdrawalDate.month()==now.month())) {
        monthlyWithdrawnAmount=0;
    }
    if (lastWithdrawalDate!=now) {
        dailyWithdrawnAmount=0;
    }
}

bool Account::canWithdraw(const int amount) const {
    if (amount>balance)
        return false;
    if (amount>cardWithdrawalLimit)
        return false;
    if (dailyWithdrawnAmount+amount>dailyWithdrawalLimit)
        return false;
    if (monthlyWithdrawnAmount+amount>monthlyWithdrawalLimit)
        return false;
    if (amount%10!=0)
        return false;
    if (amount<50)
        return false;
    return true;
}
void Account::recordWithdrawal(int amount) {
    balance-=amount;
    dailyWithdrawnAmount+=amount;
    monthlyWithdrawnAmount+=amount;
    lastWithdrawalDate=QDate::currentDate();
}