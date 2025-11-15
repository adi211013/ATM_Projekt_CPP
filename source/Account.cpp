//
// Created by adi on 11/15/25.
//

#include "../headers/Account.h"
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <sstream>

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
std::vector<Account> Account::pullAccounts()
    {
        std::string s_cardNumber,sPin,sBalance,sBlocked;
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
            if (std::getline(ss,s_cardNumber,';')&&std::getline(ss, sPin, ';') && std::getline(ss, sBalance,';')&&std::getline(ss, sBlocked)) {
                try {
                    int cardNumber=std::stoi(s_cardNumber);
                    int pin=std::stoi(sPin);
                    int balance=std::stoi(sBalance);
                    bool blocked=std::stoi(sBlocked);
                    accounts.push_back(Account(cardNumber, pin, balance,blocked));
                }
                catch(const std::exception& e) {
                    std::cerr<<"Wystapil blad: "<<e.what()<<std::endl;
                }
            }
        }
    file.close();
    return accounts;
    }
