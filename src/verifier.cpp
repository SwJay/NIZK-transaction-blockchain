//
// Created by swj on 18-7-26.
//

#include "verifier.h"
#include "crypto_hash.h"
#include "DSC.h"
#include <cmath>

/* Cipher ****************************************************************/
_Cipher::_Cipher(pairing_t pairing) {
    element_init_G1(c[0], pairing);
    element_init_G1(c[1], pairing);
    element_init_G1(c[2], pairing);
}

_Cipher::~_Cipher() {
    element_clear(c[0]);
    element_clear(c[1]);
    element_clear(c[2]);
}

void _Cipher::set(element_s *_c1, element_s *_c2, element_s *_c3) {
    element_set(c[0], _c1);
    element_set(c[1], _c2);
    element_set(c[2], _c3);
}

/* Commitment ****************************************************************/
_Commitment::_Commitment(pairing_t pairing) {
    // init
    element_init_G1(R1, pairing);
    element_init_G1(R2, pairing);
    element_init_G1(_R1, pairing);
    element_init_G1(_R2, pairing);
    for (int j = 0; j < MAX_SPACE; j++) {
        element_init_G1(V[j], pairing);
        element_init_G1(_V[j], pairing);
        element_init_GT(a[j], pairing);
        element_init_GT(_a[j], pairing);
    }
    element_init_G1(D1, pairing);
    element_init_G1(D2, pairing);
    element_init_G1(alpha, pairing);
}

_Commitment::~_Commitment() {
    element_clear(R1);
    element_clear(R2);
    element_clear(_R1);
    element_clear(_R2);
    for (int j = 0; j < MAX_SPACE; j++) {
        element_clear(V[j]);
        element_clear(_V[j]);
        element_clear(a[j]);
        element_clear(_a[j]);
    }
    element_clear(D1);
    element_clear(D2);
    element_clear(alpha);
}

/* Response ****************************************************************/
_Response::_Response(pairing_s *pairing) {
    // init
    element_init_Zr(z1, pairing);
    element_init_Zr(z2, pairing);
    for (int j = 0; j < MAX_SPACE; j++) {
        element_init_Zr(zv[j], pairing);
        element_init_Zr(_zv[j], pairing);
        element_init_Zr(zt[j], pairing);
        element_init_Zr(_zt[j], pairing);
    }
    element_init_Zr(zl, pairing);
    element_init_Zr(zk, pairing);
    element_init_Zr(tmp_z, pairing);
}

_Response::~_Response() {
    element_clear(z1);
    element_clear(z2);
    for (int j = 0; j < MAX_SPACE; j++) {
        element_clear(zv[j]);
        element_clear(_zv[j]);
        element_clear(zt[j]);
        element_clear(_zt[j]);
    }
    element_clear(zl);
    element_clear(zk);
    element_clear(tmp_z);
}

_Proof::~_Proof() {
    delete commitment;
    delete response;
    element_clear(challenge);
}

