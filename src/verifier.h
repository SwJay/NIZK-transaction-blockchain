//
// Created by swj on 18-7-26.
//

#ifndef NIZK_VERIFIER_H
#define NIZK_VERIFIER_H

#include <pbc.h>

#define MAX_SPACE 3

/* Cipher ******************************************************/
typedef struct _Cipher{
public:
    element_t c[MAX_SPACE];

    explicit _Cipher(pairing_t pairing);
    ~_Cipher();
    void set(element_t _c1, element_t _c2, element_t _c3);
}Cipher;

/* Commitment ******************************************************/
typedef struct _Commitment{
public:
    element_t R1, R2, _R1, _R2;
    element_t V[MAX_SPACE], _V[MAX_SPACE], a[MAX_SPACE], _a[MAX_SPACE];
    element_t D1, D2;
    element_t alpha;

    explicit _Commitment(pairing_t pairing);
    ~_Commitment();
}Commitment;

/* Response ******************************************************/
typedef struct _Response{
public:
    element_t z1, z2;
    element_t zv[MAX_SPACE], _zv[MAX_SPACE], zt[MAX_SPACE], _zt[MAX_SPACE];
    element_t zl, zk;
    element_t tmp_z;

    explicit _Response(pairing_t pairing);
    ~_Response();
}Response;

/* Proof ******************************************************/
typedef struct _Proof{
public:
    Commitment *commitment;
    element_t challenge;
    Response *response;

    explicit _Proof(pairing_t pairing):commitment(nullptr), response(nullptr) {element_init_Zr(challenge, pairing);}
    _Proof(Commitment *commitment, Response *response, element_t _challenge, pairing_t pairing): commitment(commitment), response(response) {
        element_init_Zr(challenge, pairing);
        element_set(challenge, _challenge);
    }
    //_Proof(const struct Proof &proof);
    ~_Proof();
}Proof;

/* Verifier ******************************************************/
class DSC;
class Account;
class Verifier{
public:
    Verifier() = default;
    ~Verifier() = default;

    bool verify(DSC *dsc, Proof *proof, Account A, Account B, Cipher *C1, Cipher *C2, Cipher *C3);
};

#endif //NIZK_VERIFIER_H
