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
#include "libdivide.h"
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

    // preproduct upper_bound and lower bound.  Initialization value for F.
    int64 B_upper;
    int64 B_lower;
    // X is the upper bound on Carmichaels constructed.  Certain functions may choose to ignore this
    bigint X;

    // variables for q, r.  Used in completion check.  mpz are for primality testing.
    int64 q;  bigint r;
    mpz_t q_mpz;  mpz_t r_mpz;

    // variable that stores (P - 1) * (P + D) / 2.  Used to test integrality of q for a given Delta
    int64 q_D;

    // Data structures for an integrality optimization:
    // C = (P^2 + Delta)/D has to be an integer.  So if p | D, there are mod p restrictions on P, Delta
    // Currently only implemented for the primes 2, 3.  So a length 6 array of length 2 bit arrays.
    long total_residue;  // will be 210 for now, 2 * 3 * 5 * 7
    long num_residues;   // will be 4 for now
    long residues_P[210][4];
    long residues_D[210][4];
    long res_P_index;
    long res_D_index;
 
  public:
    // stores pairs (q, r) that complete a Carmichael of the form Pqr
    vector<pair<int64, bigint>> qrs;

  public:
    // default sets B to 2^(16)
    SmallP_Carmichael();

    // set preproduct bounds and Carmichael bound.  Initialize F.  FD gets initialized in a separate function.
    SmallP_Carmichael(int64 B_low_val, int64 B_up_val, bigint X_val);

    // destructor to clear the mpz_t variables.  Then copy construtors to follow rule of 3.
    ~SmallP_Carmichael();
    SmallP_Carmichael(const SmallP_Carmichael& other);
    SmallP_Carmichael operator=(const SmallP_Carmichael& other);

    /* Given an admissible pre-product P, return all Carmichael numbers 
    // up to X of the form Pqr where q and r are primes.
    // Pairs (q,r) that complete the Carmichael are pushed onto vector qrs.
    // Inputs are factorizations of P and P-1.
    // This does only D-Delta method, which isn't as efficient as switching between D-Delta and C-D
    // Left here for timings and for historical interest.
    */
    void all_DDelta(Preproduct& P);

    // using only the DDelta method, compute Carmichaels for all preproducts up to a bound
    // same code as tabulate_car, but with crossover replaced by all_DDelta
    void tabulate_all_DDelta(string cars_file);

    // Similar to above, apply CD method for all D.  Don't use this for production code.
    void all_CD(Preproduct& P);
    void tabulate_all_CD(string cars_file);

    /* Testing has shown that dynamically choosing between C-D and D-Delta methods is better than always 
     * picking one or the other.  This function takes a Preproduct and a D and performs D-Delta.
     * Runs through all divisors of (P-1)(P+D)/2, writes pairs (q,r) that complete a Carmichael to vector qrs
     */  
    void DDelta(Preproduct& P, bigint D, libdivide::divider<int64>& fastD);

    /* Given a Preproduct and a D value, compute all Carmichael numbers.  This algorithm due to Pinch
     */
    void CD(Preproduct& P, bigint D, libdivide::divider<int64>& fastD); 

  /* Once I have the loop over divisors of (P-1)(P+D)/Delta, I need to perform the following steps:
    1) Compute C = (P^2 + Delta)/D, check that it is integral (unless C != 0, meaning given as parameter)
    2) Check Pqr for Korselt criterion: L | Pqr - 1
    3) check q-1 and r-1 for small prime factors (compositeness check)
    4) Compute q = (P-1)(P+D)/Delta + 1, check primality
    5) Compute r = (P-1)(P+C)/Delta + 1, check integrality and primality
  Note that I know q is integral, because Delta chosen as divisor of (P-1)(P+D).
  primality of q, r is determined through gmp func: mpz_probab_prime_p, which does Baillie-PSW
  If completion works, returns true and writes pair (q,r) to the qrs vector.  If it doesn't, returns false.
  */
    bool completion_check(Preproduct& P, int64 Delta, int64 D, libdivide::divider<int64>& D_div, int64 C_param = 0);
   
  /* Construct Carmichaels for a range of pre-products P
 *   We use a factgen2 object, write to a file.  Only process admissable pre-products, divide up work among
 *   residue classes (processor modulo num_threads).
 *   Calls preproduct_crossover, which does a mix of D-Delta and C-D methods.  Dynamic flag set to false.
 *
 *   If bounded bool true, Carmichaels constructed will be smaller than X.  If false, no upper bound on Carmichaels.
 *
 *   Two choices for output:  (1) if verbose is true, print n followed by its factorization (space separated)
 *   (2) if verbose false, print preproduct, followed by q then r (space separated)
 */
    void tabulate_car(long processor, long num_threads, string cars_file, bool bounded, bool verbose_output);

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
    void preproduct_crossover(Preproduct& P);

    /* prints out admissable pre-products in a given range */
    //void find_admissable(int64 low, int64 high);

};

#endif
