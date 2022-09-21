/* Class that stores information about a pre-product
 * Andrew Shallue, September 2022
 */

using namespace std;

#include "Preproduct.h"

// default constructor
Preproduct::Preproduct(){
  Pprimes = nullptr;
  Pprimes_len = 0;
  Pminus = nullptr;
  Pminus_len = 0;
  L = 0;
  Tau = 0;
  P = 0;
  admissable = false;
}

// Given factors of P and P-1, create an object with new memory allocation.
// Note: assume product of the P factors is P, but factors of P-1 are the unique prime factors
Preproduct::Preproduct(int64 Pval, int64* Pfac, long Pfac_len, int64* PMfac, long PMfac_len){
  // set length variables, then allocate memory for factor arrays
  P = Pval;
  Pprimes_len = Pfac_len;
  Pminus_len  = PMfac_len;
  Pprimes = new int64[Pprimes_len];
  Pminus  = new int64[Pminus_len];

  // copy over the factors for both P and P-1
  // In the loop for P, also compute L = lcm_{p | P}(p-1) and P
  L = 1;
  int64 g;
  int64 prime;

  for(long i = 0; i < Pprimes_len; ++i){
    // copying over the factor
    Pprimes[i] = Pfac[i];
    prime = Pprimes[i];

    // in computing LCM, we can muliply by p-1 and divide by the gcd
    g = gcd(prime, L);
    L = L * (prime - 1) / g;
  }
  for(long i = 0; i < Pminus_len; ++i){
    Pminus[i]  = PMfac[i];
  }

  // compute Tau, the divisor count of P-1
  int64 Pminus_prod = P - 1;
  long div_count = 1;
  long exp_count = 0;

  // loop over the unique prime factors, compute exponent, then mult with the div count
  for(long i = 0; i < Pminus_len; ++i){
    exp_count = 0;
    while(Pminus_prod % Pminus[i] == 0){
      exp_count++;
      Pminus_prod /= Pminus[i];
    }
    div_count *= (exp_count + 1);
  }
  Tau = div_count;

  // set admissable bool by calling is_admissable
  admissable = is_admissable();

}

// Need a destructor to free the memory, then copy constructors for rule of 3
Preproduct::~Preproduct(){
  delete[] Pprimes;
  delete[] Pminus;
}

// copy constructor
Preproduct::Preproduct(const Preproduct& other){
  Pprimes_len = other.Pprimes_len;
  Pminus_len = other.Pminus_len;
  L = other.L;
  Tau = other.Tau;
  P = other.P;
  admissable = other.admissable;

  // allocate memory for the factor arrays
  Pprimes = new int64[Pprimes_len];
  for(long i = 0; i < Pprimes_len; ++i){
    Pprimes[i] = other.Pprimes[i]; 
  }
  Pminus = new int64[Pminus_len];
  for(long i = 0; i < Pminus_len; ++i){
    Pminus[i] = other.Pminus[i];
  }
}

// copy assignment operator
Preproduct Preproduct::operator=(const Preproduct& other){
  Preproduct result;

  // copy over non-array variables
  result.Pprimes_len = other.Pprimes_len;
  result.Pminus_len = other.Pminus_len;
  result.L = other.L;
  result.Tau = other.Tau;
  result.P = other.P;
  result.admissable = other.admissable;

  // allocate memory for the factor arrays
  result.Pprimes = new int64[result.Pprimes_len];
  for(long i = 0; i < result.Pprimes_len; ++i){
    result.Pprimes[i] = other.Pprimes[i];
  }
  result.Pminus = new int64[result.Pminus_len];
  for(long i = 0; i < result.Pminus_len; ++i){
    result.Pminus[i] = other.Pminus[i];
  }

  return result;
}

// Tests whether a given pre-product P is admissable, 
// i.e. that gcd(p-1, P) = 1 for all p | P, and squarefree
// returns 0 if not admissable
bool Preproduct::is_admissable(){
  // construct product of the prime factors of P
  // and compute lcm L at the same time
  int64 P_product = 1;
  int64 prime;    // stores a prime
  int64 g;        // stores gcd

  // note for this loop: let P_p be the divisor of P that is the product of primes up to p
  // Then gcd(p-1, P_p) = 1 for all p => gcd(p-1, P) = 1 for all p
  // because p-1 can't share factors with primes larger than p.
  for(long i = 0; i < Pprimes_len; ++i){
    // grow the product P
    P_product = P_product * Pprimes[i];

    // grow the LCM
    // first compute gcd(p-1, P)
    prime = Pprimes[i];
    g = gcd(prime - 1, P_product);

    // Not admissable if gcd not 1
    if(g != 1) return false;
  
  }  // end for over Pprimes

  // not squarefree if this product is not P
  if(P_product != P) return false;

  // if computer gets to this point then P is admissable
  return true;
}
