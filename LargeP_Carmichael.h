/* Andrew Shallue, Tabulating Carmichael project.  2022
 * Construct all Carmichael numbers of the form Pqr with large pre-product P.
 * Relies heavily on the class LargeP_Odometer to generate the pre-products.
 */

using namespace std;

#ifndef LARGE_P_CARMICHAEL_H
#define LARGE_P_CARMICHAEL_H

#include "LargeP_Odometer.h"
#include "Preproduct.h"
#include "Odometer.h"

class LargeP_Carmichael{

  public:
    // calls default LargeP_Odometer, so B = 100001, X = B^{1/3}
    LargeP_Carmichael();

    // constructor that sets B, X, then creates Odometer based on those values
    LargeP_Carmichael(bigint B_init, long X_init); 

    // construct Carmichael numbers n < B of the form Pqr, with P >= X
    // threaded by admissable pre-product constructed.  Output written to cars_file
    // uses Pinch's method for large pre-products
    void pinch(long processor, long num_threads, string cars_file); 

    /* For a given preproduct, find all Carmichaels using method of Pinch.
     * We assume the Carmichael constructed is less than B, and P > X
     */
    void preproduct_pinch(Preproduct& P);

    /* For preproduct Pq, check divisors of Pq-1 up to trial_bound, for divisor f 
     * check whether r = (Pq - 1)/f + 1 completes the Carmichael.
     * If it does, put (q, r) as a pair into the built-in qrs vector
     */
    void r_search_trial(Preproduct P, int64 q, int64 bound);

   /* For preproduct Pq, search for r in the arithmetic progression r = (Pq)^{-1} mod L
    * We assume r_search_trial has been applied up to bound, so we know r is smaller than (Pq-1)/bound  
    */
    void r_search_progression(Preproduct P, int64 q, int64 bound);  

    /* Completion check.  Returns true if Pqr is Carmichael.  
     * For this version, we know r-1 | Pq -1, so we check that Pqr satisfies Korselt, and that r is prime
     */
    bool completion_korselt(Preproduct P, int64 q, bigint r);

    /* The completion check assumes r = (Pq)^{-1} mod L, so we know q-1 | Pqr - 1 
     * and p-1 | Pqr - 1 for p | P.  We still need to check that r-1 | Pqr - 1, and that r is prime
     */
    bool completion_divisible(Preproduct P, int64 q, bigint r);

  private:
    bigint B;    // constructing Carmichael numbers n < B
    long   X;    // lower bound on pre-products
    LargeP_Odometer od;  // odometer object that generates pre-products

  public:
    // stores pairs (q, r) that complete a Carmichael of the form Pqr
    vector<pair<int64, bigint>> qrs;
};


#endif
