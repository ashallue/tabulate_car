/* Constructing Carmichael numbers.
  New algorithm that improves upon Pinch
  Andrew Shallue and Jonathan Webster
  Fall 2021

  This algorithm is not set up to handle tabulations beyond 
  64 bits.
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

using namespace std;

#ifndef CONSTRUCT_CAR
#define CONSTRUCT_CAR

class Construct_car{
  private:
    // Object for the incremental sieve
    Factgen2 F;

    // Object for the P+D sieve
    Factgen FD;

    // bound of the sieve.  Will be twice X
    long B;
    // bound of the Carmichael construction
    long X;

  public:
    // default sets X to 2^(16), B to twice that.
    Construct_car();

    // Tests whether a given pre-product P is admissable, 
    // i.e. that gcd(p-1, P) = 1 for all p | P, and P squarefree
    // Note P given as a list of its unique prime factors.
    // If it is admissable, returns L = lcm_{p | P}(p-1).  If not, returns 0
    int64 admissable(int64 P, vector<int64> Pprimes);

    /* Given an admissible pre-product P, return all Carmichael numbers 
    // up to X of the form Pqr where q and r are primes.
    // Returns list of pairs (q,r) such that Pqr is Carmichael (except for primality test on r)
    // Inputs are factorizations of P and P-1.
    */
    vector<pair<int64, bigint>> preproduct_construction(vector<int64> P, vector<int64> Pminus, int64 L);

  /* Once I have the loop over divisors of (P-1)(P+D)/Delta, I need to perform the following steps:
    1) Compute C = (P^2 + Delta)/D, check that it is integral
    2) Check Pqr for Korselt criterion: L | Pqr - 1
    3) check q-1 and r-1 for small prime factors (compositeness check)
    4) Compute q = (P-1)(P+D)/Delta + 1, check primality
    5) Compute r = (P-1)(P+C)/Delta + 1, check integrality, 
  Note that I know q is integral, because Delta chosen as divisor of (P-1)(P+D).
  Other input: factorizations of P-1 and P+D, which we use to do primality via q-1 factorization.
  Note that r is not proven prime, that will be dealt within post-processing.

  Returns a pair (q, r) if the completion works.  Returns (0, 0) if it doesn't
  */
    pair<int64, bigint> completion_check(int64 P, int64 Delta, int64 D, int64 L, vector<int64> Pminus, vector<int64> PplusD);
   
  /* Construct Carmichaels for a range of pre-products P
 *   We use a factgen2 object, write to a file.  Only process pre-products that fall into residue class of the core.
 */
    void tabulate_car(int64 B, long processor, long num_threads, string cars_file, string admissable_file);

    /* Reads in a file of Carmichael numbers of the form P q r, computes the product, then sorts resulting list
 *  */
    vector<bigint> product_and_sort(string cars_file);

    /* prints out admissable pre-products in a given range */
    //void find_admissable(int64 low, int64 high);
 
};

#endif
