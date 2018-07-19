# privacy-preserving under UTXO

## zcash

zk_SNARK

## monero

- ring signature: preserve sender's privacy
- confidential address: preserve receiver's privacy
- ring confidential transaction( ring CT): preserve concrete amount in a transaction



# privacy-preserving under account

## channel, mixer, ring ct

## ZK

### concepts:

- proof of knowledge

  - sigma protocol:

     commitment, challenge, response

  - witness, extractor, NP language: 

    for NP statement, a witness is a  piece of information helps verifying. eg. a concrete cycle in Hamilton Problem, or a corresponding $w$ to verify whether $x$ is a square.

- square:

  - $\exist w \in Z_p$, st. $x\equiv w^2 mod	\space n$, then $x$ is a *square*

- Fiat–Shamir heuristic:

  converting a public-coin interactive proof of knowledge into a non-interactive proof of knowledge:

  - when challenge, Verifier sends a random c. Fiat-Shamir heuristic changes this via using a **non-interactive** random oracle, where in practice we use cryptographic hash function

- 

### zk-SNARK

### zk rang proof

### DSC with NIZK （矩阵元）

#### core: NIZK

*consider A transfer money to B

- prove equivalence, enough:
  - **equivalence**: for privacy, A conceals the transfer amount via using $A_pk$ and $B_pk$ to encrypt it. A has to prove two ciphers holds the same amount
  - **enough**: 常规检查