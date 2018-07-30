#include <iostream>
#include "DSC.h"
#include "account.h"
#include "verifier.h"

using namespace std;

int main() {
    DSC dsc;
    Proof proof;
    Verifier verifier;
    Account A(dsc.group, 50), B(dsc.group, 0);
    proof = A.transfer(&dsc, B, 25);
    if(verifier.verify(proof))

    return 0;
}