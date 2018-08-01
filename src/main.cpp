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
    Account A(dsc->group, 100000), B(dsc->group, 100000);

    amount = 5000;
    proof = A.transfer(dsc, B, amount, C1, C2, C3);
    if(verifier.verify(dsc, proof, A, B, C1, C2, C3))
        cout << "hello" << endl; //dsc->transfer(A, B, proof->commitment);

    return 0;
}