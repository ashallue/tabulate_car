/* Class to tabulate Carmichael numbers Pqr with large pre-product P
 * Andrew Shallue, August 2022, project joint with Jonathan Webster
 */

#include "LargeP_Carmichael.h"


// For this basic constructor, note I am using brace initialization for od to cut down on the number of objects
// I could instead use the explicit construction and assignment for od; see commented line.
// Default values for B and X from LargeP_Odometer are B = 100001 and X = B^{1/3}
LargeP_Carmichael::LargeP_Carmichael() : od(){
  //od = LargeP_Odometer();
  B = od.B;
  X = od.X;

  qrs = vector<pair<int64, bigint>>();
  qrs.reserve(1000);
} 

// constructor that setse B, X, then creates Odometer based on those values
LargeP_Carmichael::LargeP_Carmichael(bigint B_init, long X_init) : od(B_init, X_init){
  B = B_init;
  X = X_init;
  //od = LargeP_Odometer(B, X);
  
  qrs = vector<pair<int64, bigint>>();
  qrs.reserve(1000);
  
}

// construct Carmichael numbers n < B of the form Pqr, with P >= X
// threaded by admissable pre-product constructed.  Output written to cars_file
void LargeP_Carmichael::pinch(long processor, long num_threads, string cars_file){
  cout << "Start of tabulate_car function \n";
  cout << "The Odometer has B = " << od.B << " and X = " << od.X << "\n";

  ofstream output;
  output.open(cars_file);

  bigint preprod;
  int64* preprod_primes;
  long   preprod_len; 
  int64  L;

  // keep a counter of admissable pre-products
  bigint admissable_counter = 0;

  /*
  for(long i = 0; i < 100; ++i){
    cout << "(" << od.get_P() << ", " << od.get_largest_prime() << ")" << " ";
    od.next();
  }
  cout << "\n";
  */

  int64* factors = new int64[od.max_d];

  // keep rotating odometer until we exceed the maximum allowable number of prime factors
  while(od.curr_d < od.max_d){
    // get current pre-product
    preprod = od.get_P();
    // create preproduct object with factorization
    long fac_len = od.get_Pfactors(factors);
    Preproduct preprod_ob = Preproduct(preprod, factors, fac_len);

    cout << "Preprod = " << preprod << " with factors: ";
    for(long i = 0; i < fac_len; ++i){
      cout << factors[i] << " ";
    }

    if(preprod_ob.admissable){
      cout << "is admissable\n";
      admissable_counter++;
    }else{
      cout << "is not admissable\n";
    }

    
    // next pre-product
    //
    od.next();
  }
  delete[] factors;


  output.close();
  return;
} 


/* For preproduct Pq, check divisors of Pq-1 up to trial_bound, for divisor f 
 * check whether r = (Pq - 1)/f + 1 completes the Carmichael.
 * If it does, put (q, r) as a pair into the built-in qrs vector
 */