bool Verifier::verify(DSC *dsc, Proof *proof, Account A, Account B, Cipher *C1, Cipher *C2, Cipher *C3) {
    element_t c0, c1;
    element_t tmp_add, tmp_mul, tmp_neg;
    element_t tmp_pow0, tmp_pow1;
    element_t tmp_D1, tmp_D2;
    element_t tmp_gt0, tmp_gt1;
    unsigned char *value = new unsigned char(crypto_hash_BYTES);
    int eq0, eq1, eq2, eq3, eq4, eq5;
    bool flag = true;

    element_init_Zr(c0, dsc->group->pairing);
    element_init_Zr(c1, dsc->group->pairing);
    element_init_Zr(tmp_add, dsc->group->pairing);
    element_init_Zr(tmp_mul, dsc->group->pairing);
    element_init_Zr(tmp_neg, dsc->group->pairing);
    element_init_G1(tmp_pow0, dsc->group->pairing);
    element_init_G1(tmp_pow1, dsc->group->pairing);
    element_init_G1(tmp_D1, dsc->group->pairing);
    element_init_G1(tmp_D2, dsc->group->pairing);
    element_init_GT(tmp_gt0, dsc->group->pairing);
    element_init_GT(tmp_gt1, dsc->group->pairing);

    // generate challenge
    dsc->randomOracle(value, proof->commitment);
    element_from_hash(c1, (void *) value, 64);
    element_sub(c0, proof->challenge, c1);

    // init tmp_D1
    element_add(tmp_add, proof->response->z1, proof->response->z2);
    element_pow2_zn(tmp_D1, C2->c[2], proof->challenge, dsc->group->g1, tmp_add);
    // init tmp_D2
    element_pow3_zn(tmp_D2, C1->c[2], proof->challenge, C1->c[0], proof->response->zl, C1->c[1], proof->response->zk);
    element_div(tmp_D2, tmp_D2, tmp_D1);

    // check
    element_pow2_zn(tmp_pow0, C2->c[0], proof->challenge, A.publicKey[0], proof->response->z1); // R1
    eq0 = element_cmp(tmp_pow0, proof->commitment->R1);
    element_pow2_zn(tmp_pow0, C2->c[1], proof->challenge, A.publicKey[1], proof->response->z2); // R2
    eq1 = element_cmp(tmp_pow0, proof->commitment->R2);
    element_pow2_zn(tmp_pow0, C3->c[0], proof->challenge, B.publicKey[0], proof->response->z1); // _R1
    eq2 = element_cmp(tmp_pow0, proof->commitment->_R1);
    element_pow2_zn(tmp_pow0, C3->c[1], proof->challenge, B.publicKey[1], proof->response->z2); // _R2
    eq3 = element_cmp(tmp_pow0, proof->commitment->_R2);
    if(eq0 || eq1 || eq2 || eq3)
        flag = false;
    for(int j = 0; j < MAX_SPACE && flag; j++){
        element_mul_si(tmp_mul, proof->response->zt[j], (long int)pow(RANGE, j)); // D1
        element_pow_zn(tmp_pow0, dsc->group->h, tmp_mul);
        element_mul(tmp_D1, tmp_pow0, tmp_D1);
        element_mul_si(tmp_mul, proof->response->_zt[j], (long int)pow(RANGE, j)); // D2
        element_pow_zn(tmp_pow0, dsc->group->h, tmp_mul);
        element_mul(tmp_D2, tmp_pow0, tmp_D2);

        element_pairing(tmp_gt0, proof->commitment->V[j], dsc->vk); // aj
        element_neg(tmp_neg, proof->response->zt[j]);
        element_pairing(tmp_gt1, proof->commitment->V[j], dsc->group->g2);
        element_pow3_zn(tmp_pow0, tmp_gt0, proof->challenge, tmp_gt1, tmp_neg, dsc->group->gt, proof->response->zv[j]);
        eq4 = element_cmp(tmp_pow0, proof->commitment->a[j]);
        element_pairing(tmp_gt0, proof->commitment->_V[j], dsc->vk); // _aj
        element_neg(tmp_neg, proof->response->_zt[j]);
        element_pairing(tmp_gt1, proof->commitment->_V[j], dsc->group->g2);
        element_pow3_zn(tmp_pow0, tmp_gt0, proof->challenge, tmp_gt1, tmp_neg, dsc->group->gt, proof->response->_zv[j]);
        eq5 = element_cmp(tmp_pow0, proof->commitment->_a[j]);

        if(eq4 || eq5)
            flag = false;
    }
    if(flag){
        eq0 = element_cmp(tmp_D1, proof->commitment->D1);
        eq1 = element_cmp(tmp_D2, proof->commitment->D2);
        element_pow_zn(tmp_pow0, dsc->group->h, c0);
        element_mul(tmp_pow0, proof->commitment->alpha, tmp_pow0);
        element_pow_zn(tmp_pow1, dsc->group->g1, proof->response->tmp_z);
        eq2 = element_cmp(tmp_pow0, tmp_pow1);
        if(eq0 || eq1 || eq2)
            flag = false;
    }

    // clear
    element_clear(c0);
    element_clear(c1);
    element_clear(tmp_add);
    element_clear(tmp_mul);
    element_clear(tmp_neg);
    element_clear(tmp_pow0);
    element_clear(tmp_pow1);
    element_clear(tmp_D1);
    element_clear(tmp_D2);
    element_clear(tmp_gt0);
    element_clear(tmp_gt1);

    return flag;
}
