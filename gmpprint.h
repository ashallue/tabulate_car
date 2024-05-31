/* Printing an mpz_t, i.e. multi-precision integer from GMP Bignum library.
 * Code due to Jonathan Sorenson.
 */

#ifndef _GMPPRINT
#define _GMPPRINT

#include <iostream>
#include "gmp.h"

// overload the << operator to deal with mpz_t

// allocate memory for a c-string that can fit 1000 digits
const int gmpprintstrlen = 1002;
char gmpprintstr[gmpprintstrlen];

ostream & operator<<(ostream & os, const mpz_t x){
  if(mpz_sizeinbase(x, 10) + 2 <= gmpprintstrlen){
    mpz_get_str(gmpprintstr, 10, x);
    os << gmpprintstr;
  }else{
    os << mpz_get_d(x);
  }

  return os;
}

#endif

