// Code written by Jonathan Sorenson

#ifndef _BIGINT
#define _BIGINT

#include <iostream>
#include <cstring>
#include <string>
#include "gmp.h"

using namespace std;

// bigint is an alternate name for 128-bit integers, i.e. two words considered as one
typedef int bigint __attribute__ ((__mode__ (__TI__)));
typedef bigint int128;

// Sometimes we need to convert from 128-bit words to two 64-bit words
// This union structure allows the two representations to occupy same spot in memory
union Dual_rep{
  bigint double_word;
  unsigned long int two_words[2];
};

void tostring(bigint x,char s[]);

void print(bigint x, ostream & os = cout);

bigint atobi(char tmp[]);

void read(bigint &x, istream &is = cin);

// Jonathan had these next three functions inline, but that wouldn't compile 
// on Hyperion for some reason, so I removed the inline keyword

ostream & operator<<(ostream & os, bigint x);

istream & operator>>(istream & is, bigint &x);

bigint min(bigint x, bigint y);

bigint extgcd128(bigint a, bigint b, bigint &x, bigint &y);

bigint inv128(bigint x, bigint m);

// These next functions are written by Andrew Shallue

// compute a^e mod n
// Uses mpz_t for the intermediate values, and uses gmp pow_mod
bigint pow_mod(bigint a, bigint e, bigint n);

/* A function that converts a string of digits into an integer of type bigint.
 * Performs a size check to make sure it is at most 128 bits
 */
bigint string_to_bigint(string num);

// convert from bigint n to mpz_t type m (passed by reference)
void bigint_to_mpz(bigint n, mpz_t &m);

// convert from mpz_t (passed by reference) to bigint.  Code same as that found in string_to_bigint
bigint mpz_to_bigint(mpz_t &m);

#endif
