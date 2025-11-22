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

  // need primes up to sqrt(B/X).  For safeties sake, construct primes up to twice that bound
  prime_B = 2 * ceil(pow(B / X, 1.0 / 2));

  // magical number.  Idea is that if only small number of sieve steps, do that instead of complicated inner loop
  long small_sieve_steps = 50;

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

  // magical number.  Idea is that if only small number of sieve steps, do that instead of complicated inner loop
  long small_sieve_steps = 50;
  
  cout << "Constructing LargePreproduct object with B = " << B << " and X = " << X << "\n";

  // we have Xqq < Xqr < B, and so q < sqrt(B/X).  This is the upper bound 
  // at the high end.  If X is small, q could be as large as sqrt(B)
  // I've tested B / X and it works correctly: bigint / long casts to bigint
  // then storing it in prime_B (type long) is fine as long as sqrt is < 64 bits 
  double one_half = 1.0 / 2;
  
  // need primes up to sqrt(B/X).  For safeties sake, construct primes up to twice that bound
  prime_B = 2 * ceil(pow(B / X, one_half));

  cout << "prime_B = " << prime_B << "\n";
 
  // create primes array.  First set up nums with nums[i] = i, then apply factor_sieve.
  // the largest prime is prime_B, so that is the upper bound on the array
  long* nums = new long[prime_B];
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
  delete[] nums;
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
  /*
  cout << "The prime constructed are ";
  for(long i = 0; i < primes_count; i++){
    cout << primes[i] << " ";
  }
  cout << "\n";
  */
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
  this->small_sieve_steps = other.small_sieve_steps;

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
  small_sieve_steps = other.small_sieve_steps;

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
  if(primes[0] >= bound){
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
    if(primes[curr_index] > bound && primes[curr_index - 1] <= bound){
      done = true;
    }else{
      // if interval down to size 1, the answer should be one higher than current
      if( (max - min) == 1 ){
        curr_index++;
      }
      // now update to either the top half or bottom half
      else if(primes[curr_index] >= bound){
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
// I am using long double type, which has 64-bit mantissa and is written in scientific notation.
// Since 128/3 < 64, this should be fine.  However, I can only get it to print 23 bit mantissa.
// Not sure if it is a print issue or if I have a precision issue.
long LargePreproduct::find_upper(bigint num, bigint den, long root){
  // exponent is 1 / root
  double exp = 1.0 / root;
  // base is num / den
  // long double is in scientific notation, so result can be greater than 64 bits
  // I've tested that assignment will convert num from bigint to long double
  long double base = num;
  base = base / den;  

  // then use pow function to calculate result
  long double pow_result = pow(base, exp);

  // take the ceiling and return
  return ceil(pow_result);
}

// boolean function, checks Korselt condition, applies Baillie-PSW test to r
bool LargePreproduct::korselt_check(bigint Pq, bigint L, bigint r){
  // check bound
  if(Pq * r >= B){
    return false;
  }

  // check Korselt.  Sufficient to show that Pq * r = 1 mod lcm(L, r-1)
  // for the cases where r is constructed as (Pq)^{-1} mod L.
  // compute product modulo L and modulo r-1
  // Mult check: Pq, r, L all bigint, so ops are bigint.  Fits as long as L at most 64 bits.
  bigint modL = (Pq % L) * (r % L) % L;
  bigint modrminus = Pq % (r-1);
  if(modL != 1 || modrminus != 1) return false;

  // now for primality testing of r.  Convert it to type mpz, which requires Dual_rep
  mpz_t r_big;
  mpz_init(r_big);
  Dual_rep d;
  d.double_word = r;

  // set the high bits, multiply by 2**64, add the low bits
  mpz_set_si(r_big, d.two_words[1]);
  mpz_mul_2exp(r_big, r_big, 64);
  mpz_add_ui(r_big, r_big, d.two_words[0]);

  // now apply Baillie-PSW test. Implementation from GMP
  if(mpz_probab_prime_p(r_big, 0) == 0){
    return false;
  }

  // at this point, passed the test, return true;
  mpz_clear(r_big);
  return true;
}

// employ the two-divisors result for large L (Lemma 2.1 of Coppersmith, Howgrave-Graham, Nagaraj
// i.e. find divisors of (Pq - 1) congruent to (Pq)^{-1} - 1 mod L.  Requires gcd 1, 
// so division by a gcd is performed.  At most 2 divisors found, placed into rs vector.
// Returns boolean value, false if L too small for technique, true if L * L > = Pq - 1
bool LargePreproduct::r_2divisors(bigint &preprod, long &q, bigint &L, bigint &L1, bigint &scriptP, bigint &g, bigint &Pqinv, vector<long> &rs){

  // variables for the two rs constructed
  bigint fst_r, snd_r;  

  // calculate Pqinv = (Pq)^{-1} mod L, then g = gcd(inv - 1, L).  Never mind, now parameters
  //bigint Pqinv = inv128(preprod, L);
  //bigint g = gcd128(Pqinv - 1, L);
  //cout << "(Pq)^-1 = " << Pqinv << "\n";

  // if L1^2 < scriptP, return empty rs
  if(L1 * L1 < scriptP){
    return false;
  }else{
    // r1 = (Pq)^{-1} - 1 / g
    // have r1 * r2 = scriptP mod L1, so r2 = scriptP * r1^{-1} mod L1
    bigint r1 = (Pqinv - 1) / g;
    r1 = r1 % L1;
    // op check: scriptP, inv128 both bigint, so mult is bigint.
    // overflow check: could be a problem if L1 more than 64 bits.
    bigint r2 = (scriptP * inv128(r1, L1)) % L1;
   
    // the other divisor is then scriptP / r2.  We lift both via:  r = g * div + 1
    if(scriptP % r1 == 0){
      fst_r = r1 * g + 1;
     
      // include rs greater than q and passes korselt check
      if(fst_r > q && korselt_check(preprod, L, fst_r)){

        rs.push_back(fst_r);
      }
    }
    // check if r2 is a divisor
    if(scriptP % r2 == 0){
      snd_r = g * (scriptP / r2) + 1;
      if(snd_r > q && snd_r != fst_r && korselt_check(preprod, L, snd_r)){
        
        rs.push_back(snd_r);
      }
    }

    return true;
  }
}

// version 2 of Jonathan's r_sieving function
void LargePreproduct::r_sieving(bigint &preprod, long &q, bigint &L, bigint &L1, bigint &scriptP, bigint &g, bigint &Pqinv, vector<long> &rs)
{

  // this divisor is of the form Pqinv + k*L
  // r1 + k*L1 < sqrt( scriptP ) implies
  // g(r1 + k*L1) + 1 < g*sqrt( scriptP ) + 1
  double ub1 = g * sqrt(scriptP) + 1;
  // Pqinv + kL < B/(Pq)
  double ub2 = B / preprod;
  double ub = min( ub1, ub2 );
  bigint d = Pqinv;

  // Andrew note: <= is correct, accounts for integer div above.  Comparing bigint to double is weird.
  // the mantissa of a double is 53 bits
  while( d <= ub )
  {
    if(d > q && korselt_check(preprod, L, d))
    {
      rs.push_back(d);
    }
    d+= L;
  }

  // there is a divisor greater than sqrt( sriptP )
  // we find this "large" divisor by finding
  // a "small" factor f of Pq -1
  // then the divisor is d = (Pq-1)/f + 1
  if( ub1 < ub2 )
  {
    // find r2
    bigint r1 = ( Pqinv - 1 ) / g;
    bigint r2;

    // check if r1 is 0.  If 0, set r2 to be 1 as the co-divisor
    if(r1 == 0){
      r2 = 1;
    }else{
      r2 = ( inv128( r1, L1) * scriptP ) % L1;
    }

    // find the bound
    // r2 + k*L1 < sqrt( scriptP )
    double ub3 = sqrt( scriptP );
    // the bound (Pq-1)/( r2 + k*L1) < B/(Pq)
    // tells us the initialization place
    int k = ( (preprod - 1)*preprod ) / ( (B - preprod ) * L1 )  - 1 ;
    k = max(0, k);
    // initialize d to be of the correct size
    bigint f = r2 + k*L1;

    // Andrew note: another comparison of a bigint to a double
    while( f <= ub3 )
    {
      if( (preprod - 1) % f == 0)
      {
        d = (preprod - 1) / f + 1;
        if(d > q && korselt_check(preprod, L, d))
        {
          rs.push_back(d);
        }
      }
      f+= L1;
    }
  }
}

// we did a run where the while loops had < rather than <= and missed some Carmichaels
// This function deals with just the cases where we have equality
void LargePreproduct::equal_r_sieving(bigint &preprod, long &q, bigint &L, bigint &L1, bigint &scriptP, bigint &g, bigint &Pqinv, vector<long> &rs)
{

  // this divisor is of the form Pqinv + k*L
  // r1 + k*L1 < sqrt( scriptP ) implies
  // g(r1 + k*L1) + 1 < g*sqrt( scriptP ) + 1
  double ub1 = g*sqrt(scriptP) + 1;
  // Pqinv + kL < B/(Pq)
  double ub2 = B/preprod;
  double ub = min( ub1, ub2 );
  bigint d = Pqinv;

  // jump to end of the loop
  d = ub;

  if( d > q && korselt_check(preprod, L, d) ){
    rs.push_back(d);
  }


  // there is a divisor greater than sqrt( sriptP )
  // we find this "large" divisor by finding
  // a "small" factor f of Pq -1
  // then the divisor is d = (Pq-1)/f + 1
  if( ub1 < ub2 )
  {
    // find r2
    bigint r1 = ( Pqinv - 1 ) / g;
    bigint r2;

    // check if r1 is 0.  If 0, set r2 to be 1 as the co-divisor
    if(r1 == 0){
      r2 = 1;
    }else{
      r2 = ( inv128( r1, L1) * scriptP ) % L1;
    }

    // find the bound
    // r2 + k*L1 < sqrt( scriptP )
    double ub3 = sqrt( scriptP );
    // the bound (Pq-1)/( r2 + k*L1) < B/(Pq)
    // tells us the initialization place
    int k = ( (preprod - 1)*preprod ) / ( (B - preprod ) * L1 )  - 1 ;
    k = max(0, k);
    // initialize d to be of the correct size
    bigint f = r2 + k*L1;

    // jump to end of the loop
    f = ub3;

    if( (preprod - 1) % f == 0)
    {
      d = (preprod - 1) / f + 1;
      if(d > q && korselt_check(preprod, L, d))
      {
        rs.push_back(d);
      }
    }
      
  }
}



// use sieving to find r such that r = (Pq)^{-1} mod L, the ones that pass Korselt get placed in rs
// currently no attempt to deal with small L
// This is pinch's description: for large p we look for divisors of Pq-1, for small p we sieve stepsize L
void LargePreproduct::pinch_r_sieving(bigint &preprod, long &q, bigint &L, bigint &L1, bigint &scriptP, bigint &g, bigint &Pqinv, vector<long> &rs){
  
  bigint r1, r2;

  // using trial division up to sqrt(Pq / L), check for r-1 | Pq -1
  // stepsize is 1.  Note an innovation of S-W is to use stepsize L1 instead
  long division_bound = floor(sqrt(preprod / L));
  for(long d = 2; d <= division_bound; d++){
    if( (preprod - 1) % d == 0){

      // this gives two divisors, the other being scriptP * r1^-1 mod L1
      // r - 1 = d, so r = d + 1
      r1 = d + 1;
      r2 = (scriptP * inv128(r1, L1)) % L1;
      
      // check korselt and primality of r, if it passes add to rs vector
      if(r1 > q && korselt_check(preprod, L, r1)){
        rs.push_back(r1);
      }
      //if(r2 > q && korselt_check(preprod, L, r2)){
      //  rs.push_back(r2);
      //}
    }
  } // end for d

  // now sieve r = (Pq)^{-1} mod L up to (Pq - 1) / division_bound
  // we also know that max(q, division_bound) < r < B/Pq
  bigint sieve_lower = q;
  if(division_bound > sieve_lower) sieve_lower = division_bound;
  bigint sieve_upper = (preprod - 1) / division_bound;
  if(B / preprod < sieve_upper) sieve_upper = B / preprod;
 
  // compute (Pq)^{-1} mod L.  Never mind, passed in as parameter now
  //bigint Pqinv = inv128(preprod, L);
  
  // for starting point, want smallest int = (Pq)^{-1} greater than sieve_lower and greater than q
  // If we take the generic x = k * n + a > B, solution is k = floor( (B-a)/n ) + 1 
  
  // if sieve_lower = Pqinv - 1, it means that r = Pqinv found in division step, so start sieving step larger
  bigint k1 = 0;
  if( Pqinv <= sieve_lower + 1 ){

    k1 = (sieve_lower - Pqinv) / L + 1;  
  }

  // calculat the k value that makes starting point greater than q
  bigint k2 = 0;
  if( Pqinv <= q ){
    k2 = (q - Pqinv) / L + 1;
  }
  // set k to be the maximum of k1 and k2
  bigint k = k1;
  if(k < k2) k = k2;
 
  // now loop with stepsize L
  for(bigint r = k * L + Pqinv; r < sieve_upper; r += L){
    // if it passes korselt, add to rs vector
    if(r > q && korselt_check(preprod, L, r)){
      rs.push_back(r);
    }
  } // end of sieving loop
    

  
  // second try - only sieving
  // turns out this is slower than the code above
  //bigint Pqinv = inv128(preprod, L);
  //for(bigint r = Pqinv; r < preprod; r+= L){
  //  if(r > q && korselt_check(preprod, L, r)){
  //    rs.push_back(r);
  //  }
  //}
}


// Function which does all the inner loop work.  For given preproduct P of length d-1, finds r.
// Current strategy: 1) check if only one potential r, check if at most small number of sieve steps,
// then 2) attempt 2 divisors strategy, and if that fails do basic sieving.
// Void function, fills the given vector with the rs found
void LargePreproduct::inner_loop_work(bigint preprod, long q, bigint L, vector<long> &rs){ 

  // clear the rs vector and compute (Pq)^{-1} mod L
  rs.clear();
  bigint Pqinv = inv128(preprod, L);    
  bool twocheck;

  // compute g = gcd(Pqinv - 1, L), L1 = L / g, scriptP = (preprod - 1)/g.  These will be passed 
  // to the r2_divisors and r_sieving functions
  //bigint g = gcd128(Pqinv - 1, L);
  //bigint L1 = L / g;
  //bigint scriptP = (preprod - 1) / g;
  
  /* 
  // histogram calculation on scriptP vs L1
  if(L1 * L1 * L1 * L1 < scriptP){
    hist1++;
  }else if(L1 * L1 * L1 < scriptP){
    hist2++;
  }else if(L1 * L1 < scriptP){
    hist3++;
  }else{
    hist4++;
  }
  */

  // r is at most Pq - 1 and r is at most B / (Pq), so the number of sieve steps is bounded 
  // by the minimum of B / (Pq * L) and (Pq - 1) / L
  // Op check: all entities are bigint, so all ops are bigint 
  bigint small_sieve_bound = min( B / (L * preprod), (preprod - 1) / L ); 
  
  // if P * lambda(P) > B, we know that there is only one r to check, namely (Pq)^{-1}
  if(preprod * L > B){
    //count1++;

    // if car constructed < B, and (Pq)^{-1} is greater than q and car passes korselt check, add to list
    if(Pqinv * preprod < B && Pqinv > q && korselt_check(preprod, L, Pqinv)){
      rs.push_back(Pqinv);
    }
 
  // if c > count of sieve steps, we know there are only c r's to check
  }else if(small_sieve_steps > small_sieve_bound){
    //count2++;     

    // now loop with stepsize L
    // for the upper bound, note that r = B / preprod allowed because / is integer division
    for(bigint r = Pqinv; r <= min( B / preprod, preprod - 1 ); r += L){

      // if it passes korselt, add to rs vector
      if(r > q && korselt_check(preprod, L, r)){
        rs.push_back(r);
      }
    } // end of sieving loop

  // otherwise, use other techniques to find r
  // these will apply korselt check, and for rs that pass, they get pushed onto rs vector
  }else{
    //count3++;

    // compute g = gcd(Pqinv - 1, L), L1 = L / g, scriptP = (preprod - 1)/g.  These will be passed 
    // to the r2_divisors and r_sieving functions
    // I call these above, right, so these can be deleted? or other way around, delete the onese above?
    bigint g = gcd128(Pqinv - 1, L);
    bigint L1 = L / g;
    bigint scriptP = (preprod - 1) / g;

    /*   
    // histogram calculation on scriptP vs L1
    if(L1 * L1 * L1 * L1 < scriptP){
      hist1++;
    }else if(L1 * L1 * L1 < scriptP){
      hist2++;
    }else if(L1 * L1 < scriptP){
      hist3++;
    }else{
      hist4++;
    }
   */
    // first attempt the two divisor technique.  Works if L large enough
    twocheck = r_2divisors(preprod, q, L, L1, scriptP, g, Pqinv, rs);

    /*
    if(twocheck && preprod > 100000000000000){
       cout << "r_2divisors passed check with preprod = " << preprod << " q = " << q << "\n";
       count3++;
    }
    */
    // if it failed, do sieving instead       
    if(!twocheck){
      //count4++;
      r_sieving(preprod, q, L, L1, scriptP, g, Pqinv, rs);
    }    
  } // end else twocheck
  
}


// version that seeks to implement pinch's strategy
// Check if at most small number of sieve steps, then call pinch_r_sieving
void LargePreproduct::pinch_inner_loop_work(bigint preprod, long q, bigint L, vector<long> &rs){

  // clear the rs vector and compute (Pq)^{-1} mod L
  rs.clear();
  bigint Pqinv = inv128(preprod, L);    
  bool twocheck;


  
  // r is at most Pq - 1 and r is at most B / (Pq), so the number of sieve steps is bounded 
  // by the minimum of B / (Pq * L) and (Pq - 1) / L 
  bigint small_sieve_bound = min( B / (L * preprod), (preprod - 1) / L ); 
  
  // if P * lambda(P) > B, we know that there is only one r to check, namely (Pq)^{-1}
  if(preprod * L > B){
    count1++;

     //if car constructed < B, and (Pq)^{-1} is greater than q and car passes korselt check, add to list
     if(Pqinv * preprod < B && Pqinv > q && korselt_check(preprod, L, Pqinv)){
      rs.push_back(Pqinv);
     }
 
  
  // if c > count of sieve steps, we know there are only c r's to check
  }else if(small_sieve_steps > small_sieve_bound){
    //count2++;     

    // now loop with stepsize L
    for(bigint r = Pqinv; r < min( B / preprod, preprod - 1 ); r += L){

      // if it passes korselt, add to rs vector
      if(r > q && korselt_check(preprod, L, r)){
        rs.push_back(r);
      }
    } // end of sieving loop
  
  }else{
    //count3++;
  
    // compute g = gcd(Pqinv - 1, L), L1 = L / g, scriptP = (preprod - 1)/g.  These will be passed 
    // r_sieving functions
    bigint g = gcd128(Pqinv - 1, L);
    bigint L1 = L / g;
    bigint scriptP = (preprod - 1) / g;

    // call pinch_r_sieving to do trial division and larger sieve if needed
    //pinch_r_sieving(preprod, q, L, L1, scriptP, g, Pqinv, rs);
  
    pinch_r_sieving(preprod, q, L, L1, scriptP, g, Pqinv, rs);
  }
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

  // keep running computation of P and lcm_p|P p-1
  bigint P1, P2, P3;
  bigint L1, L2, L3;
  long g;
  bigint Pqinv;

  //vector for storing rs found
  vector<long> rs;

  // nested for loops
  // compute first upper bound as B^{1/4}
  upper1 = find_upper(B, 1, 4);
  //cout << "upper1 = " << upper1 << "\n";

  // start p1 at the first prime, which should be 3 
  i1 = 0;
  p1 = primes[0];
  P1 = p1;
  do{

    // compute L1
    L1 = p1 - 1;

    // since p1 * p2 > X, and p2 > p1, we need to find i2 that makes both of these true
    // this is equivalent to saying that p1 > sqrt(X) if and only if i2 = i1 + 1
    // Op check: X is long, p1 is long, so mult is long *
    if(p1 * p1 > X){
      lower_index = i1 + 1;
    }else{
      lower_index = find_index_lower(X / p1);
    }
    i2 = lower_index;  

    // also need to compute the corresponding upper bound: (B/p1)^{1/3}
    upper2 = find_upper(B, p1, 3);
    //cout << "then lower_index = " << lower_index << " and upper2 = " << upper2 << "\n";

    // finding the start index for p2 
    p2 = primes[i2];
    // check admissability, bump ahead until found
    while( gcd( p2 - 1, P1) != 1){
      i2++;
      p2 = primes[i2];
    }
    P2 = P1 * p2;
  
    do{
  
      //update L2
      L2 = L1 * (p2 - 1);
      g = gcd(L1, p2 - 1);
      L2 = L2 / g;
 
      // lower bound for q is just the previous prime, upper is (B/p1p2)^{1/2}
      upper3 = find_upper(B, P2, 2);
      //cout << "For p2 = " << p2 << " upper bound is " << upper3 << "\n";

      // finding the start index and prime for q
      i3 = i2 + 1;
      q = primes[i3];
      // check admissability, bump ahead until found
      while( gcd( q - 1, P2 ) != 1 ){
        i3++;
        q = primes[i3];
      }
      P3 = P2 * q;

      do{
        // update L3
        L3 = L2 * (q - 1);
        g = gcd(L2, q - 1);
        L3 = L3 / g;

        //cout << "now find r for the preproduct " << p1 << " " << p2 << " " << q << " with L = " << L3 << "\n";
        
        // complicated inner loop work that finds r's that make carmichaels
        // clears rs vector and refills it
        inner_loop_work(P3, q, L3, rs);
  
        // write to file
        for(long i = 0; i < rs.size(); i++){

          output << P3 * rs[i] << " ";
          output << p1 << " " << p2 << " " << q << " " << rs[i] << "\n";
        }
 
        // find next q that makes P2 * q admissable
        do{
          i3++;
          q = primes[i3];
        }while( gcd( q - 1, P2 ) != 1 );
        P3 = P2 * q; 
      
      } while(q < upper3); // end of do q

      // find next p2 that makes p1*p2 admissable
      do{
        i2++;
        p2 = primes[i2];
      }while( gcd( p2 - 1, P1 ) != 1 );
      P2 = P1 * p2;   
 
    }while(p2 < upper2);  // end of do p2

    // next prime p1
    i1++;
    p1 = primes[i1]; 
    P1 = p1;
  }while(p1 < upper1);  // end of do p1

  output.close();
}

// threaded version of cars4
void LargePreproduct::cars4_threaded(string cars_file, long thread, long num_threads){
  //setup file
  ofstream output;
  output.open(cars_file);

  // primes out of the primes index, their indices
  long p1, p2, q;
  long i1, i2, i3;
  // lower bounds are given in terms of index, uppers in terms of values
  long lower_index;
  long upper1, upper2, upper3;

  // keep running computation of P and lcm_p|P p-1
  bigint P1, P2, P3;
  bigint L1, L2, L3;
  long g;

  vector<long> rs;

  // nested for loops
  // compute first upper bound as B^{1/4}
  upper1 = find_upper(B, 1, 4);
  cout << "upper1 = " << upper1 << "\n";

  // start p1 at the prime corresponding to thread number 
  i1 = thread;
  p1 = primes[i1];
  P1 = p1;
  do{

    // compute L1
    L1 = p1 - 1;

    // since p1 * p2 > X, and p2 > p1, we need to find i2 that makes both of these true
    // this is equivalent to saying that p1 > sqrt(X) if and only if i2 = i1 + 1
    if(p1 * p1 > X){
      lower_index = i1 + 1;
    }else{
      lower_index = find_index_lower(X / p1);
    }
    i2 = lower_index;  

    // also need to compute the corresponding upper bound: (B/p1)^{1/3}
    upper2 = find_upper(B, p1, 3);
    //cout << "then lower_index = " << lower_index << " and upper2 = " << upper2 << "\n";

    // finding the start index for p2 
    p2 = primes[i2];
    // check admissability, bump ahead until found
    while( gcd( p2 - 1, P1) != 1){
      i2++;
      p2 = primes[i2];
    }
    P2 = P1 * p2;
  
    do{
  
      //update L2
      L2 = L1 * (p2 - 1);
      g = gcd(L1, p2 - 1);
      L2 = L2 / g;
 
      // lower bound for q is just the previous prime, upper is (B/p1p2)^{1/2}
      upper3 = find_upper(B, P2, 2);
      //cout << "For p2 = " << p2 << " upper bound is " << upper3 << "\n";

      // finding the start index and prime for q
      i3 = i2 + 1;
      q = primes[i3];
      // check admissability, bump ahead until found
      while( gcd( q - 1, P2 ) != 1 ){
        i3++;
        q = primes[i3];
      }
      P3 = P2 * q;

      do{
        // update L3
        L3 = L2 * (q - 1);
        g = gcd(L2, q - 1);
        L3 = L3 / g;

        //cout << "Inner loop with p1 = " << p1 << " p2 = " << p2 << " q = " << q << "\n";

        // complicated inner loop work that finds r's that make carmichaels
        // clears rs vector and refills it
        inner_loop_work(P3, q, L3, rs);
  
        // write to file
        for(long i = 0; i < rs.size(); i++){
          // note this next line might attempt to print a bigint and faile
          output << P3 * rs[i] << " ";
          output << p1 << " " << p2 << " " << q << " " << rs[i] << "\n";
        }
 
        // find next q that makes P2 * q admissable
        do{
          i3++;
          q = primes[i3];
        }while( gcd( q - 1, P2 ) != 1 );
        P3 = P2 * q; 
      
      } while(q < upper3); // end of do q

      // find next p2 that makes p1*p2 admissable
      do{
        i2++;
        p2 = primes[i2];
      }while( gcd( p2 - 1, P1 ) != 1 );
      P2 = P1 * p2;   
 
    }while(p2 < upper2);  // end of do p2

    // next prime p1
    i1 += num_threads;
    p1 = primes[i1]; 
    P1 = p1;
  }while(p1 < upper1);  // end of do p1

  output.close();
}

// faster generation of admissable preproducts
void LargePreproduct::cars4_threaded_modified(string cars_file, long thread, long num_threads){
  //setup file
  ofstream output;
  output.open(cars_file);

  // primes out of the primes index, their indices
  long p1, p2, q;
  long i1, i2, i3;
  // lower bounds are given in terms of index, uppers in terms of values
  long lower_index;
  long upper1, upper2, upper3;

  // keep running computation of P and lcm_p|P p-1
  bigint P1, P2, P3;
  bigint L1, L2, L3;
  long g;

  vector<long> rs;

  // nested for loops
  // compute first upper bound as B^{1/4}
  upper1 = find_upper(B, 1, 4);

  // start p1 at the prime corresponding to thread number 
  i1 = thread;
  p1 = primes[i1];
  P1 = p1;
  do{

    // compute L1
    L1 = p1 - 1;

    // since p1 * p2 > X, and p2 > p1, we need to find i2 that makes both of these true
    // this is equivalent to saying that p1 > sqrt(X) if and only if i2 = i1 + 1
    if(p1 * p1 > X){
      lower_index = i1 + 1;
    }else{
      lower_index = find_index_lower(X / p1);
    }
    i2 = lower_index;  

    // also need to compute the corresponding upper bound: (B/p1)^{1/3}
    upper2 = find_upper(B, p1, 3);
    //cout << "then lower_index = " << lower_index << " and upper2 = " << upper2 << "\n";

    // finding the start index for p2 
    p2 = primes[i2];
    // check admissability, bump ahead until found
    while( p2 % p1 == 1 ){ p2 = primes[ ++i2 ]; }
    P2 = P1 * p2;
    do{
  
      //update L2
      L2 = L1 * (p2 - 1);
      g = gcd(L1, p2 - 1);
      L2 = L2 / g;
 
      // lower bound for q is just the previous prime, upper is (B/p1p2)^{1/2}
      upper3 = find_upper(B, P2, 2);
      //cout << "For p2 = " << p2 << " upper bound is " << upper3 << "\n";

      // finding the start index and prime for q
      i3 = i2 + 1;
      q = primes[i3];
      // check admissability, bump ahead until found
      while( q % p1 == 1 || q % p2 == 1 ){ q = primes[ ++i3 ]; }
      P3 = P2 * q;

      do{
        // update L3
        L3 = L2 * (q - 1);
        g = gcd(L2, q - 1);
        L3 = L3 / g;

        //cout << "Inner loop with p1 = " << p1 << " p2 = " << p2 << " q = " << q << "\n";

        // complicated inner loop work that finds r's that make carmichaels
        // clears rs vector and refills it
        inner_loop_work(P3, q, L3, rs);
  
        // write to file
        for(long i = 0; i < rs.size(); i++){
          // note this next line might attempt to print a bigint and faile
          output << P3 * rs[i] << " ";
          output << p1 << " " << p2 << " " << q << " " << rs[i] << "\n";
        }
 
        // find next q that makes P2 * q admissable
        do{ q = primes[ ++i3 ]; } while( q % p1 == 1 || q % p2 == 1 );
        P3 = P2 * q; 
      
      } while(q < upper3); // end of do q

      // find next p2 that makes p1*p2 admissable
      do{ p2 = primes[ ++ i2 ]; } while( p2 % p1 == 1 );
      P2 = P1 * p2;   
 
    }while(p2 < upper2);  // end of do p2

    // next prime p1
    i1 += num_threads;
    p1 = primes[i1]; 
    P1 = p1;
  }while(p1 < upper1);  // end of do p1

  output.close();
}


//////////////////////////////////////////////////////////////////
// part of LargePreproduct.cpp found in a separate text file to include
#include "LP_extra.cpp"
///////////////////////////////////////////////////////////////

// recursive serial version of cars
void LargePreproduct::cars_rec(long d, string cars_file){
  vector<long> ps;

  //setup file
  ofstream output;
  output.open(cars_file);

  // call helper with initial P = 1, ps empty, L = 1
  cars_rec_helper(d, 1, ps, 1, output);

  output.close(); 
}

// threaded recursive version.  Has initial 3 nested loops, then calls cars_rec_helper
// requires d >= 6
void LargePreproduct::cars_rec_threaded(long d, string cars_file, long thread, long num_threads){
  if(d < 6){
    cout << "Error in cars_rec_threaded, d must be 6 or greater\n";
    return;
  }

  //setup file
  ofstream output;
  output.open(cars_file);

  // primes and indices for initial loops
  long p1, p2, p3;
  long i1, i2, i3;
  long upper1, upper2, upper3;

  // keep running computation of preproduct P, vector of indices making up primes in P, and lcm_p|P p-1
  bigint P1, P2, P3;
  bigint L1, L2, L3;
  vector<long> ps;
  long g;

  // threading based on number of admissable preproducts p1*p2*p3
  long num_admissable = 0;

  // first upper bound is B^{1/d}
  upper1 = find_upper(B, 1, d);

  // start p1 at 3
  i1 = 0;
  p1 = primes[i1];
  P1 = p1;
  do{
    // compute L1, initial p2, upper2 = (B/p1)^{1/(d-1)}
    L1 = p1 - 1;
    i2 = i1 + 1;
    p2 = primes[i2];
    upper2 = find_upper(B, p1, d-1);

    // check admissability, bump ahead until found
    while( gcd( p2-1, P1 ) != 1 ){
      i2++;
      p2 = primes[i2];
    }
    P2 = P1 * p2;
  
    // loop for p2
    do{

      // compute L2, initial p3, upper3 = (B/p1p2)^{1/(d-2)}
      L2 = L1 * (p2 - 1);
      g = gcd(L1, p2 - 1);
      L2 = L2 / g;

      i3 = i2 + 1;
      p3 = primes[i3];
      upper3 = find_upper(B, P2, d-2);

      // check admissability
      while( gcd(p3 - 1, P2) != 1){
        i3++;
        p3 = primes[i3];
      }
      P3 = P2 * p3;

      // loop for p3
      do{

        // check threading
        if(num_admissable % num_threads == thread){
          // compute current L3
          L3 = L2 * (p3 - 1);
          g = gcd(L2, p3 - 1);
          L3 = L3 / g;

          // now call recursive helper function for the rest of the nested loops
          // note we pass the indices of the primes p1, p2, p3
          ps.clear();
          ps.push_back(i1);  ps.push_back(i2);  ps.push_back(i3);
          cars_rec_helper(d, P3, ps, L3, output);

        } // end if correct threading

        // find next admissable p3
        do{
          i3++;
          p3 = primes[i3];
        }while( gcd( p3 - 1, P2 ) != 1);
        P3 = P2 * p3;
 
        num_admissable++;   // increment admissable counter

      }while(p3 < upper3); // end of do p3

      do{
        i2++;
        p2 = primes[i2];
      }while( gcd( p2-1, P1 ) != 1 );
      P2 = P1 * p2;

    }while(p2 < upper2); // end of do p2

    // next prime p1
    i1++;
    p1 = primes[i1];
    P1 = p1;
  }while(p1 < upper1); // end do p1

  output.close();

  // output counts from pinch_inner_loop_work
  //cout << "count1 = " << count1 << " count2 = " << count2 << " count3 = " << count3 << " count 4 = " << count4 << "\n";

  //cout << "hist1 = " << hist1 << " hist2 = " << hist2 << " hist3 = " << hist3 << " hist4 = " << hist4 << "\n";
}

// version with faster admissibility checks
void LargePreproduct::cars_rec_threaded_modified(long d, string cars_file, long thread, long num_threads){
  if(d < 6){
    cout << "Error in cars_rec_threaded, d must be 6 or greater\n";
    return;
  }

  //setup file
  ofstream output;
  output.open(cars_file);

  // primes and indices for initial loops
  long p1, p2, p3;
  long i1, i2, i3;
  long upper1, upper2, upper3;

  // keep running computation of preproduct P, vector of indices making up primes in P, and lcm_p|P p-1
  bigint P1, P2, P3;
  bigint L1, L2, L3;
  vector<long> ps;
  long g;

  // threading based on number of admissable preproducts p1*p2*p3
  long num_admissable = 0;

  // first upper bound is B^{1/d}
  upper1 = find_upper(B, 1, d);

  // start p1 at 3
  i1 = 0;
  p1 = primes[i1];
  P1 = p1;
  do{
    // compute L1, initial p2, upper2 = (B/p1)^{1/(d-1)}
    L1 = p1 - 1;
    i2 = i1 + 1;
    p2 = primes[i2];
    upper2 = find_upper(B, p1, d-1);

    // check admissability, bump ahead until found
    while( p2 % p1 == 1 ){ p2 = primes[ ++i2 ]; }
    P2 = P1 * p2;
  
    // loop for p2
    do{

      // compute L2, initial p3, upper3 = (B/p1p2)^{1/(d-2)}
      L2 = L1 * (p2 - 1);
      g = gcd(L1, p2 - 1);
      L2 = L2 / g;

      i3 = i2 + 1;
      p3 = primes[i3];
      upper3 = find_upper(B, P2, d-2);

      // check admissability
      while( p3 % p2 == 1 || p3 % p1 == 1 ){ p3 = primes[ ++i3 ]; }
      P3 = P2 * p3;

      // loop for p3
      do{

        // check threading
        if(num_admissable % num_threads == thread){
          // compute current L3
          L3 = L2 * (p3 - 1);
          g = gcd(L2, p3 - 1);
          L3 = L3 / g;

          // now call recursive helper function for the rest of the nested loops
          // note we pass the indices of the primes p1, p2, p3
          ps.clear();
          ps.push_back(i1);  ps.push_back(i2);  ps.push_back(i3);
          cars_rec_helper(d, P3, ps, L3, output);

        } // end if correct threading

        // find next admissable p3
        do{ p3 = primes[ ++i3 ]; } while( p3 % p1 == 1 || p3 % p2 == 1 );
        P3 = P2 * p3;
 
        num_admissable++;   // increment admissable counter

      }while(p3 < upper3); // end of do p3

      do{ p2 = primes[ ++i2 ]; } while( p2 % p1 == 1 );
      P2 = P1 * p2;

    }while(p2 < upper2); // end of do p2

    // next prime p1
    i1++;
    p1 = primes[i1];
    P1 = p1;
  }while(p1 < upper1); // end do p1

  output.close();

  // output counts from pinch_inner_loop_work
  //cout << "count1 = " << count1 << " count2 = " << count2 << " count3 = " << count3 << " count 4 = " << count4 << "\n";

  //cout << "hist1 = " << hist1 << " hist2 = " << hist2 << " hist3 = " << hist3 << " hist4 = " << hist4 << "\n";
}
    
// recursive version. This helper function tracks preproduct so far.  k is the factor count for preproduct, 
// while d is the number of factors in the final carmichael number
// Also, the vector of primes is actually a vector of indices that point to the corresponding primes
void LargePreproduct::cars_rec_helper(long d, bigint preprod, vector<long> &pis, bigint L, ofstream& output){

  // k is the number of factors in the preproduct
  long k = pis.size();
  long largest_index;
  bigint largest_prime;

  // pull out the largest prime and largest index, if k >= 1
  if(k >= 1){
    largest_index = pis[k-1];
    largest_prime = primes[largest_index];
  }

  // base case.  If d-1 == k, it means r is the current prime, so do inner loop work
  if(d - 1 == k){


    // q is the prime at the end of the ps vector
    long q = largest_prime;

    vector<long> rs;
    inner_loop_work(preprod, q, L, rs);

    // now print those carmichels to the file
    for(long i = 0; i < rs.size(); ++i){
      
      output << preprod * rs[i] << " ";
      for(long j = 0; j < pis.size(); ++j){
        output << primes[ pis[j] ] << " ";
      }
      output << rs[i];
      output << "\n";
    }
    return;

  // otherwise, calculate bounds and loop over all next primes
  }else{

    bigint new_L;
    bigint g;

    // lower and upper bounds on current prime
    long lower_bound, upper_bound; 
    // current_prime is the prime in the primes array corresponding to index
    long index;
    long current_prime;

    // calculate lower and upper bounds
    // Usually we just use the next prime on the lower end, so no lower bound.  Note can't have d-1 == k
    // if d-3 == k, P > X means lower bound is X/P, unless we know for sure P * next prime > X already
    // if d-2 == k, it means q is the current prime and we just use next prime
    // if preprod is 1, we are at the beginning, so set prime index to 0, which should be 3 
    if(preprod == 1){
      index = 0;
    }else if(d - 2 == k){
      index = largest_index + 1;
    }else if(d - 3 == k){
      if(preprod * largest_prime <= X){
        index = find_index_lower(X / preprod);
      }else{
        index = largest_index + 1;
      }
    }else{
      index = largest_index + 1;
    }

    // starting prime is then the prime at that index
    current_prime = primes[index];

    // upper bound is (B / preprod)^( 1 / (d - k) )
    upper_bound = ceil( pow ( B / preprod, 1.0 / (d - k) ) );
  
    //cout << "Upper bound = " << upper_bound << " and lower_bound = " << lower_bound << " and index = " << index << "\n";
 
    // check admissability, bump ahead until found
    while( gcd( current_prime - 1, preprod) != 1){
      index++;
      current_prime = primes[index];
    }
  
    // now loop.  Continue doing work until upper bound reached
    do{
      // compute new L based on the current prime   
      new_L = L * (current_prime - 1);
      g = gcd(L, current_prime - 1);
      new_L = new_L / g;

      // recursive call.  First add prime to the primes vector and update L
      pis.push_back(index);

      // Op check: preprod is bigint, current_prime is long, so what * is this?
      cars_rec_helper(d, preprod * current_prime, pis, new_L, output);

      // once the recursive call is finished, we pop the prime off in preparation for next one
      pis.pop_back();
    
      // move prime ahead until next admissable found
      do{
        index++;
        current_prime = primes[index];
      }while( gcd( current_prime - 1, preprod ) != 1 );    

    // this is the while part of the main do-while loop
    }while(current_prime < upper_bound);
  }
}
