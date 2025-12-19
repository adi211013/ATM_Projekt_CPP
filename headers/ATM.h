//
// Created by adi on 11/15/25.
//

#pragma once

#include <map>
#include <iostream>

class ATM {
    std::map<int,int> inventory;
    public:
    static ATM pullATM();
    static void pushATM(ATM &atm);
    bool canPayOut(int amount, std::map<int,int>& out);
    bool solve(int remaining, std::map<int, int>::reverse_iterator it, std::map<int, int>::reverse_iterator end, std::map<int, int>& out);
    void commitPayOut(const std::map<int, int> &out);
    friend std::ostream& operator<<(std::ostream& file, const ATM& atm);
    friend std::istream& operator>>(std::istream& file, ATM& atm);
};



