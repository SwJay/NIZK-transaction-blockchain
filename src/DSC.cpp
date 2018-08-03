//
// Created by swj on 18-7-25.
//

#include "DSC.h"

/* DSC Ctor
 * Setup: generate bilinear group & generate public parameter
 */
DSC::DSC() {
    // variable declaration
    element_t lambda, tmp1, tmp2, inv_tmp; // lambda is the signkey for Boneh-Boyen signature
    element_pp_t g1_pp;

    // initialization
    group = new BiliGroup; // set bilinear group
    for(int i = 0; i < RANGE; i++){
        element_init_G1(sigma[i], group->pairing); // sigma init
        element_init_GT(T[i], group->pairing); // T init
    }
    element_init_Zr(lambda, group->pairing);
    element_init_Zr(tmp1, group->pairing);
    element_init_Zr(tmp2, group->pairing);
    element_init_Zr(inv_tmp, group->pairing);
    element_pp_init(g1_pp, group->g1); // g1_pp init as g1
    element_init_G2(vk, group->pairing); // vk init

    element_random(lambda);
    element_pow_zn(vk, group->g2, lambda); // get vk

    // compute BB-sig for 0~1023 in advance
    for (int i = 0; i < RANGE; i++) {
        element_set_si(tmp1, i);
        element_add(tmp2, lambda, tmp1);
        element_invert(inv_tmp, tmp2);
        element_pp_pow_zn(sigma[i], inv_tmp, g1_pp);
        element_pairing(T[i], sigma[i], group->g2);
    }

    // clear
    element_clear(lambda);
    element_clear(tmp1);
    element_clear(tmp2);
    element_clear(inv_tmp);
    element_pp_clear(g1_pp);
}

DSC::~DSC() {
    element_clear(vk);
    for(int i = 0; i < RANGE; i++){
        element_clear(sigma[i]);
        element_clear(T[i]);
    }
    delete group;
}

void DSC::randomOracle(unsigned char *value, Commitment *message) {
    element_t result, result0, result1, result_t0, result_t1;
    int length;
    element_init_G1(result,group->pairing);
    element_init_G1(result0,group->pairing);
    element_init_G1(result1,group->pairing);
    element_init_GT(result_t0,group->pairing);
    element_init_GT(result_t1,group->pairing);

    element_mul(result0, message->R1, message->R2);
    element_mul(result1, message->_R1, message->_R2);
    element_mul(result, result0, result1);
    element_set(result_t0, group->gt);
    for(int j =0; j < MAX_SPACE; j++){
        element_mul(result_t1, message->a[j], message->_a[j]);
        element_mul(result_t0, result_t0, result_t1);

        element_mul(result1, message->V[j], message->_V[j]);
        element_mul(result, result, result1);
    }
    element_mul(result0, message->D1, message->D2);
    element_mul(result1, result, message->alpha);
    element_mul(result, result0, result1);
    element_pairing(result_t1, result, group->g2);
    element_mul(result_t0, result_t1, result_t0);

    length = element_length_in_bytes(result_t0);
    unsigned char *c = (unsigned char *)pbc_malloc(length);
    element_to_bytes(c, result_t0);
    crypto_hash_sha512_ref(value, c, (unsigned long long)length);

    element_clear(result);
    element_clear(result0);
    element_clear(result1);
    element_clear(result_t0);
    element_clear(result_t1);
    pbc_free(c);
}
