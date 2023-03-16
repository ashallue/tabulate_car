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

using namespace std;

/* Notation:
 * We are constructing Carmichael numbers up to B. These numbers have d prime factors.
 * Preproducts constructed have d-1 prime factors, and are larger than X.
 * max_d is the largest possible value for d, given B, i.e. product of primes up to n
 */

class LargePreproduct{
  private:
    bigint B;  // bound on Carmichaels constructted
    long   X;  // lower bound on preproducts
    long max_d;  // largest possible value for d
    long prime_B;  // largest value for q = p_{d-1}

  public: 
    // default values are B = 100,001 and X = B^{1/3}
    LargePreproduct();
    // constructor that takes B, X as input
    LargePreproduct(bigint B_init, long X_init);


};

#endif
