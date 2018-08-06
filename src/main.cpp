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
    // a single transaction
    try{
        Account *A = new Account(dsc, 100000);
        Account *B = new Account(dsc, 50000);
        amount = 10000;

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