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
 * Preproducts constructed have d-2 prime factors, and are larger than X (or equal to X).
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

    // magical number.  In the inner loop, if at most this many sieve steps do that instead of other work
    long small_sieve_steps = 50;

    // counts for different innner loop cases;
    long count1 = 0;
    long count2 = 0;
    long count3 = 0;
    long count4 = 0;

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

    // threaded version.  Embarrasingly parallel, distributes according to residue class of outer prime index
    void cars4_threaded(string cars_file, long thread, long num_threads);

    // recursive version, creates carmichael numbers with d prime factors
    void cars_rec(long d, string cars_file);

    // threaded recursive version.  Has initial 3 nested loops, then calls cars_rec_helper
    // requires d >= 6
    void cars_rec_threaded(long d, string cars_file, long thread, long num_threads);

    // note that these functions found in a separate file called extra_large_case.cpp
    void cars5_threaded(string cars_file, long thread, long num_threads);
    void cars6_threaded(string cars_file, long thread, long num_threads);
    void cars7_threaded(string cars_file, long thread, long num_threads);
    void cars8_threaded(string cars_file, long thread, long num_threads); 
    void cars9_threaded(string cars_file, long thread, long num_threads);


  public:

    // recursive version. This helper function tracks preproduct so far.  k is the factor count for preproduct, 
    // while d is the number of factors in the final carmichael number.
    // The pis vector stores indices of the primes making up the preproduct
    void cars_rec_helper(long d, bigint preprod, vector<long> &pis, bigint L, ofstream& file_object);
    
    // helper function.  Given lower bound, find index of the smallest prime larger than the bound
    // Algorithm is binary search.  Return 0 if bound is greater than prime_B (corresponds to prime 2)
    // minimum bound that works is 3, otherwise will go into an infinite loop since no prime < 2
    long find_index_lower(long bound);

    // helper function.  Given num, den, root compute bound = (num / den)^(1/root), then find the index 
    // of the largest prime smaller than that bound and return that index
    long find_index_upper(bigint num, bigint den, long root);

    // helper function.  Return (num / den)^(1/root) as an integer
    long find_upper(bigint num, bigint den, long root);

    // boolean function, checks Korselt condition, applies Baillie-PSW test to r
    bool korselt_check(bigint Pq, bigint L, bigint r);

    // employ the two-divisors result for large L (Lemma 2.1 of Coppersmith, Howgrave-Graham, Nagaraj
    // i.e. find divisors of (Pq - 1) congruent to (Pq)^{-1} - 1 mod L.  Requires gcd 1, 
    // so division by a gcd is performed.  At most 2 divisors found, placed into rs vector.
    // Returns boolean value, false if L too small for technique, true if L * L > = Pq - 1
    // Update: now passing in scriptP = (preprod - 1)/g and L1 = L/g, where g = gcd(Pqinv - 1, L)
    bool r_2divisors(bigint &preprod, long &q, bigint &L, bigint &L1, bigint &scriptP, bigint &g, bigint &Pqinv, vector<long> &rs);

    // use sieving to find r such that r = (Pq)^{-1} mod L, call korselt_check, those that pass go in rs
    // currently no attempt to deal with small L
    void r_sieving(bigint &preprod, long &q, bigint &L, bigint &L1, bigint &scriptP, bigint &g, bigint &Pqinv, vector<long> &rs);

    // Pinch's description of the r_sieving function.  Trial division, then sieving with stepsize L
    void pinch_r_sieving(bigint &preprod, long &q, bigint &L, bigint &L1, bigint &scriptP, bigint &g, bigint &Pqinv, vector<long> &rs);

    // Function which does all the inner loop work.  For given preproduct P of length d-1, finds r.
    // Current strategy: 1) check if only one potential r, check if at most small number of sieve steps,
    // then 2) attempt 2 divisors strategy, and if that fails do basic sieving.
    // Void function, fills the given vector with the rs found
    void inner_loop_work(bigint preprod, long q, bigint L, vector<long> &rs); 

};

#endif
