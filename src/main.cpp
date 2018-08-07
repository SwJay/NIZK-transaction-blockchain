#include <iostream>
#include "DSC.h"
#include "account.h"
#include "verifier.h"

using namespace std;

int main() {
    auto *dsc = new DSC();
    auto *Ca = new Cipher(dsc->group->pairing);
    auto *Cb = new Cipher(dsc->group->pairing);
    Proof *proof;
    Verifier verifier;
    uint amount;
    /**
     * A single transaction (available amount ranges from 0 ~ 2^30-1 )
     * error catch:
     * 1. initialize account with negative balance
     * 2. transfer amount is set negative
     * */
    try{
        Account *A = new Account(dsc, 10000);
        Account *B = new Account(dsc, 50000);
        amount = 100000;

        proof = A->transfer(dsc, B, amount, Ca, Cb);

        if(verifier.verify(dsc, proof, A, B, A->cipherBalance, Ca, Cb))
            dsc->transfer(A, B, Ca, Cb);

        cout << "A: " << A->getBalance(dsc->group) << endl;
        cout << "B: " << B->getBalance(dsc->group) << endl;

        delete A;
        delete B;
    }catch(const char *msg){
        cerr << msg << endl;
    }

    delete Ca;
    delete Cb;
    delete proof;
    delete dsc;
    return 0;
}