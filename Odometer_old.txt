/* Andrew Shallue, 2022, Tabulating Carmichael project
   implementation file for the odometer
*/

#include "Odometer.h"

using namespace std;

// default sets primes, powers to be empty vectors
Odometer::Odometer(){
  primes = vector<int64>();
  powers = vector<long>();
  
  curr_prime = 0;
  curr_power = 0;
 
  div_exp = vector<long>();
}

// set the prime and powers
Odometer::Odometer(vector<int64> ps, vector<int64> pows){
  primes = ps;
  powers = pows;

  curr_prime = 0;
  curr_power = 0;
  
  // set div_exp as an vector of 0's, corresponding to divisor 1
  div_exp = vector<long>();
  for(long i = 0; i < primes.size(); ++i){
    div_exp.push_back(0);
  }
}

// rotate odometer, then update div
void Odometer::next_div(){
  // Find the least significant index where curr_power < max_power
  // Along the way, turn intermediate exps to 0
  curr_prime = 0;
  while(curr_prime < primes.size() && div_exp.at(curr_prime) == powers.at(curr_prime)){
    div_exp.at(curr_prime) = 0;
    curr_prime++;
  }

  // check whether the loop ended because we are at the end of the primes vector
  if (curr_prime == primes.size()){
    // if so, we have reset the div_exp vector to all 0's, so we are done
    return;
  }
  // if we are not at the end of the div vector, add one at index curr_prime
  else{
    div_exp.at(curr_prime)++;
  }
  return;
}

// print div_exp
void Odometer::print_exp(){
  for(long i = 0; i < div_exp.size(); ++i){
    cout << div_exp.at(i) << " ";
  }
  cout << "\n";
}

// return divisor corresponding to div_exp
int64 Odometer::get_div(){
  int64 div = 1;

  // for every prime, up to its corresponding power, multiply by the prime
  for(long i = 0; i < div_exp.size(); ++i){
    for(long j = 0; j < div_exp.at(i); ++j){
      div *= primes.at(i);
    }
  }
  return div;
}