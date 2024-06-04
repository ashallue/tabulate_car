/* Printing an mpz_t, i.e. multi-precision integer from GMP Bignum library.
 * Code due to Jonathan Sorenson.
 */

#ifndef GMPPRINT
#define GMPPRINT

#include <fstream>
#include <iostream>
#include "gmp.h"
#include <vector>
#include "bigint.h"

using namespace std;

// overload the << operator to deal with mpz_t
ostream & operator<<(ostream & os, const mpz_t x);

// function that takes a string made up of strings of digits separated by spaces
// I assume that the first number is an arbitrarily long n, and the rest are smaller factors
void parse_mpz(string char_nums, mpz_t &n, vector<bigint> &factors);

#endif

