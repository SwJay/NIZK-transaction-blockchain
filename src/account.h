//
// Created by swj on 18-7-26.
//

#ifndef NIZK_ACCOUNT_H
#define NIZK_ACCOUNT_H

#include <pbc.h>
#include "biligroup.h"

typedef struct _Proof{

}Proof;

class Account{
private:
    element_t secreteKey[2];
    uint balance;
public:
    element_t publicKey[2];
public:
    Account(BiliGroup *group, const uint &init_balance);
    ~Account();

    inline void addAmount(const uint &amount){balance += amount;}
    Proof transfer(Account B, const uint &amount);
};

#endif //NIZK_ACCOUNT_H
