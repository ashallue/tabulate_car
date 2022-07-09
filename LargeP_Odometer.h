/* Andrew Shallue, Tabulating Carmichaels project. 2022
 * The goal of this class is to construct all pre-products greater than a bound.
 * Helper class for the case of tabulating Carmichaels with large pre-product.
 *
 * This class works under the assumption that the bound on the Carmichaels constructed is at most 128 bits
 */

using namespace std;

#ifndef LARGEP_H
#define LARGEP_H

#include "bigint.h"
#include "functions.h"
#include <vector>
#include <math.h>

class LargeP_Odometer{
  public:
    static const long max_d = 25;   // At most 25 distinct primes can be multiplied and stay within 128 bits
    long curr_d;  // count of prime factors of n.  count for P is d-2
    long P_len;     // count of prime factors of P.  We have P_len = curr_d - 2
    vector<long> primes; // vector of primes of size X = B^{1/3}   
    long uppers[max_d];  // upper bounds on prime factors
    long lowers[max_d];  // lower bounds on prime factors

    long indices[max_d]; // stores the indices of the primes dividing current pre-product
    bigint P_curr;       // current pre-product that corresponds to the index array
  
  public:
    // helper function.  Given lower bound, find index of the smallest prime larger than the bound
    // Algorithm is binary search.  Return 0 if bound is greater than prime_B (corresponds to prime 2)
    // minimum bound that works is 3, otherwise will go into an infinite loop since no prime < 2
    long find_index_lower(long bound);

  public:
    bigint B;     // bound on Carmichaels constructed
    bigint X;     // lower bound on pre-products P such that n = Pqr
    long prime_B; // bound on the prime factors of P

    // For now, this constructor orients around Carmichaels with three prime factors
    LargeP_Odometer();
    
    // Rotate the indices.  Starts with furthest right index, depending on P_len
    void next(); 

    // retrieve current pre-product.  A simple getter function
    bigint get_P();
  
};

#endif
