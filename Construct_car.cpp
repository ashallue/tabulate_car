/* Implementation file for class that constructs 
Carmichael numbers.
  Andrew Shallue and Jonathan Webster

  This is a second attempt.  In my first attempt I treated P-1 and P+D as separate quantities.
  But this is a bad idea: I can end up with repeated divisors, when what I want is divisors of (P-1)(P+D).
  So in this attempt I will unify these and have a single set of divisors to loop over.

  Assumptions on sizes that apply throughout:
  pre-product P is at most 32bits, D is at most 32bits, so both fit into int64.
  importantly, the quantity (P-1)(P+D) also fits into int64
  q fits into an int64, but primality testing on q requires bigint (128 bits), which is handled by Sorenson
  r is larger than 64 bits, fits into bigint, but primality testing and proving requires 256-bit, so arbitrary precision
*/

#include "Construct_car.h"

using namespace std;

// default sets X to 2^(16) = 65536.
// Incremental sieve starts at 2
Construct_car::Construct_car(){
  X = 65536;
  B = 2 * X;

  // create Factgen2 object and initialize
  F = Factgen2();
  F.init(2, B);

  // create Factgen object for P+D
  // don't initialize now, need to do so later
  FD = Factgen();
}

// Tests whether a given pre-product P is admissable, 
// i.e. that gcd(p-1, P) = 1 for all p | P, and squarefree
// returns 0 if not admissable
int64 Construct_car::admissable(int64 P, int64* Pprimes, long Pprimes_len){
  // construct product of the prime factors of P
  // and compute lcm L at the same time
  int64 P_product = 1;
  int64 L = 1;
  int64 prime;    // stores a prime
  int64 g;        // stores gcd

  for(long i = 0; i < Pprimes_len; ++i){
    // grow the product P
    P_product = P_product * Pprimes[i];

    // grow the LCM
    // first compute gcd(p-1, P)
    prime = Pprimes[i];
    g = gcd(prime - 1, P_product);

    // Not admissable if gcd not 1
    if(g != 1){
      L = 0;
    }else{
      // otherwise, L = lcm(prevL, p-1)
      g = gcd(L, prime - 1);
      L = L * (prime - 1) / g; 
    }
  }  // end for over Pprimes

  // not squarefree if this product is not P
  if(P_product != P) return 0;

  // if at this point, P is admissable, return the LCM
  return L;
}

