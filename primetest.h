// Code written by Jonathan Sorenson
// Prime tests for 16, 32, 64-bit numbers.
// Idea: smallest strong pseudoprime to the first n bases known, 
// pick n for the appropriate cutoff.  Issue with 64-bit numbers, see comments below.

#ifndef _PT
#define _PT

// functions for deterministic, unconditional prime testing
// for small-ish integers

#include "int.h"
#include "bigint.h"
#include <vector>

using namespace std;


// 32 and 64-bit strong pseudoprime test for the given base
// we have n-1=m*2^e with m odd
bool strong(int32 base, int32 n, int32 m, int16 e);

// fast test for 16-bit integers
// ψ2 = 1373653 > 2^15=32768
// so two strong ps tests will do it
bool primetest(int16 n);

// fast test for 32-bit integers
// ψ4 = 3215031751 > 2^31=2147483648
// so four strong ps tests will do it
bool primetest(int32 n);

const int64 primetestcutoff=3825123056546413051;

// test for 64-bit integers
// ψ9 = 3825 12305 65464 13051, below 2^63= 9223 37203 68547 75808,
// so we punt above this limit, for now
bool primetest(int64 n);

/* Pocklington test written by Andrew Shallue.  
   First applies strong test to the first 5 prime bases.  If composite, provably composite.
   If the number passes, attempts to prove it is prime.  This part could be slow, but is 
   expected to take polynomial time.
   Input is n along with the prime factors of n-1.
*/
bool pocklington(const int64 &n, const vector<int64> &primes);

/* Trial division by primes up to 1000.  
 * If output is false, provably composite.  If true, probably prime
 */
bool trial_thousand(const int64 &n);


#endif
