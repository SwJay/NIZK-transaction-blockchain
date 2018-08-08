#include <iostream>
#include "DSC.h"
#include "account.h"
#include "verifier.h"
#include <random>
#include <ctime>
#include <sys/time.h>

#define LOW 0
#define HIGH (unsigned int)(pow(2, 30) - 1)
#define SAMPLE_SIZE 500

using namespace std;

int main() {
    auto *dsc = new DSC();
    auto *Ca = new Cipher(dsc->group->pairing);
    auto *Cb = new Cipher(dsc->group->pairing);
    Proof *proof;
    Verifier verifier;
    uint amount;
    uint balance;
    struct timeval t1, t2;
    long totalTime1 = 0;

    default_random_engine e((unsigned long)time(0));
    uniform_int_distribution<unsigned> u(LOW, HIGH);
    vector<unsigned int> rand_vec;

    /**
     * A single transaction (available amount ranges from 0 ~ 2^30-1 )
     * error catch:
     * 1. initialize account with negative balance
     * 2. transfer amount is set negative
     * */
    try{
        for(int i = 0; i < SAMPLE_SIZE; i++){
            balance = (uint)(i + 1) * 10000;
            Account *A = new Account(dsc, balance);
            Account *B = new Account(dsc, balance);
            amount = u(e) % balance;

            gettimeofday(&t1, NULL);

            proof = A->transfer(dsc, B, amount, Ca, Cb);
            if(verifier.verify(dsc, proof, A, B, A->cipherBalance, Ca, Cb))
                dsc->transfer(A, B, Ca, Cb);

            gettimeofday(&t2, NULL);
            totalTime1 += (t2.tv_sec - t1.tv_sec) * 1000000 + t2.tv_usec - t1.tv_usec;



//            cout << "A: " << A->getBalance(dsc->group) << endl;
//            cout << "B: " << B->getBalance(dsc->group) << endl;

            delete A;
            delete B;
        }
    }catch(const char *msg){
        cerr << msg << endl;
    }

    cout << totalTime1 << endl;

    delete Ca;
    delete Cb;
    delete proof;
    delete dsc;
    return 0;
}