/* Given an admissible pre-product P, return all Carmichael numbers 
// up to X of the form Pqr where q and r are primes.
// Return is list of pairs (q, r) such that Pqr is Carmichael (except for primality test on r)
// Inputs are factorizations of P and P-1, along with L = lcm_{p | P} (p-1)
// This version not tied to F.  Would be slightly more efficient if it was.
*/
vector<pair<int64, bigint>> Construct_car::preproduct_construction(int64* P, long P_len, int64* Pminus, long Pminus_len, int64 L){
  vector<pair<int64, bigint>> output;
  int64 p;     // stores a temp prime
  long e;      // stores a temp exponent
  int64 Delta_bound;  // stores upper bound on Delta to ensure it isn't too big

  // Generate P as a single integer
  int64 P_product = 1;
  for(long i = 0; i < P_len; ++i){
    P_product = P_product * P[i];
  }
  int64 q_temp;  // will store (P-1)(P+D)/2
  int64 div = 1;     // will store divisors of (P-1)(P+D)/2

  // testing
  /*
  cout << "P_product: " << P_product << "\n";
  cout << "P_minus: ";
  for(long i = 0; i < Pminus_len; ++i){
    cout << Pminus[i] << " ";
  }
  cout << "\n";
  */

  // variables to help store and compute factorization of (P-1)(P+D)/2
  int64* PplusD;     // unique prime dividing P+D
  long PplusD_len;   // number of such primes
  int64* q_primes;   // unique primes dividing q_temp = (P-1)(P+D)/2
  long q_primes_len; // number of such primes
  long* q_exps;      // exponents of primes dividing q_temp
  pair<int64*, long> merge_output;

  // We need a Factgen object for factorizations of P+D
  // Initialize to match D in [2 .. P-1]
  FD.init(P_product + 2, 2 * P_product);

  // Basic loop structure: for all D in [2..(P-1)], for all divisors
  // of the expression (P-1)(P+D)/2, do stuff.
  for(int64 D = 2; D < P_product; ++D){

    // testing
    //cout << "P = " << P_product << " " << "D = " << D << "\n";

    // go to the next P+D through the Factgen object
    FD.next();

    // We set up an odometer, which requires primes and powers
    q_temp = (P_product - 1) * (P_product + D) / 2;

    // P_minus has the unique prime factors dividing P-1.
    // copy over prev into PplusD
    PplusD = FD.prev;
    PplusD_len = FD.prevlen;   
 
    // merge PplusD and Pminus to get prime factors of q_temp
    q_primes = new int64[PplusD_len + Pminus_len];
    q_primes_len = merge_array(Pminus, Pminus_len, PplusD, PplusD_len, q_primes);   
 
    q_exps = new long[q_primes_len];

    // loop over prime divisors of (P-1)(P+D) / 2, compute exponent  
    for(long i = 0; i < q_primes_len; ++i){
      p = q_primes[i];
      e = 0;
      while(q_temp % p == 0){
        e++;
        q_temp = q_temp / p;
      }
      q_exps[i] = e;
    }

    // Set up odometer to run through divisors of (P-1)(P+D)/2
    Odometer q_od = Odometer(q_primes, q_exps, q_primes_len);
    div = q_od.get_div();

    //cout << "div = " << div << "\n";

    // Run the code for divisor Delta = 1
    // apply completion check subroutine to see if this divisor Delta creates Carmichael
    pair<int64, bigint> qr = completion_check(P_product, div, D, L, q_primes, q_primes_len);
    //cout << "qr: " << qr.first << " " << qr.second << endl;
    if(qr.first != 0 && qr.second != 0){
      //cout << "Carmichael found in divs " << qr.first << " " << qr.second << "\n";
      output.push_back(qr);
    }
    // now go to next divisor
    q_od.next_div();
    div = q_od.get_div();
    
    // Throw out the divisor if it is too big.  It needs to be small enough so q is bigger than p_{d-2}.
    // The appropriate bound is Delta < (P-1)(P+D)/(p_{d-2}-1)
    // So we add 1 to the quotient, since integer division rounds down.
    Delta_bound = (P_product - 1) * (P_product + D);
    Delta_bound = Delta_bound / (P[P_len-1] - 1);
    
    // continue looking at divisors until it is back to 1
    while(div != 1){

      //cout << "div = " << div << "\n";

      if(div < Delta_bound){

        // apply completion check subroutine to see if this divisor Delta creates Carmichael
        pair<int64, bigint> qr = completion_check(P_product, div, D, L, q_primes, q_primes_len);
        //cout << "qr: " << qr.first << " " << qr.second << endl;
        if(qr.first != 0 && qr.second != 0){
          //cout << "Carmichael found in divs " << qr.first << " " << qr.second << "\n";
          output.push_back(qr);
        }
      } // end if div < Delta_bound
      
      // next iteration
      q_od.next_div();
      div = q_od.get_div();
    } //end while div != 1

    // free memory for q_primes and q_exps before next cycle
    delete[] q_primes;
    delete[] q_exps;

  } // end for D
  
  return output;
}

