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
Account::Account(DSC *dsc, const int &init_balance) {
    element_t c1, c2, c3;
    element_t ta;
    element_t y1, y2;

    if(init_balance < 0)
        throw "ERROR: Initial balance is set negative.";

    cipherBalance = new Cipher(dsc->group->pairing);
    balance = init_balance;

    element_init_Zr(secreteKey[0], dsc->group->pairing);
    element_init_Zr(secreteKey[1], dsc->group->pairing);
    element_init_G1(publicKey[0], dsc->group->pairing);
    element_init_G1(publicKey[1], dsc->group->pairing);
    element_init_G1(c1,dsc->group->pairing);
    element_init_G1(c2,dsc->group->pairing);
    element_init_G1(c3,dsc->group->pairing);
    element_init_Zr(ta,dsc->group->pairing);
    element_init_Zr(y1,dsc->group->pairing);
    element_init_Zr(y2,dsc->group->pairing);

    element_random(secreteKey[0]);
    element_random(secreteKey[1]);
    element_pow_zn(publicKey[0], dsc->group->g1, secreteKey[0]);
    element_pow_zn(publicKey[1], dsc->group->g1, secreteKey[1]);

    //random y1, y2
    element_random(y1);
    element_random(y2);

    element_set_si(ta, init_balance);
    element_pow_zn(c1, publicKey[0], y1);
    element_pow_zn(c2, publicKey[1], y2);
    element_pow3_zn(c3, dsc->group->g1, y1, dsc->group->g1, y2, dsc->group->h, ta);
    cipherBalance->set(c1, c2, c3);

    element_clear(c1);
    element_clear(c2);
    element_clear(c3);
    element_clear(ta);
    element_clear(y1);
    element_clear(y2);
}

Account::~Account() {
    delete cipherBalance;
    element_clear(secreteKey[0]);
    element_clear(secreteKey[1]);
    element_clear(publicKey[0]);
    element_clear(publicKey[1]);
}

/*
 * core func
 * TODO: ta encryption adjust
 * */
Proof *Account::transfer(DSC *dsc, Account *B, const int &amount, Cipher *C2, Cipher *C3){
    element_t y1, y2;
    element_t challenge;
    auto *commitment = new Commitment(dsc->group->pairing);
    auto *response = new Response(dsc->group->pairing);

    if(amount < 0)
        throw "ERROR: transaction amount is set negative.";

    // init randomness
    element_init_Zr(y1,dsc->group->pairing);
    element_init_Zr(y2,dsc->group->pairing);
    element_init_Zr(challenge,dsc->group->pairing);

    /* generate cipher of t, ta */
    encrypt(dsc, B, amount, C2, C3, y1, y2);

    /* generate proof */
    commit_respond(dsc, B, amount, commitment, response, challenge, y1, y2);
    auto *proof = new Proof(commitment, response, challenge, dsc->group->pairing);

    element_clear(y1);
    element_clear(y2);
    element_clear(challenge);

    return proof;
}

void Account::encrypt(DSC *dsc, Account *B, uint amount, Cipher *C2, Cipher *C3, element_t y1, element_t y2) {
    element_t c1, c2, c3;
    element_t t, ta;

    // init
    element_init_G1(c1,dsc->group->pairing);
    element_init_G1(c2,dsc->group->pairing);
    element_init_G1(c3,dsc->group->pairing);
    element_init_Zr(ta,dsc->group->pairing);
    element_init_Zr(t,dsc->group->pairing);

    //random y1, y2
    element_random(y1);
    element_random(y2);

    //convert t
    element_set_si(t, amount);

    //cipher text of t using pkA
    element_pow_zn(c1, publicKey[0], y1);
    element_pow_zn(c2, publicKey[1], y2);
    element_pow3_zn(c3, dsc->group->g1, y1, dsc->group->g1, y2, dsc->group->h, t);
    C2->set(c1, c2, c3);

    //cipher text of t using pkB
    element_pow_zn(c1, B->publicKey[0], y1);
    element_pow_zn(c2, B->publicKey[1], y2);
    element_pow3_zn(c3, dsc->group->g1, y1, dsc->group->g1, y2, dsc->group->h, t);
    C3->set(c1, c2, c3);

    // clear
    element_clear(c1);
    element_clear(c2);
    element_clear(c3);
    element_clear(t);
    element_clear(ta);
}

