/* Class that stores information about a pre-product
 * Andrew Shallue, September 2022
 */

using namespace std;

#include "Preproduct.h"

Preproduct::Preproduct(int64* Pfac, long Pfac_len, int64* PMfac, long PMfac_len){
  // set length variables, then allocate memory for factor arrays
  Pprimes_len = Pfac_len;
  Pminus_len  = PMfac_len;
  Pprimes = new int64[Pprimes_len];
  Pminus  = new int64[Pminus_len];

  // copy over the factors
  for(long i = 0; i < Pprimes_len; ++i){
    Pprimes[i] = Pfac[i];
  }
  for(long i = 0; i < Pminus_len; ++i){
    Pminus[i]  = PMfac[i];
  }

  // compute L = lcm_{p | P} (p-1)
  

  // compute Tau, the divisor count of P-1
}