/* Once I have the loop over divisors of (P-1)(P+D)/Delta, I need to perform the following steps:
    1) Compute C = (P^2 + Delta)/D, check that it is integral
    2) Check Pqr for Korselt criterion
    3) check q and r for small prime factors (compositeness check)
    4) Compute q = (P-1)(P+D)/Delta + 1, check primality.  Don't check integrality, we know because div = Delta.
    5) Compute r = (P-1)(P+C)/Delta + 1, check integrality
  Note that I know q is integral, because Delta chosen as divisor of (P-1)(P+D).
  Other input: unique primes dividing (P-1)(P+D)/2, which we use to do primality via q-1 factorization.
  primality of q, r determined through mpz_probab_prime_p, a gmp func which does Baillie-PSW
  Returns a pair (q, r) if the completion works.  Returns (0, 0) if it doesn't
*/
pair<int64, bigint> Construct_car::completion_check(int64 P, int64 Delta, int64 D, int64 L, int64* q_primes, long q_primes_len){
  // setup output.  By default set it to (0, 0) which means false
  pair<int64, bigint> output;
  output.first = 0;   output.second = 0;

  // compute C and check that it is integral
  int64 intermediate;
  int64 C;

  // compute quotient and rem at the same time, compiler should only use 1 instruction
  intermediate = P * P + Delta;
  C = intermediate / D;
  bigint inter_rem = intermediate % D;

  // if rem is not 0, C not integral, return the (0,0) output pair
  if(inter_rem != 0){
    return output;
  }

  // compute q and check primality
  // Final value of q is (P-1)(P+D) / Delta
  int64 q;
  q = (P - 1) * (P + D);
  q = (q / Delta) + 1;

  // compute r, check it is integral.  Recall r = (P-1)(P+C)/Delta + 1
  bigint r, r_quo, r_rem; 
  r = (P - 1) * (P + C);
  r_quo = r / Delta;
  r_rem = r % Delta;

  if(r_rem != 0) return output;
  r = r_quo + 1;

  // check that Pqr satisfies Korselt criterion, i.e. Pqr = 1 mod lcm(L, q-1, r-1)
  // first compute product modulo L.  We work with reduced quantities since L is smaller than q, r
  bigint modL = ( (q % L) * (r % L) % L ) * (P % L) % L;
  bigint modqminus = (r % (q-1)) * (P % (q-1)) % (q-1);
  bigint modrminus = (q * P) % (r-1);
  if(modL != 1 || modqminus != 1 || modrminus != 1) return output;
  
  // primality testing on q, r.  Will use a gmp func, so requires conversion to mpz
  mpz_t q_big;
  mpz_t r_big;
  mpz_init(q_big); mpz_init(r_big); 

  // q can be converted directly.  r is possibly 128 bits, we need to use Dual_rep
  mpz_set_si(q_big, q);
  Dual_rep d;
  d.double_word = r;

  // Set the high bits, multiply by 2**64, then add over low bits
  // I think I have the si, ui correct here.  I tested it with examples where d.two_words[0] requires 64bits, 
  // and it worked fine.
  mpz_set_si(r_big, d.two_words[1]);
  mpz_mul_2exp(r_big, r_big, 64);
  mpz_add_ui(r_big, r_big, d.two_words[0]);

  /*
  // testing
  cout << "q in two different reps: " << q << " ";
  mpz_out_str(nullptr, 10, q_big);
  cout << "\n r in two different reps: " << r << " ";
  mpz_out_str(nullptr, 10, r_big);
  cout << "\n";
  */

  // check pseudo-primality of q, r.  mpz_probab_prime_p could return 0, 1, or 2.  0 for composite
  if(mpz_probab_prime_p(q_big, 0) == 0) return output;
  if(mpz_probab_prime_p(r_big, 0) == 0) return output;  

  // clear mpz
  mpz_clear(q_big);   mpz_clear(r_big);

  // If we have gotten to this point we have passed all the checks above
  output.first = q;   output.second = r;
  return output;
}

/* Construct Carmichaels for a range of pre-products P < B
 */
void Construct_car::tabulate_car(int64 B, long processor, long num_threads, string cars_file, string admissable_file){
  int64* P_factors;
  long   P_factors_len;
  int64* Pminus_factors;
  long   Pminus_factors_len;
  vector<pair<int64, bigint>> qrs;
  int64 LCM;

  // file stream object
  ofstream output;
  output.open(cars_file);

  ofstream admissable_w_zero;
  admissable_w_zero.open(admissable_file);

  // initialize the Factgen2 object that stores factorizations of P, P-1
  F.init(2, B);

  // count the number of admissable pre-products
  int64 num_admissable = 0;

  // Now loop over pre-products P
  for(int64 P = 3; P < B; ++P){

    // testing
    //cout << "tabulate_car, loops with P = " << P << "\n";

    // retrieve factorizations of P, P-1
    P_factors = F.current;
    P_factors_len = F.currentlen;
    Pminus_factors = F.prev;
    Pminus_factors_len = F.prevlen;

    // check admissability.  If so, add to count
    LCM = admissable(F.currentval, P_factors, P_factors_len);
    if(LCM != 0) num_admissable++;

    // if num_admissable has the correct residue, do work, otherwise continue
    if( (num_admissable % num_threads) != (processor % num_threads)){
      F.next();
    }else{

      // if admissable, construct Carmichaels
      if(LCM != 0){
     
        // Construct all Carmichael numbers with pre-product P
        qrs.clear();
        qrs = preproduct_crossover(P_factors, P_factors_len, Pminus_factors, Pminus_factors_len, LCM, false);

        // testing
       // cout << "P = " << P << " generates " << qrs.size() << " many carmichaels\n";

        // if no carmichales constructed, write pre-product to other file
        if(qrs.size() == 0){
          admissable_w_zero << F.currentval << "\n";
        }else{

          // print to file
          //output << "Carmichaels for P = " << P << " number of Cars is " << qrs.size() << "\n";
          for(long j = 0; j < qrs.size(); ++j){
            output << P << " " << qrs.at(j).first << " " << qrs.at(j).second << "\n";
          } // end for
        } // end else
      } // end if L != 0
      // move the factorization window
      F.next();

    } // end if correct processor
  } // end for P 

  // close file
  output.close();
  admissable_w_zero.close();
}

