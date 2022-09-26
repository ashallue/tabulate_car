/* Constructing Carmichael numbers.
  New algorithm that improves upon Pinch
  Andrew Shallue and Jonathan Webster
  Fall 2021

  This algorithm is not set up to handle tabulations beyond 
  64 bits.
*/
#include "functions.h"
#include "Factgen.h"
#include "Preproduct.h"
#include "int.h"
#include "bigint.h"
#include "gmp.h"
#include "Odometer.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>


using namespace std;

#ifndef SMALLP_CONSTRUCT_H
#define SMALLP_CONSTRUCT_H


class SmallP_Carmichael{
  private:
    // Object for the incremental sieve
    Factgen2 F;

    // Object for the P+D sieve
    Factgen FD;

    // preproduct upper_bound.  Initialization value for F.
    int64 B;

    // variables for q, r.  Used in completion check.  mpz are for primality testing.
    int64 q;  bigint r;
    mpz_t q_mpz;  mpz_t r_mpz;

  public:
    // default sets B to 2^(16)
    SmallP_Carmichael();

    // set preproduct bound B to given value.  Initialize F.  FD gets initialized in a separate function.
    SmallP_Carmichael(int64 B_val);

    // destructor to clear the mpz_t variables.  Then copy construtors to follow rule of 3.
    ~SmallP_Carmichael();
    SmallP_Carmichael(const SmallP_Carmichael& other);
    SmallP_Carmichael operator=(const SmallP_Carmichael& other);

    /* Given an admissible pre-product P, return all Carmichael numbers 
    // up to X of the form Pqr where q and r are primes.
    // Returns list of pairs (q,r) such that Pqr is Carmichael (except for primality test on r)
    // Inputs are factorizations of P and P-1.
    // This does only D-Delta method, which isn't as efficient as switching between D-Delta and C-D
    // Left here for timings and for historical interest.
    */
    vector<pair<int64, bigint>> all_DDelta(Preproduct& P);

    /* Testing has shown that dynamically choosing between C-D and D-Delta methods is better than always 
     * picking one or the other.  This function takes a Preproduct and a D and performs D-Delta
     */  
    vector<pair<int64, bigint>> DDelta(Preproduct& P, bigint D);

    /* Given a Preproduct and a D value, compute all Carmichael numbers.  This algorithm due to Pinch
     */
    vector<pair<int64, bigint>> CD(Preproduct& P, bigint D); 

  /* Once I have the loop over divisors of (P-1)(P+D)/Delta, I need to perform the following steps:
    1) Compute C = (P^2 + Delta)/D, check that it is integral (unless C != 0, meaning given as parameter)
    2) Check Pqr for Korselt criterion: L | Pqr - 1
    3) check q-1 and r-1 for small prime factors (compositeness check)
    4) Compute q = (P-1)(P+D)/Delta + 1, check primality
    5) Compute r = (P-1)(P+C)/Delta + 1, check integrality and primality
  Note that I know q is integral, because Delta chosen as divisor of (P-1)(P+D).
  primality of q, r is determined through gmp func: mpz_probab_prime_p, which does Baillie-PSW
  Returns a pair (q, r) if the completion works.  Returns (0, 0) if it doesn't
  */
    pair<int64, bigint> completion_check(Preproduct& P, int64 Delta, int64 D, int64 C_param = 0);
   
  /* Construct Carmichaels for a range of pre-products P
 *   We use a factgen2 object, write to a file.  Only process admissable pre-products, divide up work among
 *   residue classes (processor modulo num_threads).
 *   Calls preproduct_crossover, which does a mix of D-Delta and C-D methods.  Dynamic flag set to false.
 */
    void tabulate_car(long processor, long num_threads, string cars_file, string admissable_file);

    /* Construct Carmichaels for prime pre-products P.  Similar to tabulate_car
 *     Note this only does D-Delta.  Thus bad for production; only use for timing comparisons with Pinch
 * */
    void tabulate_car_primeP(long processor, long num_threads, string cars_file);

    /* version that calls the crossover version. 
 */
    void tabulate_car_primeP_crossover(long processor, long num_threads, string cars_file);


  /* Another version, but this one has the D crossover strategy
   * For each D, we choose either D-Delta method or CD method depending on the anticipated amount of work for each.
   * This involves calculating L_p, the length of the interval for CD method, and estimating the number of divisors
   * of (P-1)(P+D) for the D-Delta method. 
  */
    vector<pair<int64, bigint>> preproduct_crossover(Preproduct& P);

    /* prints out admissable pre-products in a given range */
    //void find_admissable(int64 low, int64 high);
 
};

#endif
