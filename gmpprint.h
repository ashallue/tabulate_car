/* Printing an mpz_t, i.e. multi-precision integer from GMP Bignum library.
 * Code due to Jonathan Sorenson.
 */

#ifndef GMPPRINT
#define GMPPRINT

#include <fstream>
#include <iostream>
#include "gmp.h"

using namespace std;

// overload the << operator to deal with mpz_t


ostream & operator<<(ostream & os, const mpz_t x);

istream & operator>>(istream & is, mpz_t &x);

#endif

