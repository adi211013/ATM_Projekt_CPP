//
// Created by adi on 12/23/25.
//

#include "../headers/TransactionLog.h"

#include <chrono>

TransactionLog::TransactionLog(int cardNumber, int amount, std::string status) : cardNumber(cardNumber), amount(amount),
    status(status) {
}

std::ostream &operator<<(std::ostream &file, const TransactionLog &log) {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    file << "\n[" << std::put_time(std::localtime(&now), "%d:%m:%Y %H:%M:%S") << "]"
            << "\n Karta: " << log.cardNumber
            << " | Kwota: " << log.amount << "PLN "
            << "| Status: " << log.status;
    return file;
}