/* Construct Carmichaels for prime pre-products P
 */
void Construct_car::tabulate_car_primeP(int64 B, long processor, long num_threads, string cars_file){
  int64* P_factors;
  long   P_factors_len;
  int64* Pminus_factors;
  long   Pminus_factors_len;
  vector<pair<int64, bigint>> qrs;
  int64 LCM;

  // file stream object
  ofstream output;
  output.open(cars_file);

  // initialize Factgen2 object
  F.init(2, B);

  // count the number of prime pre-products
  int64 num_prime_P = 0;

  // loop over pre-products
  for(int64 P = 3; P < B; ++P){
    // check if P is prime.  If not, continue
    if(!F.isprime_current()){
      F.next();
    }else{
      //cout << "Found prime P = " << P << "\n";     

      // found a prime.  Increment count, set Pminus
      num_prime_P++;
      P_factors = F.current;
      P_factors_len = F.currentlen;
      Pminus_factors = F.prev;
      Pminus_factors_len = F.prevlen;

      // if P is prime, the LCM is just P-1
      LCM = P - 1;
      
      // if prime count in correct residue class, construct cars
      if(num_prime_P % num_threads != processor){
        F.next();
      }else{
        qrs.clear();
        qrs = preproduct_construction(P_factors, P_factors_len, Pminus_factors, Pminus_factors_len, LCM);

        // write to file
        for(long j = 0; j < qrs.size(); ++j){
          output << P << " " << qrs.at(j).first << " " << qrs.at(j).second << "\n";
        }
 
        // advance window
        F.next();
      }
   
    }// end if P prime

  } // end for P
  // close file
  output.close();
}

/* Version that calls the cross-over function
 */
void Construct_car::tabulate_car_primeP_crossover(int64 B, long processor, long num_threads, string cars_file, bool dynamic){
  int64* P_factors;
  long   P_factors_len;
  int64* Pminus_factors;
  long   Pminus_factors_len;
  vector<pair<int64, bigint>> qrs;
  int64 LCM;

  // file stream object
  ofstream output;
  output.open(cars_file);

  // initialize Factgen2 object
  F.init(2, B);

  // count the number of prime pre-products
  int64 num_prime_P = 0;

  // loop over pre-products
  for(int64 P = 3; P < B; ++P){
    // check if P is prime.  If not, continue
    if(!F.isprime_current()){
      F.next();
    }else{
      //cout << "Found prime P = " << P << "\n";     

      // found a prime.  Increment count, set Pminus
      num_prime_P++;
      P_factors = F.current;
      P_factors_len = F.currentlen;
      Pminus_factors = F.prev;
      Pminus_factors_len = F.prevlen;

      // if P is prime, the LCM is just P-1
      LCM = P - 1;
      
      // if prime count in correct residue class, construct cars
      if(num_prime_P % num_threads != processor){
        F.next();
      }else{
        qrs.clear();
        qrs = preproduct_crossover(P_factors, P_factors_len, Pminus_factors, Pminus_factors_len, LCM, dynamic);

        // write to file
        for(long j = 0; j < qrs.size(); ++j){
          output << P << " " << qrs.at(j).first << " " << qrs.at(j).second << "\n";
        }
 
        // advance window
        F.next();
      }
   
    }// end if P prime

  } // end for P
  // close file
  output.close();
}

