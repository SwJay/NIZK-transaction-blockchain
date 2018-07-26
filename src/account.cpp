//
// Created by swj on 18-7-26.
//

#include "account.h"
#include <iostream>

using namespace std;

/* Account Ctor
 * generate sk, pk
 */
Account::Account(BiliGroup *group, const uint &init_balance) {
    element_init_Zr(secreteKey[0], group->pairing);
    element_init_Zr(secreteKey[1], group->pairing);
    element_init_G1(publicKey[0], group->pairing);
    element_init_G1(publicKey[1], group->pairing);

    element_random(secreteKey[0]);
    element_random(secreteKey[1]);
    element_pow_zn(publicKey[0], secreteKey[0], group->g1);
    element_pow_zn(publicKey[1], secreteKey[1], group->g1);
    balance = init_balance;
}

Account::~Account() {
    element_clear(secreteKey[0]);
    element_clear(secreteKey[1]);
    element_clear(publicKey[0]);
    element_clear(publicKey[1]);
}

Proof Account::transfer(Account B,const uint &amount) {
    Proof proof;
    if(balance < amount)
        cout << "no sufficient balance!" << endl;
    else{
        balance -= amount;
        B.addAmount(amount);
    }
    return proof;
}