void Account::commit_respond(DSC *dsc, Account *B, uint amount, Commitment *commitment, Response *response, element_t challenge, element_t y1, element_t y2) {
    element_t r1, r2, l, k;
    element_t v[MAX_SPACE], _v[MAX_SPACE], s[MAX_SPACE],w[MAX_SPACE], q[MAX_SPACE], m[MAX_SPACE];
    element_t tmp_mul, tmp_neg, tmp_add, tmp_inv;
    element_t tmp_pow0, tmp_pow1;
    int tj[MAX_SPACE], _tj[MAX_SPACE];
    uint tmp_t0, tmp_t1;
    element_t tmp_c0, tmp_c1;
    unsigned char *value = (unsigned char*)pbc_malloc(crypto_hash_BYTES);

    //init
    element_init_Zr(r1,dsc->group->pairing);
    element_init_Zr(r2,dsc->group->pairing);
    element_init_Zr(l,dsc->group->pairing);
    element_init_Zr(k,dsc->group->pairing);
    for(int j = 0; j < MAX_SPACE; j++){
        element_init_Zr(v[j],dsc->group->pairing);
        element_init_Zr(_v[j],dsc->group->pairing);
        element_init_Zr(s[j],dsc->group->pairing);
        element_init_Zr(w[j],dsc->group->pairing);
        element_init_Zr(q[j],dsc->group->pairing);
        element_init_Zr(m[j],dsc->group->pairing);
    }
    element_init_Zr(tmp_mul,dsc->group->pairing);
    element_init_Zr(tmp_neg,dsc->group->pairing);
    element_init_Zr(tmp_add,dsc->group->pairing);
    element_init_Zr(tmp_inv,dsc->group->pairing);
    element_init_G1(tmp_pow0,dsc->group->pairing);
    element_init_G1(tmp_pow1,dsc->group->pairing);
    element_init_Zr(tmp_c0,dsc->group->pairing);
    element_init_Zr(tmp_c1,dsc->group->pairing);

    //random r1, r2, l, k, tmp_c0, z
    element_random(r1);
    element_random(r2);
    element_random(l);
    element_random(k);
    element_random(tmp_c0);
    element_random(response->tmp_z);

    /* commit ******************************************/
    // compute Ri
    element_pow_zn(commitment->R1, publicKey[0], r1);
    element_pow_zn(commitment->R2, publicKey[1], r2);
    element_pow_zn(commitment->_R1, B->publicKey[0], r1);
    element_pow_zn(commitment->_R2, B->publicKey[1], r2);

    // for each tj, compute Vj, aj, D1, D2
    tmp_t0 = amount;
    tmp_t1 = getBalance(dsc->group) - amount;
    element_add(tmp_add, r1, r2); // r1 + r2
    element_neg(tmp_neg, tmp_add); // -r1 -r2
    element_pow_zn(commitment->D1, dsc->group->g1, tmp_add); // D1
    element_pow3_zn(commitment->D2, cipherBalance->c[0], l, cipherBalance->c[1], k, dsc->group->g1, tmp_neg); // D2

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
        element_pow_zn(commitment->V[j], dsc->sigma[tj[j]], v[j]);
        element_pow_zn(commitment->_V[j], dsc->sigma[_tj[j]], _v[j]);
        // a
        element_mul(tmp_mul, s[j], v[j]);
        element_neg(tmp_neg, tmp_mul);
        element_pow2_zn(commitment->a[j], dsc->T[tj[j]], tmp_neg, dsc->group->gt, q[j]);
        //_a
        element_mul(tmp_mul, w[j], _v[j]);
        element_neg(tmp_neg, tmp_mul);
        element_pow2_zn(commitment->a[j], dsc->T[_tj[j]], tmp_neg, dsc->group->gt, m[j]);
        // D1
        element_mul_si(tmp_mul, s[j], (long int)pow(RANGE, j));
        element_pow_zn(tmp_pow0, dsc->group->h, tmp_mul);
        element_mul(commitment->D1, tmp_pow0, commitment->D1);
        // D2
        element_mul_si(tmp_mul, w[j], (long int)pow(RANGE, j));
        element_pow_zn(tmp_pow1, dsc->group->h, tmp_mul);
        element_mul(commitment->D2, tmp_pow1, commitment->D2);
    }

    // compute alpha
    element_neg(tmp_neg, tmp_c0);
    element_pow2_zn(commitment->alpha, dsc->group->g1, response->tmp_z, dsc->group->h, tmp_neg);

    /* challenge ******************************************/
    dsc->randomOracle(value, commitment);
    element_from_hash(tmp_c1, (void *) value, crypto_hash_BYTES);
    element_add(challenge, tmp_c0, tmp_c1);

    /* respond ******************************************/
    element_mul(tmp_mul, challenge, y1); // z1
    element_sub(response->z1, r1, tmp_mul);
    element_mul(tmp_mul, challenge, y2); // z2
    element_sub(response->z2, r2, tmp_mul);

    for(int j = 0; j < MAX_SPACE; j++) {
        element_mul(tmp_mul, challenge, v[j]); // zv[j]
        element_sub(response->zv[j], q[j], tmp_mul);
        element_mul(tmp_mul, challenge, _v[j]); // _zv[j]
        element_sub(response->_zv[j], m[j], tmp_mul);
        element_mul_si(tmp_mul, challenge, tj[j]); // zt[j]
        element_sub(response->zt[j], s[j], tmp_mul);
        element_mul_si(tmp_mul, challenge, _tj[j]); // _zt[j]
        element_sub(response->_zt[j], w[j], tmp_mul);
    }
    element_div(tmp_mul, challenge, secreteKey[0]); // zl
    element_sub(response->zl, l, tmp_mul);
