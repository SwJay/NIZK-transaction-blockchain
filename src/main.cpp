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
    Account *A = new Account(dsc, 43124);
    Account *B = new Account(dsc, 50342);
    amount = 4125;
    proof = A->transfer(dsc, B, amount, Ca, Cb);
    if(verifier.verify(dsc, proof, A, B, A->cipherBalance, Ca, Cb))
        dsc->transfer(A, B, Ca, Cb);

    delete Ca;
    delete Cb;
    delete proof;
    delete A;
    delete B;
    delete dsc;
    return 0;
}