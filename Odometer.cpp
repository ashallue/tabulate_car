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

  store_divisors = false;
  num_divisors = 2;
  all_divisors = new int64[num_divisors];
  all_divisors[0] = 1;
  all_divisors[1] = 2;
  curr_div_index = 0;

  // default multiple we apply at the end is 1
  multiple = 1;
  // default initial div is 1 (times multiple)
  initial_div = 1;
}

// set the prime and powers
// by default, do the space-efficient version.  if storage flag turned to true,
// this constructor will calculate and store all divisors in an array
Odometer::Odometer(int64* ps, long* pows, long len, int64 given_multiple, bool storage){
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

  multiple = given_multiple;

  // calculate the initial divisor and store it.  set div to initial_div
  initial_div = 1 * multiple;
  div = initial_div;

  store_divisors = storage;
  // if flag is true, do the work to calculate and store all the divisors
  // otherwise, simply set 1 as the only divisor and ignore this array
  if(store_divisors){
    // calculate number of divisors from primes and powers
    num_divisors = 1;
    for(long i = 0; i < num_length; ++i){
      num_divisors *= (powers[i] + 1);

    }
    
    // allocate memory, call recursive function that does work
    all_divisors = new int64[num_divisors];
    // initially feed the array with 1 as the only divisor
    all_divisors[0] = initial_div;

    // now do recursive function, and update curr_div_index to the beginning
    create_divisors(0, 1);
    curr_div_index = 0;

    /*
    // multiply all divisors by the given multiple, but only if multiple is not 1
    if(multiple != 1){
      for(long i = 0; i < num_divisors; ++i){
        all_divisors[i] = all_divisors[i] * multiple;      
      }
    }
    */
    
  }else{
    all_divisors = new int64[1];
    all_divisors[0] = 1;
    num_divisors = 1;
    curr_div_index = 0;
  }
}

// destructor frees memory for primes, powers, div_exp
Odometer::~Odometer(){
  delete[] primes;
  delete[] powers;
  delete[] div_exp;
  delete[] all_divisors;
}

// rotate odometer, then update div
// if store_divisors set to true, simply move linearly to next divisor in the array
// So this function has two very different algorithms, depending on setup
void Odometer::next_div(){
  if(store_divisors){
    //testing
    //cout << "next_div, curr_div_index = " << curr_div_index << " and num_divisors = " << num_divisors << "\n";

    // if current index already at the end, rotate around to the beginning
    if(curr_div_index == num_divisors - 1){
      curr_div_index = 0;
      div = initial_div;
    }else{
      curr_div_index++;
      div = all_divisors[curr_div_index];
    }
  }else{
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
      div *= primes[curr_prime];
    }
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

/* Recursive helper function that calculates divisors
 * Rough idea: for every power of the prime at prime_index, multiply by all the existing divisors,
 * and add the new divisors to the array.  The array has memory allocated, just need to update index.
 */
void Odometer::create_divisors(long prime_index, long curr_position){
 
  // we are done if the curr_position matches the number of divisors and the prime_index matches length
  // Update: now it is possible the last prime is in must_have and has exp = 1, and thus should be skipped
  // So I am changing this to prime_index <= num_length
  if(prime_index <= num_length && curr_position == num_divisors){
    return;
  // it shouldn't happen that only one of those is true.  Check just to be safe
  // Update: now the second problem can't occur
  }else if(prime_index == num_length){
    cout << "Error in create_divisors, prime_index is maxed out without all divisors constructed\n";
  /*
  }else if(curr_position == num_divisors){
    cout << "Error in create_divisors, only curr_position is maxed out\n";
    cout << "curr_position = " << curr_position << " num_divisors = " << num_divisors;
    cout << " prime_index = " << prime_index << " num_length = " << num_length << "\n"; 
    cout << "primes are ";
    for(long i = 0; i < num_length; ++i){
      cout << primes[i] << " ";
    }
    cout << "and exps are ";
    for(long i = 0; i < num_length; ++i){
      cout << powers[i] << " ";
    }
    cout << "\n";
  */
  // work for the recursive call
  }else{

    // new position tells how many divisors have been computed so far
    long new_position = curr_position;

    // for every power of the prime p
    int64 p = primes[prime_index];
    int64 power = 1;
    long max_exp = powers[prime_index];

    // now, for every power of that prime up to the maximum, multiply by all existing divisors
    for(long i = 0; i < max_exp; ++i){
      // new prime power is p times the old one
      power *= p;

      // now multiply power by all the divisors and add to the array
      for(long j = 0; j < curr_position; ++j){
        all_divisors[new_position] = all_divisors[j] * power;
        new_position++;
      }
    } // end for prime powers 
    // make a recursive call with updated parameters
    create_divisors(prime_index+1, new_position);  
  }
  return;
};
