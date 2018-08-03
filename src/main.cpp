#include <iostream>
#include "DSC.h"
#include "account.h"
#include "verifier.h"

using namespace std;

int main() {
    auto *dsc = new DSC();
    auto *C1 = new Cipher(dsc->group->pairing);
    auto *C2 = new Cipher(dsc->group->pairing);
    auto *C3 = new Cipher(dsc->group->pairing);
    Proof *proof;
    Verifier verifier;
    uint amount;
    Account *A = new Account(dsc->group, 43124);
    Account *B = new Account(dsc->group, 50342);

    amount = 43124;
    proof = A->transfer(dsc, B, amount, C1, C2, C3);
    if(verifier.verify(dsc, proof, A, B, C1, C2, C3))
        cout << "hello" << endl; //dsc->transfer(A, B, proof->commitment);

    delete C1;
    delete C2;
    delete C3;
    delete proof;
    delete A;
    delete B;
    delete dsc;
    return 0;
}