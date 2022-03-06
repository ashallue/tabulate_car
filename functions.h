/* Helper functions for constructing Carmichael numbers
  Andrew Shallue and Jonathan Webster
  Fall 2021
*/
#include <iostream>
#include <vector>
#include "math.h"
#include "int.h"
#include <algorithm>   // std::sort on vectors

using namespace std;

#ifndef FUNCTIONS
#define FUNCTIONS

// print a vector<long>
void print_vec(vector<long> nums);

/* Given an array which is assumed to store integer i at position i, 
  and given a bound which is the size of the array,
  create a factor sieve, i.e. the array is transformed to 
  store the largest prime factor of index i
*/
void factor_sieve(long* nums, long B);

/* From a factor sieve, retrieve the primes, returned as a vector
*/
vector<long> primes_fromfs(long* sieved_nums, long B);

/* From a factor sieve, compute primality of a given number
*/
bool is_prime(long n, long* sieved_nums, long B);

/* From a factor sieve, return unique prime divisors of n
*/
vector<long> unique_prime_divs(long n, long* sieved_nums, long B);

/* From a factor sieve, compute all divisors of n
*/
vector<long> divisors(long n, long* sieved_nums, long B);

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


#endif
