/* Andrew Shallue, Tabulating Carmichaels project, 2022
 * Class for tabulating pre-products that are large
 */

using namespace std;

#include "LargeP_Odometer.h"

// For now, this constructor is focused on Carmichaels with 3 prime factors
LargeP_Odometer::LargeP_Odometer(){
  // let's focus on 3 prime factors
  curr_d = 3;
  P_len = 1;

  // For now, set B = 1000
  // Let's set preproduct lower bound X at B^{1/3}
  // I need primes up to B^{1/3}.
  B = 100001;
  double one_third = 1.0 / 3;
  X       = ceil(pow(B, one_third));
  prime_B = ceil(pow(B, one_third));

  // create the primes vector.  This involves setting up an array with nums[i] = i
  // then creating factor_sieve, then retrieving primes from that factor sieve
  long nums[prime_B];
  for(long i = 0; i < prime_B; i++){
    nums[i] = i;
  }
  factor_sieve(nums, prime_B);
  primes = primes_fromfs(nums, prime_B);
  
  //For the bounds, intialize first to all 0's
  for(long i = 0; i < max_d; ++i){
    uppers[i] = 0;
    lowers[i] = 0;
  }

  // for Carmichaels with 3 prime factors, initially all we know is the 
  // bounds on index 0.  Pre-product is prime, so must be greater than X
  uppers[0] = ceil(pow(B, one_third));
  lowers[0] = X;

  // initialize indices to all 0's
  for(long i = 0; i < max_d; ++i){
    indices[i] = 0;
  }
  // except that first index is the smallest prime greater than X
  indices[0] = find_index_lower(X);
  P_curr = primes[ indices[0] ];
}

// helper function.  Given lower bound, find index of the smallest prime larger than the bound
// Algorithm is binary search.  Return 0 if bound is greater than prime_B (corresponds to prime 2)
long LargeP_Odometer::find_index_lower(long bound){
  // testing
  /*
  cout << "This is find_index_lower with bound " << bound << "\n";
  cout << "The primes are \n";
  for(long i = 0; i < primes.size(); i++){
    cout << primes[i] << " ";
  }
  cout << "\n";
  */
  // check that bound is smaller than prime_B
  if(primes[primes.size()-1] < bound){
    //cout << "Error in find_index_lower, no prime above bound " << bound << "\n";
    return 0;
  }

  // start the search at the middle of the set of primes
  long curr_index = primes.size() / 2;
  long min = 0;
  long max = primes.size() - 1;
  long jump = (max - min) / 2;

  // continue until primes[curr_index] >= bound and primes[curr_index - 1] < bound 
  bool done = false;
  while(!done){
    // testing
    //cout << "min = " << min << " max = " << max << " curr_index = " << curr_index << "\n";
 
    // check if done
    if(primes[curr_index] >= bound && primes[curr_index - 1] < bound){
      done = true;
    }else{
      // if interval down to size 1, the answer should be one higher than current
      if( (max - min) == 1 ){
        curr_index++;
      }
      // now update to either the top half or bottom half
      else if(primes[curr_index] > bound){
        // jump down
        max = curr_index;
        jump = (max - min) / 2;
        curr_index -= jump;
      }else{
        // else jump up
        min = curr_index;
        jump = (max - min) / 2;
        curr_index += jump;
      }
    }
  } // end while

  // when the while loop is done, we have found our index
  return curr_index;
}

// retrieve current pre-product.  A simple getter function
bigint LargeP_Odometer::get_P(){
  return P_curr;
}
    
// Rotate the indices.  Starts with furthest right index, depending on P_len
void LargeP_Odometer::next(){
  return;
} 


