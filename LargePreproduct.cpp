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
  prime_B = ceil(pow(B, 1.0 / 2));

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

  // we have Xqq < Xqr < B, and so q < sqrt(B/X).  This is the upper bound 
  // at the high end.  At the low end, q could be as large as sqrt(B)
  // I've tested B / X and it works correctly: bigint / long casts to bigint
  // then storing it in prime_B (type long) is fine as long as sqrt is < 64 bits 
  double one_half = 1.0 / 2;
  prime_B = ceil(pow(B, one_half));
 
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

// helper function.  Given lower bound, find index of the smallest prime larger than the bound
// Algorithm is binary search.  Return -1 if bound is greater than prime_B (corresponds to prime 2)
long LargePreproduct::find_index_lower(long bound){
  // testing
  /*
  cout << "This is find_index_lower with bound " << bound << "\n";
  cout << "The primes are \n";
  for(long i = 0; i < primes.size(); i++){
    cout << primes[i] << " ";
  }
  cout << "\n";
  */
  // check that bound is smaller than prime_B, if not return -1
  if(primes[primes_count-1] < bound){
    //cout << "Error in find_index_lower, no prime above bound " << bound << "\n";
    return -1;
  }
  if(primes[0] > bound){
    //cout << "Error in find_index_lower, at bottom of the primes array\n";
    return primes[0];
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
  // return index found
  return curr_index;
}

// helper function.  Given num, den, root compute bound = (num / den)^(1/root), then find the index 
// of the largest prime smaller than that bound and return that index
// Update: deprecating this function.  Too complicated.
long LargePreproduct::find_index_upper(bigint num, bigint den, long root){
  // final check involves gmp exponentiation
  mpz_t bound_prod1;
  mpz_t bound_prod2;
  mpz_t big_den;
  mpz_t big_num;
  mpz_inits(bound_prod1, bound_prod2, big_den, big_num, 0);
  
  // convert num and den to mpz_t type for comparisons
  bigint_to_mpz(den, big_den);
  bigint_to_mpz(num, big_num);
  mpz_set_ui(big_den, 0);
  mpz_set_ui(big_num, 0);

  // compute the bound
  long bound = ceil(pow( num / den, 1.0 / root ));

  // make an index guess.  Since p = n * log(n), guess n = p / log(p)
  long guess = floor( bound / log(bound) );
  cout << "guess is " << guess << "\n";
  long output;  // will store the output to return

  // if guess is at the maximum of the primes list or above, simply take the maximum
  if(guess >= primes_count){
    output = primes_count;

  // if guess is less than 0, we assume nothing works, so output will be -1
  }else if(guess < 0){
    output = -1;
  
  // case for if the guess is too low
  }else if(primes[guess] < bound){

    // if guess is low, add one until guess is high
    while(primes[guess] < bound && guess < primes_count){
      guess++;
    }
   
    // if we hit the upper limit, simply return that
    if(guess == primes_count){
      cout << "problem in find_index_upper, while loop hit the upper limit\n";
      output = primes_count;
    
    // otherwise the correct result is probably guess--, check to make sure
    }else{
      // first compute the upper bound den * p2^root
      mpz_set_ui(bound_prod2, primes[guess]);
      mpz_pow_ui(bound_prod2, bound_prod2, root);
      mpz_mul(bound_prod2, bound_prod2, big_den);

      // now subtract 1 from guess and compute den * p1^root
      guess--;
      mpz_set_ui(bound_prod1, primes[guess]);
      mpz_pow_ui(bound_prod1, bound_prod1, root);
      mpz_mul(bound_prod1, bound_prod1, big_den);
      
      // we want bound_prod1 < num and bound_prod2 > num
      if(mpz_cmp(bound_prod1, big_num) < 0 && mpz_cmp(bound_prod2, big_num) >= 0){

        output = guess;

      }else{
        cout << "failure in find_index_upper, the guess found did not satisfy the bounds\n";
        output = -1;
      }
    }
  // case for if the guess is too big
  }else{
  
    // subtract one from guess until it is no longer too big
    while(primes[guess] >= bound && guess > 0){
      guess--;
    }
    // if we hit the lower limit, return -1 to signify failure
    if(guess == 0){
      cout << "problem in find_index_upper, while loop hit the lower limit\n";
      output = -1;

    //otherwise, guess should be what we want, compute two bounds to check 
    }else{

      // first compute the lower bound den * p1^root
      mpz_set_ui(bound_prod1, primes[guess]);
      mpz_pow_ui(bound_prod1, bound_prod1, root);
      mpz_mul(bound_prod1, bound_prod1, big_den);

      // now compute den * p2^root where p2 is prime above the guess
      mpz_set_ui(bound_prod2, primes[guess + 1]);
      mpz_pow_ui(bound_prod2, bound_prod2, root);
      mpz_mul(bound_prod2, bound_prod2, big_den);

      // we want bound_prod1 < num and bound_prod2 > num
      if(mpz_cmp(bound_prod1, big_num) < 0 && mpz_cmp(bound_prod2, big_num) >= 0){
        output = guess;
      }else{
        cout << "failure in find_index_upper, the guess found did not satisfy the bounds\n";
        output = -1;
      }
    }

  }
  // clear the mpz and return output
  mpz_clears(bound_prod1, bound_prod2, big_den, big_num, 0);
  return output;
}

// Helper function that returns (num / den)^(1/root) as an integer
// Note that a double should be enough precision: 128 / 3 < 43, 
// and a double is 64 bits total.
long LargePreproduct::find_upper(bigint num, bigint den, long root){
  // exponent is 1 / root
  double exp = 1.0 / root;
  // base is num / den
  long double base = num * 1.0 / den;

  // then use pow function to calculate result
  long double pow_result = pow(base, exp);

  // take the ceiling and return
  return ceil(pow_result);
}

// this one constructs Carmichaels with d = 4 and writes to file
void LargePreproduct::cars4(string cars_file){
  //setup file
  ofstream output;
  output.open(cars_file);

  // primes out of the primes index, their indices
  long p1, p2, q;
  long i1, i2, i3;
  // lower bounds are given in terms of index, uppers in terms of values
  long lower_index;
  long upper1, upper2, upper3;

  // nested for loops
  // compute first upper bound as B^{1/4}
  upper1 = find_upper(B, 1, 4);
  cout << "upper1 = " << upper1 << "\n";

  // start p1 at the first prime, which should be 3 
  i1 = 0;
  p1 = primes[0]; 
  while(p1 < upper1){


    // since p1 * p2 > X, and p2 > p1, we need to find i2 that makes both of these true
    lower_index = find_index_lower(X / p1);
    cout << "for p1 = " << p1 << " lower index found is " << lower_index << "\n";
    if(lower_index < i1 + 1) lower_index = i1 + 1;
    i2 = lower_index;  

    // also need to compute the corresponding upper bound: (B/p1)^{1/3}
    upper2 = find_upper(B, p1, 3);
    cout << "then lower_index = " << lower_index << " and upper2 = " << upper2 << "\n";

    p2 = primes[i2];
    // if no appropriate prime, i2 == -1
    while(i2 != -1 && p2 < upper2){
   
      // lower bound for q is just the previous prime, upper is (B/p1p2)^{1/2}
      upper3 = find_upper(B, p1 * p2, 2);
      cout << "For p2 = " << p2 << " upper bound is " << upper3 << "\n";

      i3 = i2 + 1;
      q = primes[i3];

      while(q < upper3){
        cout << "now find r for the preproduct " << p1 << " " << p2 << " " << q << "\n";
 
        // next prime
        i3++;
        q = primes[i3];
      } // end for i3

      // next prime
      i2++;
      p2 = primes[i2];
    } // end for i2

    // next prime
    i1++;
    p1 = primes[i1]; 
  } // end for i1

  output.close();
}
