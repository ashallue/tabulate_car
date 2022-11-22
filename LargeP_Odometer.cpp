/* Andrew Shallue, Tabulating Carmichaels project, 2022
 * Class for tabulating pre-products that are large
 */

using namespace std;

#include "LargeP_Odometer.h"

// Sets up all the variables.  Default B = 100001 and X = B^{1/3}
LargeP_Odometer::LargeP_Odometer(){
  // let's start with 3 prime factors
  curr_d = 3;
  P_len = 1;

  // For now, set B = 1000
  // Let's set preproduct lower bound X at B^{1/3}
  // I need primes up to B^{1/3}.
  B = 1000001;
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
 
  // use factor sieve to create array of primes.  Start with one of size prime_B, 
  // then copy over to an array of the correct size
  long* primes_initial = new long[prime_B];
  primes_count = primes_array_fromfs(nums, prime_B, primes_initial);
  primes = new long[primes_count];
  for(long i = 0; i < primes_count; ++i){
    primes[i] = primes_initial[i];
  }
  delete[] primes_initial;
  
  // Calculate max_d.  
  bigint prod = 1;
  max_d = 1;  //start at p = 3;
  while(prod < B && max_d < prime_B){
    prod = prod * primes[max_d];
    max_d++;
  }
  // subtract one to get it back less than B
  max_d--;

  // allocate memory for arrays
  uppers = new long[max_d];
  lowers = new long[max_d];
  indices = new long[max_d];
 
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

  // but there might not be such primes, in which case we move on to length-2 pre-products
  if(indices[0] == 0) next_nextd();
}

// Constructor that takes B and X as input.  Calculates max_d, finds first pre-product
LargeP_Odometer::LargeP_Odometer(bigint B_init, long X_init){
  // let's start with 3 prime factors
  curr_d = 3;
  P_len = 1;

  // Let's set preproduct lower bound X at B^{1/3}
  // I need primes up to B^{1/3}.
  B = B_init;
  double one_third = 1.0 / 3;
  X       = X_init;
  prime_B = ceil(pow(B, one_third));

  // create the primes vector.  This involves setting up an array with nums[i] = i
  // then creating factor_sieve, then retrieving primes from that factor sieve
  long nums[prime_B];
  for(long i = 0; i < prime_B; i++){
    nums[i] = i;
  }
  factor_sieve(nums, prime_B);

  // use factor sieve to create array of primes.  Start with one of size prime_B, 
  // then copy over to an array of the correct size
  long* primes_initial = new long[prime_B];
  primes_count = primes_array_fromfs(nums, prime_B, primes_initial);
  primes = new long[primes_count];
  for(long i = 0; i < primes_count; ++i){
    primes[i] = primes_initial[i];
  }
  delete[] primes_initial;

  cout << "factor_sieve resulted in " << primes_count << " many primes: ";
  for(long i = 0; i < primes_count; ++i){
    cout << primes[i] << " ";
  }
  cout << "\n";
 
  // Calculate max_d.  
  bigint prod = 1;
  max_d = 1;  //start at p = 3;
  while(prod < B && max_d < prime_B){
    prod = prod * primes[max_d];
    max_d++;
  }
  // subtract one to get it back less than B
  max_d--;

  cout << "Computation of max_d: " << max_d << "\n";

  // allocate memory for arrays
  uppers = new long[max_d];
  lowers = new long[max_d];
  indices = new long[max_d];
 
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
  
  cout << "first index is " << indices[0] << " ";

  P_curr = primes[ indices[0] ];

  cout << "Which corresponds to the prime " << P_curr << "\n";

  // but there might not be such primes, in which case we move on to length-2 pre-products
  if(indices[0] == 0) next_nextd();

  cout << "initial indices array: ";
  for(long i = 0; i < max_d; ++i){
    cout << indices[i] << " ";
  }
  cout << "\n";

  cout << "curr_d is " << curr_d << " and P_curr is " << P_curr <<  " and P_len is " << P_len << "\n";
}

