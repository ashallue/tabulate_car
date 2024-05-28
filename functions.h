/* Helper functions for constructing Carmichael numbers
  Andrew Shallue and Jonathan Webster
  Fall 2021
*/

#ifndef FUNCTIONS
#define FUNCTIONS

#include <iostream>
#include <vector>
#include "math.h"
#include "int.h"
#include "bigint.h"
#include <string>
#include <algorithm>   // std::sort on vectors
#include "gmp.h"

using namespace std;


// print a vector<long>
void print_vec(vector<long> nums);
// print a vector<bigint>
void print_vec(vector<bigint> nums);

/* Given an array which is assumed to store integer i at position i, 
  and given a bound which is the size of the array,
  create a factor sieve, i.e. the array is transformed to 
  store the largest prime factor of index i
*/
void factor_sieve(long* nums, long B);

/* From a factor sieve, retrieve the primes, returned as a vector
*/
vector<long> primes_fromfs(long* sieved_nums, long B);

/* From a factor sieve, retrieve the primes, stored in the primes array.
 * The function returns the length, i.e. the number of primes found
 */
long primes_array_fromfs(long* sieved_nums, long B, long* primes);

/* From a factor sieve, compute primality of a given number
*/
bool is_prime(long n, long* sieved_nums, long B);

/* Returns true if n is a prime power, false if not.  Note primes are not prime powers for this function.
 * The power part is trivial: take kth roots then kth powers to check.
 * The prime part is also trivial, from a factor sieve.
 */
bool is_prime_power(long n, long* sieve_nums, long B);

/* Returns true if and only if n is a non-trivial power of an integer.
 * So returns false on input 0, input 1, and integers like 3 or 6.
 * Trivial algorithm: find roots using gmp function.
 */
bool is_power(bigint n);

/* From a factor sieve, return unique prime divisors of n
*/
vector<long> unique_prime_divs(long n, long* sieved_nums, long B);

/* From a factor sieve, compute all divisors of n
*/
vector<long> divisors(long n, long* sieved_nums, long B);

/* From a factor sieve, find unique prime divisors < B
 * Store them in the divisors parameter.  Return True if the cofactor is prime, False if not
 */
bool bounded_factor(bigint n, long* sieved_nums, long B, vector<long>& prime_divs);

/* From a factor sieve, compute the product of primes up to X, where X < B
 */
bigint prime_product(long X, long* sieved_nums, long B);

/* Incremental sieve.
   Source: https://www.codevamping.com/2019/01/incremental-sieve-of-eratosthenes/
   The input is a bound B, the output is the list of primes up to B.
   The complexity goal is O(B loglog B) time and O(B / log B) space.
*/
vector<long> inc_sieve(long B);

// Given two vectors of integers, sort and then merge to remove duplicates
// Used to merge two lists of prime divisors.
// Assumes the vectors have no repeats.
vector<int64> merge(vector<int64>& fst, vector<int64>& snd);

// same as previous merge, but now for arrays instead
// output is long, which represents length.  Values stored in parameter output
long merge_array(int64* fst, long fst_len, int64* snd, long snd_len, int64* output);

/* Tabulate all Carmichael numbers up to a bound B.
 * Apply Korselt condition to each n.  Factorizations from a factor sieve
 */
vector<long> trivial_car_tab(long B); 

/* This function returns the exponent e such that p^e || n.  If p does not divide n, returns 0.
 */
long exp_in_factorization(int64 p, int64 n);


#endif
