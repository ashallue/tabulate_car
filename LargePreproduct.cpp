/* Second attempt at large preproducts, this time using for loops.
 * I think this means that this same class also has to complete the Carmichael numbers.
 * Andrew Shallue, Spring 2023, part of Tabulating Carmichaels project
 */

#include "LargePreproduct.h"

// default values are B = 100,001 and X = B^{1/3}
LargePreproduct::LargePreproduct(){
  B = 1000001;
  double one_third = 1.0 / 3;
  X = ceil(pow(B, one_third));
  prime_B = ceil(pow(B, one_third));

  // 3 * 5 * 7 * 11 * 13 * 17 * 19 > 4.8 million, 3 * 5 * 7 * 11 * 13 * 17 = 255255
  max_d = 6;

  // create primes array.  First set up nums with nums[i] = i, then apply factor_sieve.
  // the largest prime is prime_B, so that is the upper bound on the array
  long nums[prime_B];
  for(long i = 0; i < prime_B; i++){
    nums[i] = i;
  }
  factor_sieve(nums, prime_B);

  // now copy over the primes to the primes array, starting with 3
  long* primes_initial = new long[prime_B];
  primes_count = primes_array_fromfs(nums, prime_B, primes_initial);

  primes = new long[primes_count];
  for(long i = 1; i < primes_count; ++i){
    primes[i-1] = primes_initial[i];
  }
  delete[] primes_initial;
  primes_count--;

  cout << "LargePreproduct object constructed with B = " << B << " and X = " << X << "\n";
  cout << "The prime constructed are ";
  for(long i = 0; i < primes_count; i++){
    cout << primes[i] << " ";
  }
  cout << "\n";
}

// constructor that takes B, X as input
LargePreproduct::LargePreproduct(bigint B_init, long X_init){
  B = B_init;
  X = X_init;

  // we have Xq < Xr < (Pq)r < B, and so q < B/X.  Thus B/X is the largest prime I'm constructing
  prime_B = B / X + 1;
 
  // create primes array.  First set up nums with nums[i] = i, then apply factor_sieve.
  // the largest prime is prime_B, so that is the upper bound on the array
  long nums[prime_B];
  for(long i = 0; i < prime_B; i++){
    nums[i] = i;
  }
  factor_sieve(nums, prime_B);

  // now copy over the primes to the primes array, starting with 3
  long* primes_initial = new long[prime_B];
  primes_count = primes_array_fromfs(nums, prime_B, primes_initial);

  primes = new long[primes_count];
  for(long i = 1; i < primes_count; ++i){
    primes[i-1] = primes_initial[i];
  }
  delete[] primes_initial;
  primes_count--;

  // compute max_d.  keep producting primes until bigger than B
  bigint prod = 1;
  // we will grab primes, starting with 3 at index 0
  max_d = 0;
  while(prod < B && max_d < primes_count){
    prod = prod * primes[max_d];
    max_d++;
  }
  // subtract one to get it back less than B
  if(max_d == primes_count){
    cout << "Problem in LargePreproduct constructor, computation of max_d hit end of primes array\n";
  }else{
    max_d--;
  }

  cout << "LargePreproduct object constructed with B = " << B << " and X = " << X << "\n";
  cout << "The prime constructed are ";
  for(long i = 0; i < primes_count; i++){
    cout << primes[i] << " ";
  }
  cout << "\n";
  cout << "max_d = " << max_d << "\n"; 
}


// Following rule of 3 to clean up the primes array
LargePreproduct::~LargePreproduct(){
  delete[] primes;
}

LargePreproduct& LargePreproduct::operator=(const LargePreproduct &other){
  cout << "Inside operator=\n";

  // copy over the basic variables
  this->B = other.B;
  this->X = other.X;
  this->max_d = other.max_d;
  this->prime_B = other.prime_B;

  // copy over the primes array
  this->primes_count = other.primes_count;
  this->primes = new long[this->primes_count];
  for(long i = 0; i < this->primes_count; ++i){
    this->primes[i] = other.primes[i];
  }

  // return the object constructed
  return *this;
}

LargePreproduct::LargePreproduct(const LargePreproduct &other){
  cout << "Inside copy constructor\n";

  // copy over the basic variables
  B = other.B;
  X = other.X;
  max_d = other.max_d;
  prime_B = other.prime_B;

  // copy over the primes array
  primes_count = other.primes_count;
  primes = new long[primes_count];
  for(long i = 0; i < primes_count; ++i){
    primes[i] = other.primes[i];
  }
}