void LargeP_Carmichael::r_search_trial(Preproduct P, int64 q, int64 bound){
  bigint Pq = P.Prod * q;

  // store prime divs in an array.  The maximum number of possible prime divs is log_2(Pq)
  double logPq = log(Pq) / log(2);
  long prime_divs_allocation = ceil(logPq);
  int64* prime_divs = new int64[ prime_divs_allocation ];
  long num_prime_divs = 0;

  // look for divisors of Pq - 1.  Stop if primes array exhausted, or exceeding bound
  // We will build divisors by first searching for prime divisors
  // Note that the odometer od has a large list of small primes called "primes" with length = prime_count
  long primes_index = 0;
  int64 div = od.primes[primes_index];
  while(primes_index < od.primes_count && div < bound && div < Pq){
    if( (Pq - 1) % div == 0){
      prime_divs[ num_prime_divs ] = div;
      num_prime_divs++;
    }

    // update primes_index and the div
    primes_index++;
    div = primes[primes_index];
  } // end while

  // We will use Odometer data structure to compute all divisors
  // compute exponents of the prime_divs
  bigint Pqminus_temp = Pq - 1;
  int64 prime_temp;
  long exp_temp; 

  long* exps = new long[ num_prime_divs ];
  for(long i = 0; i < num_prime_divs; ++i){
    prime_temp = prime_divs[i];
    exp_temp = 0;
    while( Pqminus_temp % prime_temp == 0 ){
      exp_temp++;
      Pqminus_temp /= prime_temp;
    }
    exps[i] = exp_temp;    
  }

  cout << "testing inside r_search_trial, Pq - 1 = " << Pq - 1 << "\n";
  cout << "primes are ";
  for(long i = 0; i < num_prime_divs; ++i){
    cout << prime_divs[i] << " ";
  }
  cout << " and exps are ";
  for(long i = 0; i < num_prime_divs; ++i){
    cout << exps[i] << " ";
  }
  cout << "\n";

  // create Odometer
  Odometer divs_od = Odometer(prime_divs, exps, num_prime_divs, 1, true);
  divs_od.next_div();
  div = divs_od.get_div();
 
  // variables needed for the loop over divisors 
  bigint r1;
  bigint r2;
  pair<int64, bigint> output_pair;

  // rotate through all of the divisors, keep the ones less than bound
  while(div != divs_od.initial_div){
    if(div < bound){
      // there are two r's to check, namely div + 1 and (Pq-1)/div + 1   
 
      // compute r values
      r1 = (Pq - 1) / div + 1;
      r2 = div + 1;

      // if they yield Carmichaels, write to qrs
      if(completion_korselt(P, q, r1)){
        pair<int64, bigint> output_pair;
        output_pair.first = q;
        output_pair.second = r1;
        qrs.push_back(output_pair);
      }

      if(completion_korselt(P, q, r2)){
        pair<int64, bigint> output_pair;
        output_pair.first = q;
        output_pair.second = r2;
        qrs.push_back(output_pair);
      }
    }
    // next div
    divs_od.next_div();
    div = divs_od.get_div();
  } // end while

}

/* For preproduct Pq, search for r in the arithmetic progression r = (Pq)^{-1} mod L
 * We assume r_search_trial has been applied up to bound, so we know r is smaller than (Pq-1)/bound  
   We assume that the lcm of L with q-1 is at most 64 bits.
 */
void LargeP_Carmichael::r_search_progression(Preproduct P, int64 q, int64 bound){

  int64 Pq = P.Prod * q;

  // first compute the new L value which includes q-1
  int64 L = P.L * (q-1);
  int64 g = gcd(P.L, q-1);
  L = L / g;

  // Now compute (Pq)^{-1} mod L with function found in int.h
  int64 target = inv(Pq, L);

  // loop over the arithmetic progression.  Steps of size L, target, up to (Pq) / bound
  for(int64 r = target; r <= Pq / bound; r = r + L){

  }

}
  
/* Completion check.  Returns true if Pqr is Carmichael.  
 * For this version, we know r-1 | Pq -1, so we check that Pqr satisfies Korselt, and that r is prime
 * Primality testing via mpz_probab_prime_p, an implementation of Baillie-PSW.
 */
bool LargeP_Carmichael::completion_korselt(Preproduct P, int64 q, bigint r){
  bool output = true;

  mpz_t r_mpz;      // mpz needed for primality testing
  mpz_init(r_mpz);
  int64 LCM = P.L;  // LCM of p-1 for p | P comes from the Preproduct class
  bigint modL, modqminus;
  Dual_rep d;
  bool is_r_prime;
  
  // check Korselt condition, i.e. Pqr = 1 mod lcm(L, q-1, r-1)
  // We only need to check mod L and mod q-1, b/c we know from assumption that r-1 | Pq - 1
  modL = ( (q % LCM) * (r % LCM) % LCM ) * (P.Prod % LCM) % LCM;
  modqminus = (r % (q-1)) * (P.Prod % (q-1)) % (q-1);
 
  // we need both of these congruent to 1
  output = output && (modL == 1) && (modqminus == 1);
  // if true at this point, check whether r is prime
  if(output){

    // use double work structure to convert to mpz
    d.double_word = r;

    mpz_set_si(r_mpz, d.two_words[1]);
    mpz_mul_2exp(r_mpz, r_mpz, 64);
    mpz_add_ui(r_mpz, r_mpz, d.two_words[0]);

    is_r_prime = mpz_probab_prime_p(r_mpz, 0) == 1;
    output = output && is_r_prime;
  }

  // clear mpz and return
  mpz_clear(r_mpz);
  return output;

}
