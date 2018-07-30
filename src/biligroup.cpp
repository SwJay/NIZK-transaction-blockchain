//
// Created by swj on 18-7-25.
//

#include "biligroup.h"
#include <ctime>

/* DSC Ctor
 * get a bilinear group from a.param
 * TODO: generate from a given security parameter n
 */
BiliGroup::BiliGroup() {
    FILE *file = fopen("a.param","r");
    char param[1024];
    int reps = 30;
    size_t count;
    element_t w;

    /*gmp_randinit_default(state); // random state init
    gmp_randseed_ui(state, (unsigned)time(nullptr));
    do{
        mpz_urandomb(p, state, n); // generate random p
    }while(!mpz_probab_prime_p(p, reps));*/

    count = fread(param, 1, 1024, file);
    if(!count)
        pbc_die("input error for setup");
    pairing_init_set_buf(pairing, param, count); // pairing init
    element_init_G1(g1, pairing); // g1 init
    element_init_G2(g2, pairing); // g2 init
    element_init_GT(gt, pairing); // gt init
    element_init_G1(h, pairing); // h init
    element_init_Zr(w, pairing); // w init

    element_random(g1); // assign g1
    element_random(g2); // assign g2
    element_pairing(gt, g1, g2); // assign gt
    element_random(w);
    element_pow_zn(h, g1, w); // assign h

    // clear
    element_clear(w);
}

BiliGroup::~BiliGroup() {
    element_clear(g1);
    element_clear(g2);
    element_clear(gt);
    element_clear(h);
    pairing_clear(pairing);
}
