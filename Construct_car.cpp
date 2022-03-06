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
    // Update: discovered with valgrind this wasn't needed, memory freed by ~Odometer
    //delete[] q_primes;
    //delete[] q_exps;

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
  Note that r is not proven prime, that will be dealt with in post-processing.

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
  
  /*
  // next build the vector of unique prime factors of q - 1 = (P-1)(P+D)/Delta
  // It will take several lines, but we will apply pocklington primality proof to q
  // we merge the vectors of unique prime factors, which removes duplicates
  vector<int64> temp_qminus = merge(Pminus, PplusD);
  vector<int64> qminus;
  int64 tempDelta = Delta;
  int64 tempq = (P - 1) * (P + D);

  // variables for exponents
  long D_e;
  long D_P;

  // loop over prime divisors of (P-1)(P+D).  
  for(long i = 0; i < temp_qminus.size(); ++i){

    // if the prime divides Delta, compute respective exponents
    if(tempDelta % temp_qminus.at(i) == 0){

      // compute power of the prime dividing Delta
      D_e = 1;
      while(tempDelta % temp_qminus.at(i) == 0){
        D_e++;
        tempDelta = tempDelta / temp_qminus.at(i);
      }

      // compute power of the prime dividing tempq = (P-1)*(P+D)
      D_P = 1;
      while(tempq % temp_qminus.at(i) == 0){
        D_P++;
        tempq = tempq / temp_qminus.at(i);
      }
      // now if the exponent on (P-1)(P+D) is bigger, include that prime
      if(D_P > D_e) qminus.push_back(temp_qminus.at(i));
    }else{
      // in the case where the prime doesn't divide Delta, include that prime
      qminus.push_back(temp_qminus.at(i));
    }
  } // end for
  
  // Now that we have unique prime factors of q-1, call pocklington primality on q
  if(!pocklington(q, qminus)) return output;
  */

  // check pseudo-primality of q.  Done through trial division
  if(!trial_thousand(q)) return output;

  // next check pseudo-primality of r.  This will be done through trial division
  // primes array of length 168 found in int.h
  if(!trial_thousand(r)) return output;  

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

  // Now loop over pre-products P
  for(int64 P = 3; P < B; ++P){
    // if P has the correct residue, do work, otherwise continue
    if(P % num_threads != processor){
      F.next();
    }else{

      //cout << "P = " << P << " current = " << F.currentval << " prev = " << F.prevval << "\n";

      // retrieve factorizations of P, P-1
      P_factors = F.current;
      P_factors_len = F.currentlen;
      Pminus_factors = F.prev;
      Pminus_factors_len = F.prevlen;

      // check admissability
      LCM = admissable(F.currentval, P_factors, P_factors_len);
 
      // if admissable, construct Carmichaels
      if(LCM != 0){
     
        // Construct all Carmichael numbers with pre-product P
        qrs.clear();
        qrs = preproduct_construction(P_factors, P_factors_len, Pminus_factors, Pminus_factors_len, LCM);

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

/* Reads in a file of Carmichael numbers of the form P q r, computes the product, sorts resulting vector
 */
vector<bigint> Construct_car::product_and_sort(string cars_file){
  vector<bigint> sorted_cars;
  
  // open input file
  ifstream cars;
  cars.open(cars_file);

  // store line and the numbers in that line
  string line;
  vector<bigint> linenums;
  bigint product;
  bigint num;

  // while there is a line to get, keep getting lines
  while(getline(cars, line)){
    // access each number in the line and place into a vector
    // this solution from stackoverflow: reading-line-of-integers-into-a-vector
    istringstream numbers_stream(line);
    linenums.clear();

    while(numbers_stream >> num){
      linenums.push_back(num);
    }
    // now multiply numbers in the line
    product = 1;
    for(long i = 0; i < linenums.size(); ++i){
      product = product * linenums.at(i);
    }

    // push product onto the output vector
    sorted_cars.push_back(product);

  } // end while reading lines from file

  // sort vector of carmichaels
  sort(sorted_cars.begin(), sorted_cars.end());

  // close file and return the cars
  cars.close();
  return sorted_cars;
}



 
