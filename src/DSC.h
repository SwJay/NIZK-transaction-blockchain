//
// Created by swj on 18-7-25.
//

#ifndef NIZK_DSC_H
#define NIZK_DSC_H

#include <pbc.h>
#include "biligroup.h"
/*
class Account{
private:
    mpz_class secretKey[2];
    mpz_class publicKey[2];
public:
    Account();
    ~Account() = default;

    inline mpz_class* getPublicKey(){return publicKey;}

};
*/
class DSC {
private:
    BiliGroup *group;
    element_t sigma[1024], T[1024];
public:
    DSC(uint n);
    ~DSC(){delete group;}
};

#endif