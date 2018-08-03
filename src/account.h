//
// Created by swj on 18-7-26.
//

#ifndef NIZK_ACCOUNT_H
#define NIZK_ACCOUNT_H

#include <pbc.h>
#include "biligroup.h"
#include "verifier.h"
#include "DSC.h"

class Account{
private:
    element_t secreteKey[2];
    uint balance;
public:
    element_t publicKey[2];

private:
    void encrypt(DSC *dsc, Account *B, uint amount, Cipher *C1, Cipher *C2, Cipher *C3, element_t y1, element_t y2);
    void commit_respond(DSC *dsc, Account *B, uint amount, Cipher *C1, Cipher *C2, Commitment *commitment, Response *response, element_t challenge, element_t y1, element_t y2);
public:
    Account(BiliGroup *group, const uint &init_balance);
    ~Account();

    Proof *transfer(DSC* dsc, Account* B, uint amount, Cipher *C1, Cipher *C2, Cipher *C3);
};

#endif //NIZK_ACCOUNT_H