// destructor frees memory for the arrays
LargeP_Odometer::~LargeP_Odometer(){
  delete[] uppers;
  delete[] lowers;
  delete[] indices;
  delete[] primes;
}

// copy constructor
LargeP_Odometer::LargeP_Odometer(const LargeP_Odometer& other_od){
  // copy over all the data members
  max_d   = other_od.max_d;
  curr_d  = other_od.curr_d;
  P_len   = other_od.P_len;
  primes  = other_od.primes;   // this one is a vector, should do a deep copy
  P_curr  = other_od.P_curr;
  B       = other_od.B;
  X       = other_od.X;
  prime_B = other_od.prime_B;
  primes_count = other_od.primes_count;
 
  // allocate for the 4 arrays
  uppers = new long[max_d];
  lowers = new long[max_d];
  indices = new long[max_d];
  primes = new long[primes_count];

  // copy over the data for the arrays
  for(long i = 0; i < max_d; ++i){
    uppers[i] = other_od.uppers[i];
    lowers[i] = other_od.lowers[i];
    indices[i] = other_od.indices[i];
  }
  for(long i = 0; i < primes_count; ++i){
    primes[i] = other_od.primes[i];
  }
}

// assignment operator.  Very similar to copy constructor
LargeP_Odometer LargeP_Odometer::operator=(const LargeP_Odometer& other_od){
  // create new odometer
  LargeP_Odometer result_od = LargeP_Odometer();

  // copy over all the data members which are not arrays
  result_od.max_d = other_od.max_d;
  result_od.curr_d = other_od.curr_d;
  result_od.P_len = other_od.P_len;
  result_od.primes = other_od.primes;
  result_od.P_curr = other_od.P_curr;
  result_od.B = other_od.B;
  result_od.X = other_od.X;
  result_od.prime_B = other_od.prime_B;
  result_od.primes_count = other_od.primes_count;

  // allocate for the 4 arrays
  result_od.uppers = new long[result_od.max_d];
  result_od.lowers = new long[result_od.max_d];
  result_od.indices = new long[result_od.max_d];
  result_od.primes = new long[result_od.primes_count]; 

  // copy over the data for the arrays
  for(long i = 0; i < result_od.max_d; ++i){
    result_od.uppers[i] = other_od.uppers[i];
    result_od.lowers[i] = other_od.lowers[i];
    result_od.indices[i] = other_od.indices[i];
  }
  for(long i = 0; i < result_od.primes_count; ++i){
    result_od.primes[i] = other_od.primes[i];
  }
  
  return result_od;
}

