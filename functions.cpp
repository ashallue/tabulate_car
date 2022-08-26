/* Implementations corresponding to functions.h
  Andrew Shallue and Jonathan Webster
*/

#include "functions.h"

// print a vector<long>
void print_vec(vector<long> nums){
  for(long i = 0; i < nums.size(); ++i){
    cout << nums.at(i) << " ";
  }
  cout << "\n";
}

/* Given an array which is assumed to store integer i at position i, 
  and given a bound which is the size of the array,
  create a factor sieve, i.e. the array is transformed to 
  store the largest prime factor of index i
*/
void factor_sieve(long* nums, long B){
  // 0 and 1 store themselves
  nums[0] = 0;
  nums[1] = 1;

  // pivot refers to the current prime being sieved
  long pivot = 2;
  long index = 2;
  // outer loop is over primes up to sqrt(B)
  long prime_bound = ceil(sqrt(static_cast<double>(B)));

  // continue while the next prime is smaller than the bound
  while(pivot < prime_bound){
    
    // while smaller than the bound replace entry in index 
    // with the pivot.
    for(index = 2 * pivot; index < B; index = index + pivot){
      nums[index] = pivot;
    }

    // find the next prime.  It will be the first entry where 
    // the value equals the index
    ++pivot;
    while(nums[pivot] != pivot){
      ++pivot;
    }
  }
}

/* From a factor sieve, retrieve the primes, returned as a vector
*/
vector<long> primes_fromfs(long* sieved_nums, long B){
  vector<long> output;
  for(long i = 2; i < B; ++i){
    if(sieved_nums[i] == i){
      output.push_back(i);
    }
  }
  return(output);
}

/* From a factor sieve, compute primality of a given number.
n is prime if the largest factor in position n is n, 
composite otherwise.
*/
bool is_prime(long n, long* sieved_nums, long B){
  if(n >= B){
    cout << "n is too big for the given factored sieve\n";
    return false;
  }else{
     return (sieved_nums[n] == n);
  }
}

/* Returns true if n is a prime power, false if not.
 * The power part is trivial: take kth roots then kth powers to check.
 * The prime part is also trivial, from a factor sieve.
 * If I wanted to do better, Bernstein has a paper from 98 titled "Detecting perfect powers in essentially linear time".
 */
bool is_prime_power(long n, long* sieve_nums, long B){
  bool is_power = false;  // will flip to true if n is a power of some integer
  long root;              // will store the kth root
  long base;              // if n is a power of an int, use base

  // first do the power part: for k from 2 up to log_2(n), take kth root then kth power
  for(long k = 2; k < ceil( log2(n) ) + 1 ; ++k){
    root = floor( pow(n, 1.0 / k) );

    //cout << "For k = " << k << ", root = " << root << " and pow = " << pow(root, k);
    //cout << ".  The raw root is " << pow(n, 1.0 / k) << "\n";

    // note I do not break, because n might be a power of a smaller base
    // I already encountered a weird float problem with n = 125, so check root and root+1
    if(pow(root, k) == n){
      is_power = true;
      base = root;
    }
    if(pow(root + 1, k) == n){
      is_power = true;
      base = root + 1;
    }
  }

  // now we need to check whether base is prime or not.  Simply call is_prime
  return (is_power && is_prime(base, sieve_nums, B) );
}

/* Returns true if and only if n is a non-trivial power of an integer.
 * So returns false on input 0, input 1, and integers like 3 or 6.
 * Trivial algorithm: find roots using gmp function.
 */
bool is_power(bigint n){
  bool is_power = false;  // will flip to true if n is a power (at least 2) of some integer > 1

  // trivial cases
  if(n == 0 || n == 1) return false;
 
  // we will converg n to mpz_t, so we need to create and initialize variables
  mpz_t gn;  mpz_t base;
  mpz_init(gn);  mpz_init(base);
  bigint_to_mpz(n, gn);
 
  // now check if gn is a kth root.  mpz_root returns non-zero if this is true
  for(long k = 2; k < ceil( log2(n) ) + 1; ++k){
    if(mpz_root(base, gn, k) != 0){
      is_power = true;
      break;
    }
  } 

  // free memory and return the answer
  mpz_clear(gn); mpz_clear(base);
  return is_power;
}

