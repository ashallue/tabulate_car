/* Andrew Shallue, Tabulating Carmichaels project. 2022
 * The goal of this class is to construct all pre-products greater than a bound.
 * Helper class for the case of tabulating Carmichaels with large pre-product.
 * Important!  We assume the lower bound is at most 64 bits, but note pre-products can be greater than that..
 *
 * This class works under the assumption that the bound on the Carmichaels constructed is at most 128 bits
 */

using namespace std;

#ifndef LARGEP_H
#define LARGEP_H

#include "bigint.h"
#include "functions.h"
#include "Odometer.h"
#include <vector>
#include <math.h>
#include <fstream>
#include <string>

class LargeP_Odometer{
  public:
    // max_d is the max num of distinct primes that can be multiplied up to B
    // Note that I will make room for d primes in the pre-product, though we only need d-2
    // Note that at most 25 distinct primes can be multiplied and fit in 128 bits
    long max_d;
    long curr_d;  // count of prime factors of n.  count for P is d-2
    long P_len;     // count of prime factors of P.  We have P_len = curr_d - 2
    long* primes;  // array of the primes up to B^{1/3}.  primes_count has the length
    long primes_count;
   
    long* uppers;  // upper bounds on prime factors
    long* lowers;  // lower bounds on prime factors

    long* indices; // stores the indices of the primes dividing current pre-product
    bigint P_curr;       // current pre-product that corresponds to the index array
  
  public:
    // helper function.  Given lower bound, find index of the smallest prime larger than the bound
    // Algorithm is binary search.  Return 0 if bound is greater than prime_B (corresponds to prime 2)
    // minimum bound that works is 3, otherwise will go into an infinite loop since no prime < 2
    long find_index_lower(long bound);

    // if the starting pre-product is composite rather than prime, this function will find it.
    // Happens when X > B^{1/3}.  Not efficient: steps through vi Odometer class, calling find_index_lower
    void set_start_preproduct();

    // helper function.  Given a prime, return the index corresponding to its location in primes array
    // returns -1 if for some reason it is not found
    long find_index(long given_prime);

    // helper function.  Assuming we have finished pre-products for Carmichael numbers with current length,
    // reset for the next length.  This involves updated variables and changing the indices array
    void next_nextd();

  public:
    bigint B;     // bound on Carmichaels constructed
    long X;       // lower bound on pre-products P such that n = Pqr
    long prime_B; // bound on the prime factors of P

    // Sets up all the variables.  Default B = 100001 and X = B^{1/3}
    LargeP_Odometer();

    // Constructor that takes B and X as input.  Calculates max_d, finds first pre-product
    // Assumes first pre-product has 1, 2, or 3 prime factors.  So fails if X > B^{3/5}.
    LargeP_Odometer(bigint B_init, long X_init);
  
    // destructor releases memory for the arrays;
    ~LargeP_Odometer();

    // copy constructor and assignment operator.  Remember the rule of 3 andrew.
    LargeP_Odometer(const LargeP_Odometer& other_od);
    LargeP_Odometer operator=(const LargeP_Odometer& other_od);
    
 
    // Construct odometer with given curr_d.  I only intend to use this as a helper, so primes not filled in.
    // not sure I need it.  Unfinished for now.
    LargeP_Odometer(long d);
 
    //Rotate the indices.  Starts with furthest right index, depending on P_len
    void next(); 

    // retrieve current pre-product.  A simple getter function
    bigint get_P();

    // retrieve the factorization of the current pre-product.
    // Passed pointer gets the array of primes, the return value is its length
    long get_Pfactors(int64* factors);

    // retrieve p_{d-2}, i.e. the largest prime dividing the pre-product
    long get_largest_prime();

    // print the entries of the indices array, the uppers array, and the lowers array
    void print_info();

    // repeatedly call next() to compute all pre-products and write them to a file.  
    // Stops when the product of the first d primes is greater than B
    void large_products(string filename);
 
    // A trivial algorithm for tabulating large pre-products.  Outputs P to a file given by filename
    void trivial_large_products(string filename);
};

#endif
