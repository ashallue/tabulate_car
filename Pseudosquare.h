/* Class that implements the pseudosquares primality test.
 * Stores the pseudosquares, has a primality test, and has functions
 * for various test procedures.
 *
 * Andrew Shallue, part of carmichael tabulation joint with Jonathan Webster
 * June 2022
 *
 * List of pseudosquares found at https://cr.yp.to/focus.html#focus-paper
 * At the current time, the table has 73 entries.
 * The largest is L_{373} = 4235025223080597503519329, an 82-bit number.
 * So this class can prove primality for numbers up to 81 bits.
 */

#include <vector>
#include <iostream>
#include "bigint.h"
#include "int.h"
#include "functions.h"
#include "primetest.h"

using namespace std;

#ifndef PSEUDOSQUARE_H
#define PSEUDOSQUARE_H

/* Implementation of pseudosquares primality test.
 * Source: Some Results on Pseudosquares, by Lukes, Patterson, Williams
 * Mathematics of Computation, Vol 65, Num 213, 1996, pages 361-372
 *
 */

class Pseudosquare{
  public:
    // There are 73 entries in the table of pseudosquares
    static const int num_entries = 73;
    // pseudosquares are of the form L_p = s
    // store index p in index_primes, store the s in pssquares
    int index_primes[num_entries];
    bigint pssquares[num_entries];

    // loads index_primes and pssquares with the appropriate values
    Pseudosquare();

    // computation of jacobi symbol (a | n).  Works for n composite.  
    // Source: Chap 5 of Bach/Shallit
    int jacobi(bigint a_in, bigint n_in); 

    // print primes with their corresponding L_p
    void print();

    // A function that tests if n is a pseudosquare with respect to p,
    // i.e. n = 1 mod 8 and (n | q) = 1 for all odd primes q <= p.
    // Does not check that n is the smallest integer with this property.
    bool is_pseudosquare(bigint n, long p);

    // Check all the pseudosquares in stored in the class arrays.  Output is all printed.
    // I am checking that is_pseudosquare returns true for the corresponding p, not for the next one
    // I am not checking that the n is the smallest with respect to that p.
    void check_pseudosquares();

    /* a primality proving algorithm, based on Theorem 2.7 of Lukes, Patterson, Williams.
     * If (1) all prime factors of N must exceed B, (2) N/B <= L_p, 
     * (3) p_i^ (N-1)/2 = +- 1 mod N for all primes p_i with 2 <= p_i <= p
     * (4) p_j^ (N-1)/2 = -1 mod N for some odd prime p_j <= p when N = 1 mod 8
     *     or 2^ (N-1)/2 = -1 mod N when N = 5 mod 8,
     * Then N is a prime or a power of a prime.
     */ 
    bool is_prime_pssquare(bigint n);
};

#endif
