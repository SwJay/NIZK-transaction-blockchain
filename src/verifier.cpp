//
// Created by swj on 18-7-26.
//

#include "verifier.h"

/* Cipher ****************************************************************/
_Cipher::_Cipher(pairing_t pairing) {
    element_init_G1(c1, pairing);
    element_init_G1(c2, pairing);
    element_init_G1(c3, pairing);
}

_Cipher::~_Cipher() {
    element_clear(c1);
    element_clear(c2);
    element_clear(c3);
}

void _Cipher::set(element_s *_c1, element_s *_c2, element_s *_c3) {
    element_set(c1, _c1);
    element_set(c2, _c2);
    element_set(c3, _c3);
}

/* Commitment ****************************************************************/
_Commitment::_Commitment() {




}
