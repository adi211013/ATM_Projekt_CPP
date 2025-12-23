//
// Created by adi on 12/23/25.
//

#pragma once
#include <string>

class TransactionLog {
private:
    int cardNumber;
    int amount;
    std::string status;

public:
    TransactionLog(int cardNumber, int amount, std::string status);

    friend std::ostream &operator<<(std::ostream &file, const TransactionLog &log);
};
