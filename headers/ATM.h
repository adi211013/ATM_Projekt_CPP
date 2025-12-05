//
// Created by adi on 11/15/25.
//

#ifndef ATM_H
#define ATM_H
#include <map>


class ATM {
    std::map<int,int> inventory;
    public:
    static ATM pullATM();
    static void pushATM(ATM a);
    bool canPayOut(int amount, std::map<int,int>& out);
    void commitPayOut(std::map<int,int>& out);
};



#endif //ATM_H
