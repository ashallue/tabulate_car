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

istream & operator>>(istream & is, mpz_t &x){
  // allocate memory for a c-string that can fit 1000 digits
  const int gmplen = 1002;
  char gmpintstr[gmplen];

  // gather characters until a space or newline is read
  char c;
  is >> c;
  int index = 0;
  while(c != ' ' && c!= '\n'){
    gmpintstr[index] = c;
 
    // get the next character
    is >> c;
    index++;
  }
  // put an end character
  gmpintstr[index] = '\0';

  // now set the mpz and return the stream
  mpz_set_str(x, gmpintstr, 10);
  return is;

}