// Construct odometer with given curr_d.  I only intend to use this as a helper, so primes not filled in.
LargeP_Odometer::LargeP_Odometer(long d){
  // set d and P_len
  curr_d = d;
  P_len = curr_d - 2;
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
  if(primes[primes_count-1] < bound){
    //cout << "Error in find_index_lower, no prime above bound " << bound << "\n";
    return 0;
  }

  // start the search at the middle of the set of primes
  long curr_index = primes_count / 2;
  long min = 0;
  long max = primes_count - 1;
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

// helper function.  Assuming we have finished pre-products for Carmichael numbers with current length,
// reset for the next length.  This involves updated variables and changing the indices array
void LargeP_Odometer::next_nextd(){
  // update curr_d and P_len
  curr_d++;   P_len++;

  // set primes up to P_len - 1 to be minimum possible, P_len prime is minimum given lower bound
  // Also calculate product of the pre-product up to that point, and update the lower bounds
  bigint prod = 1;
  for(long i = 0; i < P_len - 1; ++i){
    indices[i] = i+1;
    prod = prod * primes[ indices[i] ];
    lowers[i] =   primes[ indices[i] ];
  }
  // The final lower bound is either X / prod, or q_{P_len-2}, whichever is bigger
  long last_lower = X / prod ; 
  if(last_lower < primes[ indices[ P_len - 2] ]){
    indices[P_len - 1] = indices[P_len - 2] + 1;
  }else{
    long last_index = find_index_lower(last_lower);
    indices[P_len - 1] = last_index;
  }

  /* At this point there are two failure modes I can think of.
   * (1) prod * last_prime might be smaller than X, not >=.  This I can check and fix.
   * (2) indices[P_len - 1] = 0, meaning there was no prime in the set large enough.
   */
  // With the planned for bounds, I don't think (2) will happen.  Check just in case
  if(indices[P_len - 1] == 0){
    cout << "Problem in next_nextd(), no prime large enough to fill out pre-product\n";
  }else{
    // fix for (1): add 1 to index until pre-product is large enough
    while(primes[ indices[P_len - 1] ] * prod < X){
      indices[P_len - 1] = indices[P_len - 1] + 1;
    }
    // update last lower bound
    lowers[P_len - 1] = primes[ indices[P_len - 1] ];
  } 
  prod = 1; 
  long bound;
  // update the upper bounds.  (B / prod(i))^{1 / (d - i)}
  for(long i = 0; i < P_len; ++i){
    bound = ceil(pow( B/prod, 1.0 / (curr_d - i) ));
    uppers[i] = bound;
    prod = prod * primes[ indices[i] ]; 
  }
  // update the current pre-product
  P_curr = 1;
  for(long i = 0; i < P_len; ++i){
    P_curr = P_curr * primes[ indices[i] ];
  }

  return;
}

// retrieve current pre-product.  A simple getter function
bigint LargeP_Odometer::get_P(){
  return P_curr;
}
  
// retrieve the factorization of the current pre-product.
// Passed pointer gets the array of primes, the return value is its length
long LargeP_Odometer::get_Pfactors(int64* factors){
  for(long i = 0; i < P_len; ++i){
    factors[i] = primes[ indices[i] ];
    
  }
  return P_len;
}

// retrieve p_{d-2}, i.e. the largest prime dividing the pre-product
// we subtract 3 because the pre-product has two fewer primes than n does.
long LargeP_Odometer::get_largest_prime(){
  return primes[ indices[curr_d - 3] ];
}
  
// Rotate the indices.  Starts with furthest right index, depending on P_len
void LargeP_Odometer::next(){
  // index used to find the correct index to change.  Start at the furthest right
  long curr_index = P_len - 1;
  // next prime after the one at the current index
  long next_prime;
  // boolean value to identify when an index is maxed out
  bool max = false;
  // variable for calculating pre-products
  bigint prod;
  long bound;

  // work backwards from the last index.  Check if that index is maxed out already.
  // Reasons it could be maxed out:  (1) next prime is above upper bound, (2) curr index at end of array
  // I don't think curr index will be zero.  My plan is to check for that later in this function
  if(indices[curr_index] == (primes_count - 1)){
    max = true;
  }else{
    // now I know it is safe to grab the next prime
    next_prime = primes[ indices[curr_index] + 1 ];
    if(next_prime >= uppers[curr_index]) max = true;
  }
  // while maxed, subtract one from curr_index and check again
  while(max){
    curr_index--;

    // check if curr_index is now -1.  If so, we have finished this this curr_d, move on to next one
    if(curr_index == -1){
      max = false;
    
    // otherwise proceed with the same logic as before
    }else if(indices[curr_index] == (primes_count - 1)){
      max = true;
    }else{
      next_prime = primes[ indices[curr_index] + 1 ];
      if(next_prime >= uppers[curr_index]){
        max = true;
      }else{
        max = false;
      }
    }
  } // end while
  // at this point I have found the correct index to increment, or all indices maxed out
  // in the latter case, call next_nextd() to move on to Carmichaels with more prime factors
  if(curr_index == -1){
    next_nextd();
  }else{
    indices[curr_index]++;

    // if curr_index is P_len - 1, all we need to do at this point is update P_curr
    if(curr_index == P_len - 1){
      P_curr = P_curr / primes[ indices[curr_index] - 1 ];
      P_curr = P_curr * primes[ indices[curr_index]     ];
    }else{

      // calculate pre-product up to curr_index
      prod = 1; 
      for(long i = 0; i < curr_index + 1; ++i){
        prod = prod * primes[ indices[i] ];
      }
      // we now need to move forward and update lower and upper bounds 
      for(long i = curr_index + 1; i < P_len - 1; ++i){
        bound = ceil(pow( B/prod, 1.0 / (curr_d - i) ));
        uppers[i] = bound;
        lowers[i] = primes[ indices[i] + 1 ];

        // update the index: one more than the previous position
        indices[i] = indices[i-1] + 1; 
        prod = prod * primes[ indices[i] ];

      } // end for
      // update bounds on the final prime
      bound = ceil(pow( B/prod, 1.0 / 3 ));
      uppers[P_len - 1] = bound;
   
      // final lower bound is either X/prod or q_{P_len - 2}, whichever is bigger
      bound = X / prod;
      if(bound < primes[ indices[ P_len - 2 ] ]){
        indices[P_len - 1] = indices[P_len - 2] + 1;
      }else{
        long last_index = find_index_lower(bound);
        indices[P_len - 1] = last_index;
      }     
      lowers[P_len - 1] = primes[ indices[P_len - 1] ];
      
      // update pre-product
      P_curr = prod * primes[ indices[P_len - 1] ];

    } // end else  
  } // end else
  return;
} 

// print the entries of the indices array, the uppers array, and the lowers array
void LargeP_Odometer::print_info(){
  cout << "indices: ";
  for(long i = 0; i < max_d; ++i){
    cout << indices[i] << " ";
  }
  cout << "\n";
  cout << "upper bounds: ";
  for(long i = 0; i < max_d; ++i){
    cout << uppers[i] << " ";
  }
  cout << "\n";
  cout << "lower bounds: ";
  for(long i = 0; i < max_d; ++i){
    cout << lowers[i] << " ";
  }
  cout << "\n";
  return;
}

// repeatedly call next() to compute all pre-products and write them to a file.  
// Stops when the product of the first d primes is greater than B
void LargeP_Odometer::large_products(string filename){
  // set up the file
  ofstream output;
  output.open(filename);

  bigint preprod;
  // continue until current d value is greater than the max allowed
  while(curr_d < max_d){
    preprod = get_P();
    
    // write the preproduct along with its factors
    output << preprod << ": ";
    for(long i = 0; i < P_len; ++i){
      output << primes[ indices[i] ] << " ";
    }     
    output << "\n";
     
    // next pre-product
    next(); 
 
  } // end while
  output.close();
  return;
}

// A trivial algorithm for tabulating large pre-products.  Outputs P to a file given by filename
// For some reason this function works fine with B = million, but gives seg fault if B = 10 million
void LargeP_Odometer::trivial_large_products(string filename){

  // set up the flie
  ofstream output;
  output.open(filename);

  // set up the factor sieve
  long nums[B];
  for(long i = 0; i < B; i++){
    nums[i] = i;
  }
  factor_sieve(nums, B);

  vector<long> prime_factors;
  bool include_it;
  bigint prod;
  long biggest_prime;

  // n is a pre-product
  for(bigint n = 3; n < B; ++n){
    // factor n
    prime_factors = unique_prime_divs(n, nums, B);

    // check if square_free
    prod = 1;
    for(long i = 0; i < prime_factors.size(); ++i){
      prod *= prime_factors.at(i);
    }
    // keep it if it is squarefree, if it has at most max_d-2, factors is odd
    if(prod == n && prime_factors.size() <= max_d - 2 && n % 2 == 1){
      
      // find the biggest prime 
      biggest_prime = prime_factors.at(0);
      for(long i = 1; i < prime_factors.size(); ++i){
        if(prime_factors.at(i) > biggest_prime) biggest_prime = prime_factors.at(i);
      }

      // if the remaining pre-product is between X and P p_{d-2}^2 < B
      if(prod >= X && prod * biggest_prime * biggest_prime < B){
        output << prod << ": ";
        for(long i = 0; i < prime_factors.size(); ++i){
          output << prime_factors.at(i) << " ";
        }
        output << "\n";
      }
    }
  } 
  // close file
  output.close();
  return;
}

