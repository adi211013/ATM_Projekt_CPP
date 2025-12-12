//
// Created by adi on 11/15/25.
//

#include "../headers/ATM.h"
#include <fstream>

#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>

ATM ATM::pullATM() {
    ATM atm;
    std::ifstream file("atm.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        atm.inventory.clear();
        return atm;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string sNominal,sCount;
        if (std::getline(ss, sNominal, ';')&& std::getline(ss, sCount, ';')) {
            int nominal = std::stoi(sNominal);
            int count = std::stoi(sCount);
            atm.inventory[nominal]=count;
        }
    }
    file.close();
    return atm;
}
void ATM::pushATM(ATM &atm) {
    std::ofstream file("atm.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }
    for (auto const&[nominal,count]: atm.inventory) {
        file<<nominal<<";"<<count<<std::endl;
    }
    file.close();

}
bool ATM::canPayOut(int amount, std::map<int, int>& out) {
    out.clear();
    int remaining = amount;
    for (auto &[fst, snd] : std::ranges::reverse_view(inventory)) {
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

void ATM::commitPayOut(const std::map<int, int>& out) {
    for (auto const& [nominal, count] : out) {
        inventory[nominal] -= count;
    }
    ATM::pushATM(*this);
}