/* From a factor sieve, return unique prime divisors of n
*/
vector<long> unique_prime_divs(long n, long* sieved_nums, long B){
  if(n > B){
    cout << "Error in unique_prime_divs, input larger than sieve bound\n";
  }

  vector<long> prime_divs = vector<long>();
  long curr_prime;

  // continue grabbing a prime and dividing n by it until we get 1
  while(n != 1){
    curr_prime = sieved_nums[n];
    prime_divs.push_back(curr_prime);
    
    // divide out all the factors of that prime
    while(n % curr_prime == 0){
      n = n / curr_prime;
    }
  }
  return prime_divs;
}

/* From a factor sieve, compute all divisors of n
*/
vector<long> divisors(long n, long* sieved_nums, long B){
  // bounds checking
  if(n >= B) cout << "Error in divisors, " << n << " is greater than " << B << "\n";

  // divs is the output.  cofactor will store the unfactored part of n
  vector<long> divs;
  divs.push_back(1);
  vector<long> divs_copy;
  vector<long> divs_temp = divs;
  long cofactor = n;
  long p;
  long p_pow;
  vector<long> p_powers;

  while(cofactor > 1){
    // capture the next prime, create list of its powers
    p = sieved_nums[cofactor];
    p_powers = vector<long>();
    p_pow = p;
    while(n % p_pow == 0){
      p_powers.push_back(p_pow);
      p_pow = p_pow * p;
    }
    // loop stops when p_pow is one step too big.  
    p_pow = p_pow / p;

    // for each prime power, mult divs by it and add to divs_temp
    for(long i = 0; i < p_powers.size(); ++i){
      divs_copy = divs;
      for(long j = 0; j < divs_copy.size(); ++j){
        divs_copy.at(j) = divs_copy.at(j) * p_powers.at(i);
      }
      // transfer over to divs_temp
      for(long j = 0; j < divs_copy.size(); ++j){
        divs_temp.push_back(divs_copy.at(j));
      }
    } // end for p_powers
    // now set divs to divs_temp, update cofactor
    divs = divs_temp;
    cofactor = cofactor / p_pow;

  }// end outer while
  return divs;
}

/* From a factor sieve, compute the product of primes up to X, where X < B
 */
bigint prime_product(long X, long* sieved_nums, long B){
  // check that X < B
  if(X >= B){
    cout << "Error in prime_product, X >= B\n";
  }else{
    // initialize output product
    bigint product = 1;
    // retrieve primes from the factor sieve
    vector<long> primes =  primes_fromfs(sieved_nums, B);
    
    // now product the primes up to X and return the answer
    long p_index = 0;
    while(primes.at(p_index) <= X){
      product = product * primes.at(p_index);
      p_index++;
    } // end while

    return product;
  }
}

/* Incremental sieve.
   Source: https://www.codevamping.com/2019/01/incremental-sieve-of-eratosthenes/
*/
vector<long> inc_sieve(long B){
  // output vector for primes.  Intialize with 2, but we will endeavor to ignore 2
  vector<long> primes;
  primes.push_back(2); primes.push_back(3);
  long prime_bound;
  bool is_prime;
  long multiple;    // stores current multiple of the current prime

  // loop over n up to B, primes up to sqrt(n)
  for(long n = 5; n < B; n += 2){
    is_prime = true;
    prime_bound = ceil(sqrt(static_cast<double>(n)));

    for(long i = 0; i < primes.size(); i++){
      long p = primes.at(i);

      multiple = 2 * p;
      // compute multiples of p, break if bigger than n
      while(multiple < n){
        multiple += p;
      }
      // if multiple == n, n composite and we break
      if(multiple == n){
        is_prime = false;
        break;
      }
      // if not, move on to check the next prime

    }
    // if at this point is hasn't been marked composite, mark as prime
    if(is_prime){
      primes.push_back(n);
    }
  }
  return primes;
} // end inc_sieve

