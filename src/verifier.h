//
// Created by swj on 18-7-26.
//

#ifndef NIZK_VERIFIER_H
#define NIZK_VERIFIER_H

#include <pbc.h>
#define MAX_SPACE 3

typedef struct _Cipher{
public:
    element_t c1, c2, c3;

    explicit _Cipher(pairing_t pairing);
    ~_Cipher();
    void set(element_t _c1, element_t _c2, element_t _c3);
}Cipher;

typedef struct _Commitment{
public:
    element_t R1, R2, _R1, _R2;
    element_t V[MAX_SPACE], _V[MAX_SPACE], a[MAX_SPACE], _a[MAX_SPACE];
    element_t D1, D2;
    element_t alpha;

    explicit _Commitment();
    ~_Commitment();
}Commitment;

typedef struct _Response{

}Response;

typedef struct _Proof{

}Proof;

#endif //NIZK_VERIFIER_H
