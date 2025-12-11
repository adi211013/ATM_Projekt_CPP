//
// Created by adi on 11/15/25.
//

#include "../headers/ATM.h"
#include <fstream>

#include <fstream>
#include <iostream>
#include <sstream>

ATM ATM::pullATM() {
    ATM atm;
    std::ifstream file("atm.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string sNominal,sCount;
        if (std::getline(ss, sNominal, ',')&& !std::getline(ss, sCount, ',')) {
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
bool ATM::canPayOut(int amount, std::map<int,int>& out) {
    int remaining=amount;

    return true;
}
void ATM::commitPayOut(std::map<int,int>& out){}