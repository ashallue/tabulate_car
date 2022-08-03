/* Andrew Shallue, 2022, Tabulating Carmichael project
   implementation file for the odometer
*/

#include "Odometer.h"

using namespace std;

// default sets primes, powers to be empty vectors
Odometer::Odometer(){
  num_length = 1;
  primes = new int64[num_length];
  powers = new long[num_length];
  primes[0] = 2;
  powers[0] = 1;
  
  curr_prime = 0;
  curr_power = 0;
 
  div_exp = new long[num_length];
  div_exp[0] = 0;
  div = 1;
}

// set the prime and powers
Odometer::Odometer(int64* ps, long* pows, long len){
  num_length = len;
  
  // allocate memory for primes, powers
  primes = new int64[num_length];
  powers = new long[num_length];

  // copy over the elements
  for(long i = 0; i < num_length; ++i){
    primes[i] = ps[i];
    powers[i] = pows[i];
  }
  curr_prime = 0;
  curr_power = 0;
  
  // set div_exp as an array of 0's, corresponding to divisor 1
  div_exp = new long[num_length];
  for(long i = 0; i < num_length; ++i){
    div_exp[i] = 0;
  }
  div = 1;
}

// destructor frees memory for primes, powers, div_exp
Odometer::~Odometer(){
  delete[] primes;
  delete[] powers;
  delete[] div_exp;
}

// rotate odometer, then update div
void Odometer::next_div(){
  // stores prime power
  int64 prime_pow;

  // Find the least significant index where curr_power < max_power
  // Along the way, turn intermediate exps to 0
  curr_prime = 0;
  while(curr_prime < num_length && div_exp[curr_prime] == powers[curr_prime]){
    div_exp[curr_prime] = 0;

    // also update div.  Flipping exp to 0 corresponds to dividing div by p^exp
    prime_pow = pow( primes[curr_prime] , powers[curr_prime] );
    div = div / prime_pow;     

    // increment prime index
    curr_prime++;
  }

  // check whether the loop ended because we are at the end of the primes vector
  if (curr_prime == num_length){
    // if so, we have reset the div_exp vector to all 0's and div to 1, so we are done
    return;
  }
  // if we are not at the end of the div vector, add one at index curr_prime
  else{
    div_exp[curr_prime]++;

    // also multiply div by the prime at that index
    div = div * primes[curr_prime];
  }
  return;
}

// print div_exp
void Odometer::print_exp(){
  for(long i = 0; i < num_length; ++i){
    cout << div_exp[i] << " ";
  }
  cout << "\n";
}

// return divisor corresponding to div_exp
int64 Odometer::get_div(){
  return div;
}