/* Another version, but this one has the D crossover strategy
 * If the bool dynamic is true, we dynamically switch between strategies
 * If the bool is false, the crossover is set at P / (ln P)^2 
*/
vector<pair<int64, bigint>> Construct_car::preproduct_crossover(int64* P, long P_len, int64* Pminus, long Pminus_len, int64 L, bool dynamic){
  vector<pair<int64, bigint>> output;
  int64 p;     // stores a temp prime
  long e;      // stores a temp exponent
  int64 Delta_bound;  // stores upper bound on Delta to ensure it isn't too big

  // variables for primality testing, using gmp function so need to convert to mpz_t
  mpz_t q_big;
  mpz_t r_big;
  mpz_init(q_big);  mpz_init(r_big);
  
  // Generate P as a single integer
  int64 P_product = 1;
  for(long i = 0; i < P_len; ++i){
    P_product = P_product * P[i];
  }
  int64 q_temp;  // will store (P-1)(P+D)/2
  int64 div = 1;     // will store divisors of (P-1)(P+D)/2

  /*
  // testing 
  cout << "P_product: " << P_product << "\n";
  cout << "P_minus: ";
  for(long i = 0; i < Pminus_len; ++i){
    cout << Pminus[i] << " ";
  }
  cout << "\n";
  */

  // variables to help store and compute factorization of (P-1)(P+D)/2
  int64* PplusD;     // unique prime dividing P+D
  long PplusD_len;   // number of such primes
  int64* q_primes;   // unique primes dividing q_temp = (P-1)(P+D)/2
  long q_primes_len; // number of such primes
  long* q_exps;      // exponents of primes dividing q_temp
  pair<int64*, long> merge_output;

  // static crossover bound calculated at P / (ln P)^2
  int64 D_bound = P_product / ( floor(log(P_product)) * floor(log(P_product)) );
  
  //cout << "D_bound = " << D_bound << "\n";

  // For the dynamic version we need L_p, defined by 
  // P^2 + L_p = P^2 (p_{d-2} + 3)/(p_{d-2} + 1), where p_{d-2} is largest prime in P
  // So L_p = P^2 ( (p_{d-2} + 3)/(p_{d-2} + 1) - 1)
  int64 largest_p = P[P_len - 1];
  double term = (double) (largest_p + 3) / (largest_p + 1);
  int64 L_p = floor(P_product * P_product * ( term  - 1) ); 

  // constants needed for CD method
  bigint C_lower;
  bigint C_upper;
  int64 largestp = P[ P_len - 1 ];
  int64 q;
  bigint r;
  int64 Delta;
  // boolean values for whether q, r are integral, prime
  bool q_integral, r_integral;
  bool q_prime, r_prime;
  // reduced values for Korselt
  bigint modL, modqminus, modrminus;

  // We need a Factgen object for factorizations of P+D
  // Initialize to match D in [2 .. P-1]
  FD.init(P_product + 2, 2 * P_product);

  // count the number of times we do C-D
  int64 count_CD = 0;
  bool DDelta = true;

  // Basic loop structure: for all D in [2..(P-1)], for all divisors
  // of the expression (P-1)(P+D)/2, do stuff.
  for(int64 D = 2; D < P_product; ++D){
    // Start with code to decide whether to do D-Delta or C-D method
    // if count_CD > 20, do C-D.  20 chosen without investigation.
    if(count_CD > 20){
      DDelta = false;
    }else{
      // if we are in the DDelta method, then turn the FD incremental sieve
      if(DDelta){
        FD.next();
        // then FD.prev corresponds to P+D
        PplusD = FD.prev;
        PplusD_len = FD.prevlen;
      }
      // Code to determine when to switch from D-Delta to C-D
      // If not dynamic, switch when D > D_bound
      if(!dynamic){
        if(D > D_bound){
          DDelta = false;
        }
      }else{
        // in the dynamic case, switch if L_P / D is less than the count of divisors
        // We will approximate the count of divisors
        if( L_p / D < pow(2, PplusD_len + Pminus_len) ){
          DDelta = false;
        }
      }
    } // end deciding which method

    //cout << "D = " << D << " value of DDelta is " << DDelta << "\n";

    // if D is small, do the D-Delta method
    if(DDelta){

      // We set up an odometer, which requires primes and powers
      q_temp = (P_product - 1) * (P_product + D) / 2;

      // merge PplusD and Pminus to get prime factors of q_temp
      q_primes = new int64[PplusD_len + Pminus_len];
      q_primes_len = merge_array(Pminus, Pminus_len, PplusD, PplusD_len, q_primes);   
 
      q_exps = new long[q_primes_len];

      // loop over prime divisors of (P-1)(P+D) / 2, compute exponent  
      for(long i = 0; i < q_primes_len; ++i){
        p = q_primes[i];
        e = 0;
        while(q_temp % p == 0){
          e++;
          q_temp = q_temp / p;
        }
        q_exps[i] = e;
      }

      // Set up odometer to run through divisors of (P-1)(P+D)/2
      Odometer q_od = Odometer(q_primes, q_exps, q_primes_len);
      div = q_od.get_div();

      // Run the code for divisor Delta = 1
      // apply completion check subroutine to see if this divisor Delta creates Carmichael
      pair<int64, bigint> qr = completion_check(P_product, div, D, L, q_primes, q_primes_len);
      //cout << "qr: " << qr.first << " " << qr.second << endl;
      if(qr.first != 0 && qr.second != 0){
        //cout << "Carmichael found in divs " << qr.first << " " << qr.second << "\n";
        output.push_back(qr);
      }
      // now go to next divisor
      q_od.next_div();
      div = q_od.get_div();
    
      // Throw out the divisor if it is too big.  It needs to be small enough so q is bigger than p_{d-2}.
      // The appropriate bound is Delta < (P-1)(P+D)/(p_{d-2}-1)
      // So we add 1 to the quotient, since integer division rounds down.
      Delta_bound = (P_product - 1) * (P_product + D);
      Delta_bound = Delta_bound / (P[P_len-1] - 1);
    
      // continue looking at divisors until it is back to 1
      while(div != 1){

        //cout << "div = " << div << "\n";

        if(div < Delta_bound){

          // apply completion check subroutine to see if this divisor Delta creates Carmichael
          pair<int64, bigint> qr = completion_check(P_product, div, D, L, q_primes, q_primes_len);
          //cout << "qr: " << qr.first << " " << qr.second << endl;
          if(qr.first != 0 && qr.second != 0){
            //cout << "Carmichael found in divs " << qr.first << " " << qr.second << "\n";
            output.push_back(qr);
          }
        } // end if div < Delta_bound
      
        // next iteration
        q_od.next_div();
        div = q_od.get_div();
      } //end while div != 1

      // free memory for q_primes and q_exps before next cycle
      delete[] q_primes;
      delete[] q_exps;
    } // end if D small
    else{

      // Otherwise D is large, do CD method
      C_lower = 1 + (P_product * P_product) / D;
      C_upper = (P_product * P_product * (largestp + 3)) / (D * (largestp + 1));

      // turn counter
      count_CD++;

      // loop over C
      for(int64 C = C_lower; C <= C_upper; ++C){
       // compute Delta, along with numerators of q, r
       Delta = C * D - P_product * P_product;
       q = (P_product - 1) * (P_product + D);
       r = (P_product - 1) * (P_product + C);

       // Delta bound to ensure q > p_{d-2}
       int64 Delta_bound = (P_product - 1) * (P_product + D);
       Delta_bound = Delta_bound / (largestp - 1);

       // test q, r for integrality
       q_integral = q % Delta == 0;
       r_integral = r % Delta == 0;

       // only do further work if both are integral, Delta small enough
       if(q_integral && r_integral && Delta < Delta_bound){
         q = 1 + q / Delta;
         r = 1 + r / Delta;

         //cout << "before mpz stuff\n";

         // now primality testing; mpz_probab_prime_p does trial-division, then Baillie-PSW
         // primality testing on q, r.  Will use a gmp func, so requires conversion to mpz
         // q can be converted directly.  r is possibly 128 bits, we need to use Dual_rep
         mpz_set_si(q_big, q);
         Dual_rep d;
         d.double_word = r;

         // set the high bits, multiply by 2**64, then add over low bits
         mpz_set_si(r_big, d.two_words[1]);
         mpz_mul_2exp(r_big, r_big, 64);
         mpz_add_ui(r_big, r_big, d.two_words[0]);

         // Note output is 0 if composite, 1 if prob prime, 2 if provably prime
         q_prime = mpz_probab_prime_p(q_big, 0);
         r_prime = mpz_probab_prime_p(r_big, 0);

         //cout << "after mpz stuff\n";

         // check korselt if both are prime
         if(q_prime > 0 && r_prime > 0){
           modL = ( (q % L) * (r % L) % L) * (P_product % L) % L;
           modqminus = (r % (q-1)) * (P_product % (q-1)) % (q-1);
           modrminus = (q * P_product) % (r-1);

           // if all three are 1, add (q, r) to output pairs
           if(modL == 1 && modqminus == 1 && modrminus == 1){
             pair<int64, bigint> qr_found;
             qr_found.first = q;
             qr_found.second = r;
             output.push_back(qr_found);
           }
         } // end if korselt

       } // end if

      } // end for C      

    } // end if D large

  } // end for D
 
  // clear mpz
  mpz_clear(q_big);  mpz_clear(r_big); 

  return output;
} 
