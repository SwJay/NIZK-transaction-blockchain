//
// Created by swj on 18-7-26.
//

#ifndef NIZK_ACCOUNT_H
#define NIZK_ACCOUNT_H

#include <pbc.h>
#include "biligroup.h"
#include "verifier.h"
#include "DSC.h"

/*
 * TODO: decryption of HE scheme in paper may fail to accomplish with pbc
 * */

class Account{
private:
    element_t secreteKey[2];
public:
    element_t publicKey[2];
    Cipher *cipherBalance;
    uint balance;

private:
    void encrypt(DSC *dsc, Account *B, uint amount, Cipher *C2, Cipher *C3, element_t y1, element_t y2);
    void commit_respond(DSC *dsc, Account *B, uint amount, Commitment *commitment, Response *response, element_t challenge, element_t y1, element_t y2);
public:
    Account(DSC *dsc, const int &init_balance);
    ~Account();

    Proof *transfer(DSC* dsc, Account* B, const int &amount,  Cipher *C2, Cipher *C3);
    uint getBalance(BiliGroup *biligroup);
};

#endif //NIZK_ACCOUNT_H
