/* Andrew Shallue, May 2022
 * Part of a joint project with Jonathan Webster: tabulating Carmichael numbers.
 * A class that uses options to do different kinds of tabulations.
 */

#include "functions.h"
#include "Factgen.h"
#include "int.h"
#include "bigint.h"
#include "Odometer.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

#ifndef CONSTRUCT_CAR_OPTIONS
#define CONSTRUCT_CAR_OPTIONS

class Construct_car_options{
  private:
    Factgen2 F;    // object for the incremental sieve
    Factgen FD;    // object for the P+D sieve
    long B_sieve;  // bound of the sieve.

  public:
    long B_preproduct;  // maximum size of preproducts
    long B_car;         // maximum size of carmichaels tabulated
  
    // if true, only prime preproducts considered
    // if false, all preproducts are part of the tabulation
    bool prime_preproducts;  
    // if true, calculate all pairs (q,r) that complete each preproduct
    // if false, stop when the carmichael constructed is greater than B_car
    bool preproduct_complete; 
    // if true, crossover between D-Delta and C-D methods is P / (ln P)^2
    // if false, crossover point is calculated dynamically
    bool trivial_crossover;

  public:

    // default sets X to 2^(16), B to twice that.
    Construct_car_options();

    // Tests whether a given pre-product P is admissable, 
    // i.e. that gcd(p-1, P) = 1 for all p | P, and P squarefree
    // Note P given as a list of its unique prime factors.
    // If it is admissable, returns L = lcm_{p | P}(p-1).  If not, returns 0
    int64 admissable(int64 P, int64* Pprimes, long Pprimes_len);

    /* Given an admissible pre-product P, return all Carmichael numbers 
    // up to X of the form Pqr where q and r are primes.
    // Returns list of pairs (q,r) such that Pqr is Carmichael (except for primality test on r)
    // Inputs are factorizations of P and P-1.
    */
    vector<pair<int64, bigint>> preproduct_construction(int64* P, long P_len, int64* Pminus, long Pminus_len, int64 L);

  /* Once I have the loop over divisors of (P-1)(P+D)/Delta, I need to perform the following steps:
    1) Compute C = (P^2 + Delta)/D, check that it is integral
    2) Check Pqr for Korselt criterion: L | Pqr - 1
    3) check q-1 and r-1 for small prime factors (compositeness check)
    4) Compute q = (P-1)(P+D)/Delta + 1, check primality
    5) Compute r = (P-1)(P+C)/Delta + 1, check integrality, 
  Note that I know q is integral, because Delta chosen as divisor of (P-1)(P+D).
  Other input: unique primes dividing (P-1)(P+D)/2, which we use to do primality via q-1 factorization.
  Note that r is not proven prime, that will be dealt within post-processing.

  Returns a pair (q, r) if the completion works.  Returns (0, 0) if it doesn't
  */
    pair<int64, bigint> completion_check(int64 P, int64 Delta, int64 D, int64 L, int64* q_primes, long q_primes_len);
   
  /* Construct Carmichaels for a range of pre-products P
 *   We use a factgen2 object, write to a file.  Only process pre-products that fall into residue class of the core.
 */
    void tabulate_car(int64 B, long processor, long num_threads, string cars_file, string admissable_file);

    /* Construct Carmichaels for prime pre-products P.  Similar to tabulate_car */
    void tabulate_car_primeP(int64 B, long processor, long num_threads, string cars_file);

    /* version that calls the crossover version
 */
    void tabulate_car_primeP_crossover(int64 B, long processor, long num_threads, string cars_file);

    /* preproduct construction with a crossover strategy
 *     This version implements a basic version: switch from D-Delta to CD at D = P / (ln P)^2
 */
    vector<pair<int64, bigint>> preproduct_crossover(int64* P, long P_len, int64* Pminus, long Pminus_len, int64 L);

    /* prints out admissable pre-products in a given range */
    //void find_admissable(int64 low, int64 high);
 

};

#endif
