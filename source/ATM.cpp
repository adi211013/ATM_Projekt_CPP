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
    int remaining = amount;
    for (auto &[fst, snd]: std::ranges::reverse_view(inventory)) {
        int nominal = fst;
        int count = snd;
        if (remaining == 0) break;
        if (count == 0) continue;
        if (nominal > remaining) continue;
        int needed = remaining / nominal;
        int toGive = std::min(needed, count);
        out[nominal] = toGive;
        remaining -= (toGive * nominal);
    }
    return (remaining == 0);
}

void ATM::commitPayOut(const std::map<int, int> &out) {
    for (auto const &[nominal, count]: out) {
        inventory[nominal] -= count;
    }
    ATM::pushATM(*this);
}
