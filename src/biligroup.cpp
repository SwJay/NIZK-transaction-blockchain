//
// Created by swj on 18-7-25.
//

#include "biligroup.h"
#include <ctime>

BiliGroup::BiliGroup(uint n) {
    FILE *file = fopen("a.param","r");
    char param[1024];
    size_t count;
    gmp_randstate_t state;

    gmp_randinit_default(state); // random state init
    gmp_randseed_ui(state, (unsigned)time(nullptr));
    mpz_urandomb(p, state, n); // generate random p

    count = fread(param, 1, 1024, file);
    if(!count)
        pbc_die("input error for setup");
    pairing_init_set_buf(pairing, param, count); // pairing init
    element_init_G1(g1, pairing); // g1 init
    element_init_G1(h, pairing); // h init
    element_init_G2(g2, pairing); // g2 init

    element_random(g1); // assign g1
    element_random(h); // assign h
    element_random(g2); // assign g2
}
// h的选取，改成g1^w