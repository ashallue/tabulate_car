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

    cout << "Preprod = " << preprod << " ";
    for(long i = 0; i < fac_len; ++i){
      cout << factors[i] << " ";
    }
    cout << "\n";

    /*
    cout << "Preprod = " << preprod << " with factors: ";
    for(long i = 0; i < fac_len; ++i){
      cout << factors[i] << " ";
    }
    */

    if(preprod_ob.admissable){
      //cout << "is admissable\n";
      admissable_counter++;
      
      // clear the carmichaels corresponding to previous preproduct
      qrs.clear();
      preproduct_pinch(preprod_ob);

      //cout << "Preproduct = " << preprod << " , found " << qrs.size() << " many carmichaels\n";

      // print to file
      for(long j = 0; j < qrs.size(); ++j){
        output << preprod << " " << qrs.at(j).first << " " << qrs.at(j).second << "\n";
      }
 
    }else{
      //cout << "is not admissable\n";
    }

    
    // next pre-product
    //
    od.next();
  }
  // cleanup
  delete[] factors;
  qrs.clear();
  output.close();

  return;
} 

/* For a given preproduct, find all Carmichaels using method of Pinch.
 * Calls subroutines r_search_trial and r_searrch_progression
 * We assume the Carmichael constructed is less than B, and P > X
 */
void LargeP_Carmichael::preproduct_pinch(Preproduct& P){
  cout << "preproduct_pinch on " << P.Prod << " with admissable = " << P.admissable << "\n";

  // initialize mpz variable for primality on q
  mpz_t q_mpz;
  mpz_init(q_mpz);
  bool is_prime_q;
  bool is_admissable;
  int64 balance_point;  // balance between doing trial division and doing arithmetic progression
  int64 new_L;          // will hold lcm(P.L, q-1)

  // upper bound on q
  int64 q_bound = ceil( sqrt( B / P.Prod ) );
  
  // we want to loop over prime q in the range p_{d-2} < q < sqrt(B/P)
  for(int64 q = P.largest_prime() + 2; q < q_bound; q = q + 2){

    // code for checking primality of q via Baillie-PSW test
    mpz_set_si(q_mpz, q);
    is_prime_q = mpz_probab_prime_p(q_mpz, 0);
    if(is_prime_q){

      // now check that the addition of q means it is still admissable
      is_admissable = gcd(q-1, P.Prod) == 1;
      if(is_admissable){
     
        // compute balance point as D = sqrt(Pq / L)
        new_L = (P.L / gcd(P.L, q-1) ) * (q - 1);
        balance_point = floor( sqrt( P.Prod * q / new_L ) );
        // but have a minimum of 4, so we at least find factors of 2, 3
        if(balance_point < 4) balance_point = 4;

        // do trial division on Pq-1 up to the balance point, arithmetic progression above it
        r_search_trial(P, q, balance_point);
        r_search_progression(P, q, balance_point);

      } // end if admissable
    } // end if q prime
  } // end for q
  // clear q_mpz;
  mpz_clear(q_mpz);

}

/* For preproduct Pq, check divisors of Pq-1 up to trial_bound, for divisor f 
 * check whether r = (Pq - 1)/f + 1 completes the Carmichael.
 * If it does, put (q, r) as a pair into the built-in qrs vector
 */
void LargeP_Carmichael::r_search_trial(Preproduct P, int64 q, int64 bound){
  bigint Pq = P.Prod * q;

  //cout << "testing inside r_search_trial, Pq - 1 = " << Pq - 1 << " and bound is " << bound << "\n";
  
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

  // testing
  /*
  cout << "primes are ";
  for(long i = 0; i < num_prime_divs; ++i){
    cout << prime_divs[i] << " ";
  }
  cout << " and exps are ";
  for(long i = 0; i < num_prime_divs; ++i){
    cout << exps[i] << " ";
  }
  cout << "\n";
  */

  // create Odometer
  Odometer divs_od = Odometer(prime_divs, exps, num_prime_divs, 1, true);
  divs_od.next_div();
  div = divs_od.get_div();
 
  // variables needed for the loop over divisors 
  bigint r1;
  bigint r2;

  // testing purposes, count number of divisors checked
  //long loop_count = 0;

  // rotate through all of the divisors, keep the ones less than bound
  while(div != divs_od.initial_div){
    // testing
    //loop_count++;    

    if(div < bound){
      // there are two r's to check, namely div + 1 and (Pq-1)/div + 1   
 
      // compute r values
      r1 = (Pq - 1) / div + 1;
      r2 = div + 1;

      // if they yield Carmichaels, write to qrs
      if(q < r1 && completion_korselt(P, q, r1)){
        pair<int64, bigint> output_pair;
        output_pair.first = q;
        output_pair.second = r1;
        qrs.push_back(output_pair);
      }

      if(q < r2 && completion_korselt(P, q, r2)){
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
  //cout << "Number of while loop iterations = " << loop_count << " though not all under bound\n";

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
  // start value is the first int congruent to target mod L and greater than q
  int64 start = L * (q / L) + target;
  if(start <= q) start = start + L;

  // testing, count number of iterations
  //long loop_count = 0;

  // loop over the arithmetic progression.  Steps of size L, target, up to (Pq) / bound
  for(int64 r = start; r <= Pq / bound; r = r + L){
    // testing
    //loop_count++;

    // apply completion_divisible to check if r-1 | Pq - 1 and if r is prime
    if(completion_divisible(P, q, r)){
      pair<int64, bigint> output_pair;
      output_pair.first = q;
      output_pair.second = r;
      qrs.push_back(output_pair);
    }
  } // end for loop over the arithmetic progression
  
  //cout << "Number of steps in arithmetic progression = " << loop_count << "\n";
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

/* The completion check assumes r = (Pq)^{-1} mod L, so we know q-1 | Pqr - 1 
 * and p-1 | Pqr - 1 for p | P.  We still need to check that r-1 | Pqr - 1, and that r is prime
 */
bool LargeP_Carmichael::completion_divisible(Preproduct P, int64 q, bigint r){
  bool output = true;

  mpz_t r_mpz;   // mpz from gmp package, needed for primality testing
  mpz_init(r_mpz);
  int64 LCM = P.L;
  bigint modrminus;
  Dual_rep d;
  bool is_r_prime;

  // In this case for the korselt condition we only check if Pqr = 1 mod (r-1), meaning 
  // we only need to check if Pq = 1 mod (r-1)
  modrminus = (q * P.Prod) % (r-1);
  output = output && (modrminus == 1); 

  // if that passes, do primality
  if(output){
    d.double_word = r;
    mpz_set_si(r_mpz, d.two_words[1]);
    mpz_mul_2exp(r_mpz, r_mpz, 64);
    mpz_add_ui(r_mpz, r_mpz, d.two_words[0]);

    // primality test is Baillie-PSW from gmp
    is_r_prime = mpz_probab_prime_p(r_mpz, 0);
    output = output && is_r_prime;
  }
 
  mpz_clear(r_mpz);

  return output; 

}
