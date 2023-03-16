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
  Prod = 0;
  admissable = false;
}

// Given factors of P and P-1, create an object with new memory allocation.
// Note: assume product of the P factors is P, but factors of P-1 are the unique prime factors
Preproduct::Preproduct(int64 Pval, int64* Pfac, long Pfac_len, int64* PMfac, long PMfac_len){
  // set length variables, then allocate memory for factor arrays
  Prod = Pval;
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
    g = gcd(prime - 1, L);
    L = L * (prime - 1) / g;
  }
  for(long i = 0; i < Pminus_len; ++i){
    Pminus[i]  = PMfac[i];
  }

  // compute Tau, the divisor count of P-1
  int64 Pminus_prod = Prod - 1;
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

// For the large preproduct case, we don't necessarily have factorization of P-1.
// So this constructor only populates Pprimes, and only computes L
Preproduct::Preproduct(bigint Pval, int64* Pfac, long Pfac_len){

  // set length variables, then allocate memory for factor arrays
  Prod = Pval;
  Pprimes_len = Pfac_len;
  Pminus_len  = 0;
  Pprimes = new int64[Pprimes_len];
  Pminus  = nullptr;

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
    g = gcd(prime - 1, L);
    L = L * (prime - 1) / g;
  }

  // without factors of P-1, we can't compute Tau, so leave it at 1
  Tau = 1;

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
  Prod = other.Prod;
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
  result.Prod = other.Prod;
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
// Update: didn't work if a prime appears in Pprimes more than once, 
// so squarefree check now involves division.
bool Preproduct::is_admissable(){
  int64 prime;    // stores a prime
  int64 g;        // stores gcd

  // note for this loop: let P_p be the divisor of P that is the product of primes up to p
  // Then gcd(p-1, P_p) = 1 for all p => gcd(p-1, P) = 1 for all p
  // because p-1 can't share factors with primes larger than p.
  for(long i = 0; i < Pprimes_len; ++i){

    // grow the LCM
    // first compute gcd(p-1, P)
    prime = Pprimes[i];
    g = gcd(prime - 1, Prod);

    // Not admissable if gcd not 1
    if(g != 1) return false;
 
    // Not admissable if P / p == 0 mod p
    if((Prod / prime) % prime == 0) return false;
 
  }  // end for over Pprimes

  // if computer gets to this point then P is admissable
  return true;
}

// Given unique prime factors of P+D, compute the complete prime factorization of q = (P-1)(P+D)/2
// // All arrays are passed by reference.  Return value is the length of the q_primes, q_exps arrays.
long Preproduct::q_factorization(int64 q, int64* PplusD, long PplusD_len, int64* q_primes, long* q_exps){
  // new variable for q, which will get smaller as exponents computed.
  int64 q_temp = q;

  /*
  //testing
  cout << "before calling merge_array, we have q = " << q << " PplusD: ";
  for(long i = 0; i < PplusD_len; ++i){
    cout << PplusD[i] << " ";
  }
  cout << " and Pminus: ";
  for(long i = 0; i < Pminus_len; ++i){
    cout << Pminus[i] << " ";
  }
  cout << "\n";
  */

  // merge the P+D and P-1 prime factor arrays to remove duplicates
  long q_primes_len = merge_array(Pminus, Pminus_len, PplusD, PplusD_len, q_primes);

  /*
  // testing
  cout << "after calling merge_array we have q_primes: ";
  for(long i = 0; i < q_primes_len; ++i){
    cout << q_primes[i] << " ";
  }  
  cout << "\n";
  */

  long p, e;
  // loop over the prime divisors of q and compute the exponents
  // if the exponent is 0, don't include it
  // important invariant in the following loop: write_index always <= read_index
  long read_index = 0;
  long write_index = 0;
  long start_count = q_primes_len;

  while(read_index < start_count){
    p = q_primes[read_index];
    e = 0;
    while(q_temp % p == 0){
      e++;
      q_temp = q_temp / p;
    }
    
    // if e is non-zero, I want to include both p and e.  If 0, I don't
    // either way I need to re-write the prime, because things might have shifted
    if(e != 0){
      q_primes[write_index] = p;
      q_exps[write_index] = e;
      read_index++;
      write_index++;   
    }else{
      // if 0, don't write, and only shift the read_index, subtract 1 from length
      read_index++;
      q_primes_len--;
    }
  } // end while

  //return new length of the array
  return q_primes_len;
}

// return the largest prime dividing the preproduct.  Recall they are stored in increasing order.
int64 Preproduct::largest_prime(){
  // the last and largest prime is at position Pprimes_len - 1
  return Pprimes[ Pprimes_len - 1 ];
}
