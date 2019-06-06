# NIZK-transaction-blockchain

Implement an efficient NIZK scheme for private transactions on account-model blockchain. Reference paper can be found [here](https://eprint.iacr.org/2017/1239.pdf).

## privacy solutions on UTXO based blockchains:

- [Zcash](https://z.cash/zh/): zk-SNARK
- [Monero](https://z.cash/zh/): Ring CT

## How it works?

Since it preserves privacy for transactions in account-model blockchain like ethereum, similar to a bank trading database system, The key is to prove equivalence for the amounts modified on both accounts and enough for the sender's balance.

The former one is designed with a Sigma-protocol while the latter one is proved by range proof, and both are included to construct a NIZK scheme. In this repo I implemented a POC for this system in locol version, importing pbc library to support bilinear group programming.