// Given two vectors of integers, sort and then merge to remove duplicates
// Used to merge two lists of prime divisors
vector<int64> merge(vector<int64>& fst, vector<int64>& snd){
  // use built-in sort on fst and snd
  sort (fst.begin(), fst.end());
  sort (snd.begin(), snd.end());

  // now build the output vector
  vector<int64> output;

  // loop until at end of both lists
  vector<int64>::iterator fst_i = fst.begin();
  vector<int64>::iterator snd_i = snd.begin();
  while(fst_i != fst.end() && snd_i != snd.end()){

    // if fst is smaller, add that one
    if(*fst_i < *snd_i){
      output.push_back(*fst_i);
      fst_i++;
    // if snd is maller, add that one
    }else if(*snd_i < *fst_i){
      output.push_back(*snd_i);
      snd_i++;
    // otherwise they are the same, push one and advance both indices
    }else{
      output.push_back(*fst_i);
      fst_i++;
      snd_i++;
    }
  } // end while

  // now append the remainder of the other list
  if(fst_i == fst.end()){
    output.insert(output.end(), snd_i, snd.end());
  }else{
    output.insert(output.end(), fst_i, fst.end());
  }
  return output;
}

// another merge, but this time arrays.  Output is long, which represents the length of the output array, 
// instead the array is filled in with output parameter
long merge_array(int64* fst, long fst_len, int64* snd, long snd_len, int64* output){

  // use built-in sort on fst and snd
  sort(fst, fst + fst_len);
  sort(snd, snd + snd_len);

  // now loop over both arrays
  long fst_i = 0;
  long snd_i = 0;
  long output_i = 0;
 
  while(fst_i < fst_len && snd_i < snd_len){
  
    // if fst is smaller, add that one
    if(fst[fst_i] < snd[snd_i]){
      output[output_i] = fst[fst_i];
      fst_i++;
      output_i++;
    // if snd is smaller, add that one
    }else if(snd[snd_i] < fst[fst_i]){
      output[output_i] = snd[snd_i];
      snd_i++;
      output_i++;
    // otherwise they are the same, push one and advance both
    }else{
      output[output_i] = fst[fst_i];
      fst_i++;
      snd_i++;
      output_i++;
    }
  } // end while

  // now append the remainder of the other list
  if(fst_i == fst_len){
    for(snd_i; snd_i < snd_len; snd_i++){
      output[output_i] = snd[snd_i];
      output_i++;
    } 
  }else{
    for(fst_i; fst_i < fst_len; fst_i++){
      output[output_i] = fst[fst_i];
      output_i++;
    }
  }
 
  // return output_i as the output length 
  return output_i;
}
  

/* Tabulate all Carmichael numbers up to a bound B
 * Apply Korselt condition to each n.  Factorizations from factor sieve
 */
vector<long> trivial_car_tab(long B){
  vector<long> output;
   
  // create factor sieve
  long* sieve = new long[B];
  for(long i = 0; i < B; ++i){
    sieve[i] = i;
  } 

  factor_sieve(sieve, B);
  vector<long> n_primes;

  // variables for whether it is square-free, lcm, korselt
  bool squarefree;
  long lcm;
  bool korselt;
  long p_product;
  long g;

  // now loop over all n
  for(long n = 2; n < B; ++n){
    //cout << "Trivial tabulation, n = " << n << "\n";

    // use factor sieve to retrieve unique prime factors of n
    n_primes.clear();
    n_primes = unique_prime_divs(n, sieve, B);

    // Determine if n is squarefree by taking product of unique prime divs
    p_product = 1;
    for(long i = 0; i < n_primes.size(); ++i){
      p_product = p_product * n_primes.at(i);
    }
    squarefree = p_product == n;

    // now compute lcm_p (p-1).  Test if n = 1 mod lcm
    lcm = 1;
    for(long i = 0; i < n_primes.size(); ++i){
      g = gcd(lcm, n_primes.at(i) - 1);
      lcm = lcm * (n_primes.at(i) - 1) / g;  
    }
    korselt = n % lcm == 1;

    // if squarefree and composite and divisibility satisfied, add to vector
    if(squarefree && korselt && n_primes.size() >= 2) output.push_back(n);

  } // end for n

  return output;
}


/* This function returns the exponent e such that p^e || n.  If p does not divide n, returns 0.
 */
long exp_in_factorization(int64 p, int64 n){
  long exp = 0;
  int64 power = p;

  // continue while n mod power is 0
  while(n % power == 0){
    exp++;
    power *= p;
  }
  return exp;
}

