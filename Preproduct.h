/* Class that stores information about a pre-product.
 * Specifically, P for a Carmichael number of the form n = Pqr.
 * Part of tabulating Carmichaels project.
 * Andrew Shallue, September 2022
 */

#ifndef PREPRODUCT_H
#define PREPRODUCT_H

using namespace std;

class Preproduct{
  public:
    // data members
    int64* Pprimes;     // the primes dividing the preproduct
    long   Pprimes_len; // the number of prime factors of P
    int64* Pminus;      // 
    long   Pminus_len;
    int64 L;            // will hold lcm_{p | P} (p-1)
    int64 Tau;          // The divisor count of P-1

    // This constructor takes factorizations of P and P-1 as input.  The object created allocates new memory.
    // The constructor also calculates L and Tau.
    PreProduct(int64* Pfac, long Pfac_len, int64* PMfac, long PMfac_len);

    

// I should include the admissable function in this class
};

#endif

