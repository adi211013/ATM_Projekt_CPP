//
// Created by adi on 11/15/25.
//

#ifndef ACCOUNTS_H
#define ACCOUNTS_H
#include <vector>
#include <QDate>

class Account {
    int cardNumber;
    int pin;
    int balance;
    bool blocked;
    int dailyWithdrawalLimit;
    int monthlyWithdrawalLimit;
    int cardWithdrawalLimit;
    int dailyWithdrawnAmount;
    int monthlyWithdrawnAmount;
    QDate lastWithdrawalDate;
    public:
    Account(int cardNumber, int pin, int balance,bool blocked,int dailyWithdrawalLimit,int monthlyWithdrawalLimit,int cardWithdrawalLimit,int dailyWithdrawnAmount,int monthlyWithdrawnAmount,QDate lastWithdrawalDate) ;
    Account();
    friend std::ostream& operator<<(std::ostream& file, const Account& acc);
    friend std::istream& operator>>(std::istream& file, Account& acc);
    int getCardNumber() const;
    int getPin() const;
    int getBalance() const;
    bool getBlocked() const;

    static std::vector<Account> pullAccounts();
    void block();
    static void pushAccounts(const std::vector<Account> &accounts);
    void checkAndResetLimits();
    bool canWithdraw(const int amount) const;
    void recordWithdrawal(int amount);
};



#endif //ACCOUNTS_H
