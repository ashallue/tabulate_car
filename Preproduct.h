/* Class that stores information about a pre-product.
 * Specifically, P for a Carmichael number of the form n = Pqr.
 * Part of tabulating Carmichaels project.
 * Andrew Shallue, September 2022
 */

#ifndef PREPRODUCT_H
#define PREPRODUCT_H

using namespace std;

#include "int.h"
#include "bigint.h"
#include "functions.h"

class Preproduct{
  public:
    // data members
    int64* Pprimes;     // the primes dividing the preproduct
    long   Pprimes_len; // the number of prime factors of P
    int64* Pminus;      // 
    long   Pminus_len;
    int64 L;            // will hold lcm_{p | P} (p-1)
    int64 Tau;          // The divisor count of P-1
    int64 Prod;         // the product of the primes in Pprimes, i.e. the actual pre-product
    bool admissable;    // true if P is squarefree and gcd(p-1, P)=1 forall p | P   

    // default constructor
    Preproduct();

    // This constructor takes factorizations of P and P-1 as input.  The object created allocates new memory.
    // The constructor also calculates L and Tau.
    Preproduct(int64 Pval, int64* Pfac, long Pfac_len, int64* PMfac, long PMfac_len);

    // For the large preproduct case, we don't necessarily have factorization of P-1.
    // So this constructor only populates Pprimes, and only computes L
    Preproduct(int64 Pval, int64* Pfac, long Pfac_len);

    // Need a destructor to free the memory, then copy constructors for rule of 3
    ~Preproduct();
    Preproduct(const Preproduct& other);
    Preproduct operator=(const Preproduct& other);
    
    // computes whether the preproduct is admissable in a Carmichael sense
    // that is, returns true iff P squarefree and gcd(p-1, P) = 1 forall p | P
    bool is_admissable();

    // Given unique prime factors of P+D, compute the complete prime factorization of q = (P-1)(P+D)/2
    // All arrays are passed by reference.  Return value is the length of the q_primes, q_exps arrays.
    long q_factorization(int64 q, int64* PplusD, long PplusD_len, int64* q_primes, long* q_exps);

    // return the largest prime dividing the preproduct.  Recall they are stored in increasing order.
    int64 largest_prime();

};

#endif

