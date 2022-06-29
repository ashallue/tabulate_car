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
#include "gmp.h"
#include "Odometer.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>


using namespace std;

#ifndef CONSTRUCT_CAR
#define CONSTRUCT_CAR

// Sometimes we need to convert from 128-bit words to two 64-bit words
// This union structure allows the two representations to occupy same spot in memory
//union Dual_rep{
//  bigint double_word;
//  unsigned long int two_words[2];
//};

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
    int64 admissable(int64 P, int64* Pprimes, long Pprimes_len);

    /* Given an admissible pre-product P, return all Carmichael numbers 
    // up to X of the form Pqr where q and r are primes.
    // Returns list of pairs (q,r) such that Pqr is Carmichael (except for primality test on r)
    // Inputs are factorizations of P and P-1.
    // This does only D-Delta method, which isn't as efficient as switching between D-Delta and C-D
    // Left here for timings and for historical interest.
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
  primality of q, r is determined through gmp func: mpz_probab_prime_p, which does Baillie-PSW
  Returns a pair (q, r) if the completion works.  Returns (0, 0) if it doesn't
  */
    pair<int64, bigint> completion_check(int64 P, int64 Delta, int64 D, int64 L, int64* q_primes, long q_primes_len);
   
  /* Construct Carmichaels for a range of pre-products P
 *   We use a factgen2 object, write to a file.  Only process admissable pre-products, divide up work among
 *   residue classes (processor modulo num_threads).
 *   Calls preproduct_crossover, which does a mix of D-Delta and C-D methods.  Dynamic flag set to false.
 */
    void tabulate_car(int64 B, long processor, long num_threads, string cars_file, string admissable_file);

    /* Construct Carmichaels for prime pre-products P.  Similar to tabulate_car
 *     Note this calls preproduct_construction, so no crossover at all, only D-Delta
 * */
    void tabulate_car_primeP(int64 B, long processor, long num_threads, string cars_file);

    /* version that calls the crossover version.  The T/F flag is for static or dynamic crossover.
 */
    void tabulate_car_primeP_crossover(int64 B, long processor, long num_threads, string cars_file, bool dynamic);

    /* preproduct construction with a crossover strategy.
 * The boolean chooses between two different versions.
 * True: dynamic version, compares tau((P-1)(P+D)) to L_p / D    
 * False: contant version, switch from D-Delta to CD at D = P / (ln P)^2
 */
    vector<pair<int64, bigint>> preproduct_crossover(int64* P, long P_len, int64* Pminus, long Pminus_len, int64 L, bool dynamic);

    /* prints out admissable pre-products in a given range */
    //void find_admissable(int64 low, int64 high);
 
};

#endif