/*  element_invert(tmp_inv, secreteKey[0]);
    element_mul(tmp_mul, challenge, tmp_inv);
    element_sub(tmp_add, l, tmp_mul);
    int check = element_cmp(tmp_add, response->zl);*/

    element_div(tmp_mul, challenge, secreteKey[1]); // zk
    element_sub(response->zk, k, tmp_mul);

    // clear
    element_clear(r1);
    element_clear(r2);
    element_clear(l);
    element_clear(k);
    for(int j = 0; j < MAX_SPACE; j++){
        element_clear(v[j]);
        element_clear(_v[j]);
        element_clear(s[j]);
        element_clear(w[j]);
        element_clear(q[j]);
        element_clear(m[j]);
    }
    element_clear(tmp_pow1);
    element_clear(tmp_pow0);
    element_clear(tmp_mul);
    element_clear(tmp_neg);
    element_clear(tmp_add);
    element_clear(tmp_inv);
    element_clear(tmp_c0);
    element_clear(tmp_c1);
}

uint Account::getBalance(BiliGroup *biligroup) {
    element_t tmp_inv0, tmp_inv1;
    element_t result_t, result_z;
    element_t verify_t, verify_z;
    long result_i;

    element_init_Zr(tmp_inv0, biligroup->pairing);
    element_init_Zr(tmp_inv1, biligroup->pairing);
    element_init_G1(result_t, biligroup->pairing);
    element_init_Zr(result_z, biligroup->pairing);
    element_init_G1(verify_t, biligroup->pairing);
    element_init_Zr(verify_z, biligroup->pairing);

    element_invert(tmp_inv0, secreteKey[0]);
    element_invert(tmp_inv1, secreteKey[1]);
    element_pow2_zn(result_t, cipherBalance->c[0], tmp_inv0, cipherBalance->c[1], tmp_inv1);
    element_div(result_t, cipherBalance->c[2], result_t);

    element_set_si(verify_z, balance);
    element_pow_zn(verify_t, biligroup->h, verify_z);

    if(!element_cmp(verify_t, result_t))
        result_i = balance;
    else if(element_is1(result_t))
        result_i = 0;
    else{
        element_dlog_brute_force(result_z, biligroup->h, result_t);
        result_i = element_to_si(result_z);
    }

    balance = (uint)result_i;

    element_clear(tmp_inv0);
    element_clear(tmp_inv1);
    element_clear(result_t);
    element_clear(result_z);

    return (uint)result_i;
}

