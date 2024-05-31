#include "gmpprint.h"


ostream & operator<<(ostream & os, const mpz_t x){
  // allocate memory for c-string that can fit 1000 digits
  const int gmpprintstrlen = 1002;
  char gmpprintstr[gmpprintstrlen];

  // if less than the length bound, get the string version and feed it to the stream
  if(mpz_sizeinbase(x, 10) + 2 <= gmpprintstrlen){
    mpz_get_str(gmpprintstr, 10, x);
    os << gmpprintstr;

  // otherwise truncate the mpz and print that
  }else{
    os << mpz_get_d(x);
  }

  return os;
}
