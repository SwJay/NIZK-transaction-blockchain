//
// Created by swj on 18-7-26.
//

#include "account.h"
#include <iostream>
#include <cmath>

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

/*
 * core func
 * TODO: ta encryption adjust
 * */
Proof Account::transfer(DSC *dsc, Account B, uint amount) {
    Cipher C1(dsc->group->pairing), C2(dsc->group->pairing), C3(dsc->group->pairing);
    Commitment commitment;


    /* generate cipher of t, ta */
    encrypt(dsc, B, amount, &C1, &C2, &C3);

    /* generate proof */



}

void Account::encrypt(DSC *dsc, Account B, uint amount, Cipher *C1, Cipher *C2, Cipher *C3 ) {
    element_t c1, c2, c3;
    element_t y1, y2;
    element_t t, ta;

    // init
    element_init_Zr(y1,dsc->group->pairing);
    element_init_Zr(y2,dsc->group->pairing);
    element_init_G1(c1,dsc->group->pairing);
    element_init_G1(c2,dsc->group->pairing);
    element_init_G1(c3,dsc->group->pairing);
    element_init_Zr(ta,dsc->group->pairing);
    element_init_Zr(t,dsc->group->pairing);

    //random y1, y2
    element_random(y1);
    element_random(y2);

    //convert t, ta
    element_set_si(t, amount);
    element_set_si(ta, balance);

    //cipher text of ta
    element_pow_zn(c1, publicKey[0], y1);
    element_pow_zn(c2, publicKey[1], y2);
    element_pow3_zn(c3, dsc->group->g1, y1, dsc->group->g1, y2, dsc->group->h, ta);
    C1->set(c1, c2, c3);

    //cipher text of t using pkA
    element_pow_zn(c1, publicKey[0], y1);
    element_pow_zn(c2, publicKey[1], y2);
    element_pow3_zn(c3, dsc->group->g1, y1, dsc->group->g1, y2, dsc->group->h, t);
    C2->set(c1, c2, c3);

    //cipher text of t using pkB
    element_pow_zn(c1, B.publicKey[0], y1);
    element_pow_zn(c2, B.publicKey[1], y2);
    element_pow3_zn(c3, dsc->group->g1, y1, dsc->group->g1, y2, dsc->group->h, t);
    C3->set(c1, c2, c3);
}

void Account::commit_redpond(DSC *dsc, Account B, uint amount, Cipher C1, Commitment *commiment, Response *response) {
    element_t r1, r2, l, k;
    element_t v[MAX_SPACE], _v[MAX_SPACE], s[MAX_SPACE],w[MAX_SPACE], q[MAX_SPACE], m[MAX_SPACE];
    element_t tmp_mul, tmp_inv;
    int tj[MAX_SPACE], _tj[MAX_SPACE], tmp_t0, tmp_t1;

    //init
    element_init_Zr(r1,dsc->group->pairing);
    element_init_Zr(r2,dsc->group->pairing);
    element_init_Zr(l,dsc->group->pairing);
    element_init_Zr(k,dsc->group->pairing);
    element_init_G1(commiment->R1,dsc->group->pairing);
    element_init_G1(commiment->R2,dsc->group->pairing);
    element_init_G1(commiment->_R1,dsc->group->pairing);
    element_init_G1(commiment->_R2,dsc->group->pairing);
    for(int j = 0; j < MAX_SPACE; j++){
        element_init_Zr(v[j],dsc->group->pairing);
        element_init_Zr(_v[j],dsc->group->pairing);
        element_init_Zr(s[j],dsc->group->pairing);
        element_init_Zr(w[j],dsc->group->pairing);
        element_init_Zr(q[j],dsc->group->pairing);
        element_init_Zr(m[j],dsc->group->pairing);
        element_init_G1(commiment->V[j],dsc->group->pairing);
        element_init_G1(commiment->_V[j],dsc->group->pairing);
        element_init_GT(commiment->a[j],dsc->group->pairing);
        element_init_GT(commiment->_a[j],dsc->group->pairing);
    }
    element_init_G1(commiment->D1,dsc->group->pairing);
    element_init_G1(commiment->D2,dsc->group->pairing);
    element_init_Zr(tmp_mul,dsc->group->pairing);
    element_init_Zr(tmp_inv,dsc->group->pairing);

    //random r1, r2, l, k
    element_random(r1);
    element_random(r2);
    element_random(l);
    element_random(k);

    // compute Ri
    element_pow_zn(commiment->R1, publicKey[0], r1);
    element_pow_zn(commiment->R2, publicKey[1], r2);
    element_pow_zn(commiment->_R1, B.publicKey[0], r1);
    element_pow_zn(commiment->_R2, B.publicKey[1], r2);

    // for each tj, compute Vj, aj, D1, D2
    tmp_t0 = amount;
    tmp_t1 = balance - amount;
    element_add(tmp_mul, r1, r2); // r1 + r2
    element_invert(tmp_inv, tmp_mul); // -r1 -r2
    element_pow_zn(commiment->D1, dsc->group->g1, tmp_mul);
    element_pow3_zn(commiment->D2, C1.c1, l,C1.c2, k, dsc->group->g1, tmp_inv);
    for(int j = 0; j < MAX_SPACE; j++){
        tj[j] = tmp_t0 % RANGE;
        _tj[j] = tmp_t1 % RANGE;
        tmp_t0 /= RANGE;
        tmp_t1 /= RANGE;

        element_random(v[j]);
        element_random(_v[j]);
        element_random(s[j]);
        element_random(w[j]);
        element_random(q[j]);
        element_random(m[j]);
        // tj _tj
        element_pow_zn(commiment->V[j], dsc->sigma[tj[j]], v[j]);
        element_pow_zn(commiment->_V[j], dsc->sigma[_tj[j]], _v[j]);
        // a
        element_mul(tmp_mul, s[j], v[j]);
        element_invert(tmp_inv, tmp_mul);
        element_pow2_zn(commiment->a[j], dsc->T[tj[j]], tmp_inv, dsc->group->gt, q[j]);
        //_a
        element_mul(tmp_mul, w[j], _v[j]);
        element_invert(tmp_inv, tmp_mul);
        element_pow2_zn(commiment->a[j], dsc->T[_tj[j]], tmp_inv, dsc->group->gt, m[j]);
        // D1
        element_mul_si(tmp_mul, s[j], (long int)pow(RANGE, j));
        element_pow_zn(tmp_mul, dsc->group->h, tmp_mul);
        element_mul(commiment->D1, tmp_mul, commiment->D1);
        // D2
        element_mul_si(tmp_mul, w[j], (long int)pow(RANGE, j));
        element_pow_zn(tmp_mul, dsc->group->h, tmp_mul);
        element_mul(commiment->D2, tmp_mul, commiment->D2);
    }
}

