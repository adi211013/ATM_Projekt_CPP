//
// Created by adi on 11/15/25.
//

#include "../headers/ATM.h"

#include <fstream>
#include <ranges>
#include <sstream>
#include <iostream>

ATM ATM::pullATM() {
    ATM atm;
    std::ifstream file("atm.txt");
    if (file.is_open()) {
        file >> atm;
        file.close();
    } else
        atm.inventory = {{500, 0}, {200, 0}, {100, 0}, {50, 0}, {20, 0}, {10, 0}};
    return atm;
}

std::ostream &operator<<(std::ostream &file, const ATM &atm) {
    for (auto const &[nominal,count]: atm.inventory) {
        file << nominal << ";" << count << std::endl;
    }
    return file;
}

std::istream &operator>>(std::istream &file, ATM &atm) {
    atm.inventory.clear();
    std::string line;
    while (std::getline(file >> std::ws, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string sNominal, sCount;
        if (std::getline(ss, sNominal, ';') && std::getline(ss, sCount, ';')) {
            try {
                int nominal = std::stoi(sNominal);
                int count = std::stoi(sCount);
                atm.inventory[nominal] = count;
            } catch (...) {
                continue;
            }
        }
    }
    return file;
}

void ATM::pushATM(ATM &atm) {
    std::ofstream file("atm.txt");
    if (file.is_open()) {
        file << atm;
        file.close();
    }
}

bool ATM::canPayOut(int amount, std::map<int, int> &out) {
    out.clear();
    return solve(amount, inventory.rbegin(), inventory.rend(), out);
}

bool ATM::solve(int remaining, std::map<int, int>::reverse_iterator it, std::map<int, int>::reverse_iterator end,
                std::map<int, int> &out) {
    if (remaining == 0) return true;
    if (it == end) return false;
    int nominal = it->first;
    int count = it->second;
    int maxTake = std::min(remaining / nominal, count);
    for (int i = maxTake; i >= 0; --i) {
        out[nominal] = i;
        if (solve(remaining - (i * nominal), std::next(it), end, out)) {
            return true;
        }
    }
    out[nominal] = 0;
    return false;
}

void ATM::commitPayOut(const std::map<int, int> &out) {
    for (auto const &[nominal, count]: out) {
        inventory[nominal] -= count;
    }
    ATM::pushATM(*this);
}
