/* Second attempt at large preproducts, this time using for loops.
 * I think this means that this same class also has to complete the Carmichael numbers.
 * Andrew Shallue, Spring 2023, part of Tabulating Carmichaels project
 */

#ifndef LARGEPREPRODUCT_H
#define LARGEPREPRODUCT_H

#include <string>
#include <math.h>
#include "bigint.h"
#include "functions.h"
#include <fstream>

using namespace std;

/* Notation:
 * We are constructing Carmichael numbers up to B. These numbers have d prime factors.
 * Preproducts constructed have d-2 prime factors, and are larger than X.
 * max_d is the largest possible value for d, given B, i.e. product of primes up to n
 */

class LargePreproduct{
  public:
    bigint B;  // bound on Carmichaels constructted
    long   X;  // lower bound on preproducts
    long max_d;  // largest possible value for d
    long prime_B;  // largest value for q = p_{d-1}
    
    // primes array
    long* primes;
    long primes_count;

  public: 
    // default values are B = 100,001 and X = B^{1/3}
    LargePreproduct();
    // constructor that takes B, X as input
    LargePreproduct(bigint B_init, long X_init);

    // Following rule of 3 to clean up the primes array
    ~LargePreproduct();
    LargePreproduct& operator=(const LargePreproduct &other);
    LargePreproduct(const LargePreproduct &other);

    // simple functions, primarily for testing purposes
    // this one constructs Carmichaels with d = 4 and writes to file
    void cars4(string cars_file);

  private:

    // helper function.  Given lower bound, find index of the smallest prime larger than the bound
    // Algorithm is binary search.  Return 0 if bound is greater than prime_B (corresponds to prime 2)
    // minimum bound that works is 3, otherwise will go into an infinite loop since no prime < 2
    long find_index_lower(long bound);

    // helper function.  Given num, den, root compute bound = (num / den)^(1/root), then find the index 
    // of the largest prime smaller than that bound and return that index
    long find_index_upper(bigint num, bigint den, long root);

};

#endif
