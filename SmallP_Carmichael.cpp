/* Implementation file for class that constructs 
Carmichael numbers.
  Andrew Shallue and Jonathan Webster

  This is a second attempt.  In my first attempt I treated P-1 and P+D as separate quantities.
  But this is a bad idea: I can end up with repeated divisors, when what I want is divisors of (P-1)(P+D).
  So in this attempt I will unify these and have a single set of divisors to loop over.

  Update: this is now a third attempt, specifically a refactoring.  The SmallP_Carmichael class has 
  become the SmallP_Carmichael class.  Many details have been encapsulated in the Preproduct class.
  I am leaving SmallP_Carmichael in the library, but it should be considered deprecated.

  Assumptions on sizes that apply throughout:
  pre-product P is at most 32bits, D is at most 32bits, so both fit into int64.
  importantly, the quantity (P-1)(P+D) also fits into int64
  q fits into an int64, but primality testing on q requires bigint (128 bits), which is handled by Sorenson
  r is larger than 64 bits, fits into bigint, but primality testing and proving requires 256-bit, so arbitrary precision
*/

#include "SmallP_Carmichael.h"

using namespace std;

// default sets X to 2^(16) = 65536.
// Incremental sieve starts at 2
SmallP_Carmichael::SmallP_Carmichael(){
  B_upper = 65536;
  B_lower = 1;
  X = 1000000000;
  bounded_cars = false;

  // create Factgen2 object and initialize
  F = Factgen2();
  F.init(2, B_upper);

  // create Factgen object for P+D
  // don't initialize now, need to do so later
  FD = Factgen();

  // set q, r to 0.  initialize the mpz variables
  q = 0;  r = 0;  q_D = 0;
  mpz_init(q_mpz);   mpz_init(r_mpz);

  qrs = vector<pair<int64, bigint>>();
  qrs.reserve(1000);

  // set residues data structures
  total_residue = 210;
  num_residues = 4;
  for(long i = 0; i < total_residue; ++i){
    residues_P[i][0] = i % 2;
    residues_P[i][1] = i % 3;
    residues_P[i][2] = i % 5;
    residues_P[i][3] = i % 7;
    residues_D[i][0] = i % 2;
    residues_D[i][1] = i % 3;
    residues_D[i][2] = i % 5;
    residues_D[i][3] = i % 7;  
  }
  res_P_index = 0;
  res_D_index = 0;
}

// set preproduct bound B to given value.  Initialize F.  FD gets initialized in a separate function.
SmallP_Carmichael::SmallP_Carmichael(int64 B_low_val, int64 B_up_val, bigint X_val, bool bounded){
  // same as default, except for the B input
  B_upper = B_up_val;
  B_lower = B_low_val;  
  X       = X_val;
  bounded_cars = bounded; 

  F = Factgen2();
  F.init(B_lower - 1, B_upper);
  FD = Factgen();

  q = 0;  r = 0;  q_D = 0;
  mpz_init(q_mpz);  mpz_init(r_mpz);

  qrs = vector<pair<int64, bigint>>();
  qrs.reserve(1000);

  // set residues data structures
  total_residue = 210;
  num_residues = 4;
  for(long i = 0; i < total_residue; ++i){
    residues_P[i][0] = i % 2;
    residues_P[i][1] = i % 3;
    residues_P[i][2] = i % 5;
    residues_P[i][3] = i % 7;
    residues_D[i][0] = i % 2;
    residues_D[i][1] = i % 3;
    residues_D[i][2] = i % 5;
    residues_D[i][3] = i % 7;  
  }
  res_P_index = 0;
  res_D_index = 0;
}

//destructor is here to clear the mpz_t variables, everything else can be cleared using default methods
SmallP_Carmichael::~SmallP_Carmichael(){
  mpz_clear(q_mpz);
  mpz_clear(r_mpz);
  qrs.clear();
}

// copy constructor needs to initialize the mpz variables
SmallP_Carmichael::SmallP_Carmichael(const SmallP_Carmichael& other){
  // first copy over factgen objects and bounds
  F = other.F;
  FD = other.FD;
  B_upper = other.B_upper;
  B_lower = other.B_lower;
  q = other.q;
  r = other.r;
  q_D = other.q_D;
  qrs = other.qrs;

  //now itialize the mpz variables and copy them over
  mpz_init(q_mpz);  mpz_init(r_mpz);
  mpz_set(q_mpz, other.q_mpz);   mpz_set(r_mpz, other.r_mpz);

  // set residues data structures
  for(long i = 0; i < 210; ++i){
    residues_P[i][0] = i % 2;
    residues_P[i][1] = i % 3;
    residues_P[i][2] = i % 5;
    residues_P[i][3] = i % 7;
    residues_D[i][0] = i % 2;
    residues_D[i][1] = i % 3;
    residues_D[i][2] = i % 5;
    residues_D[i][3] = i % 7;  
  }
  res_P_index = other.res_P_index;
  res_D_index = other.res_D_index;
  total_residue = other.total_residue;
  num_residues = other.num_residues;
}

// operator= is very similar to copy constructor
SmallP_Carmichael SmallP_Carmichael::operator=(const SmallP_Carmichael& other){
  // create new object and copy over non-mpz variables
  SmallP_Carmichael result_ob;
  result_ob.F = other.F;
  result_ob.FD = other.FD;
  result_ob.B_upper = other.B_upper;
  result_ob.B_lower = other.B_lower;
  result_ob.q = other.q;
  result_ob.r = other.r;
  result_ob.q_D = other.q_D;
  result_ob.qrs = other.qrs;

  // initialize and set mpz variables
  mpz_init(result_ob.q_mpz);   mpz_init(result_ob.r_mpz);
  mpz_set(result_ob.q_mpz, other.q_mpz);  mpz_set(result_ob.r_mpz, other.r_mpz);

  // set residues data structures
  for(long i = 0; i < 210; ++i){
    result_ob.residues_P[i][0] = i % 2;
    result_ob.residues_P[i][1] = i % 3;
    result_ob.residues_P[i][2] = i % 5;
    result_ob.residues_P[i][3] = i % 7;
    result_ob.residues_D[i][0] = i % 2;
    result_ob.residues_D[i][1] = i % 3;
    result_ob.residues_D[i][2] = i % 5;
    result_ob.residues_D[i][3] = i % 7;  
  }
  result_ob.res_P_index = other.res_P_index;
  result_ob.res_D_index = other.res_D_index;
  result_ob.total_residue = other.total_residue; 
  result_ob.num_residues = other.num_residues; 

  return result_ob;
}

// Historical note: when this class was Construct_car, it had an admissable function.
// That function now in the Preproduct class

/* Given an admissible pre-product P, compute all Carmichael numbers 
// up to X of the form Pqr where q and r are primes.
// Return is void.  Pairs (q,r) that make Carmichaels are pushed on the vector qrs by completion check
// Inputs are factorizations of P and P-1, along with L = lcm_{p | P} (p-1)
// This version not tied to F.  Would be slightly more efficient if it was.
*/
void SmallP_Carmichael::all_DDelta(Preproduct& P){

  // We need a Factgen object for factorizations of P+D
  // Initialize to match D in [2 .. P-1]
  FD.init(P.Prod + 2, 2 * P.Prod);

  // Basic loop structure: for all D in [2..(P-1)], for all divisors
  // of the expression (P-1)(P+D)/2, do stuff.
  // First set the D residue
  res_D_index = 2;
  libdivide::divider<int64> fast_D;
  for(int64 D = 2; D < P.Prod; ++D){
   
    fast_D = libdivide::divider<int64>(D);    

    // testing
    //cout << "P = " << P.Prod << " " << "D = " << D << "and D residue = " << res_D_index << "\n";

    // go to the next P+D through the Factgen object
    FD.next();

    // gather Carmichaels from the DDelta function.  Note: stored in vector qrs
    DDelta(P, D, fast_D);

    // update the D residue index in preparation for the next cycle   
    res_D_index++;
    if(res_D_index >= total_residue) res_D_index -= total_residue;

    //if(D_cars.size() > 0) cout << "Cars found when calling DDelta method on D = " << D << "\n";

  } // end for D
  
}

// same as above.  Don't use this for production
void SmallP_Carmichael::all_CD(Preproduct& P){
  libdivide::divider<int64> fastD;
  
  // loop over D, call CD method.  For this function I shouldn't have to update FD object.
  for(int64 D = 2; D < P.Prod; ++D){
    fastD = libdivide::divider<int64>(D);
    CD(P, D, fastD);

  }
}

// using only the DDelta method, compute Carmichaels for all preproducts up to a bound
// same code as tabulate_car, but with crossover replaced by all_DDelta
void SmallP_Carmichael::tabulate_all_DDelta(string cars_file){

  int64* P_factors;
  long   P_factors_len;
  int64* Pminus_factors;
  long   Pminus_factors_len;

  // file stream object
  ofstream output;
  output.open(cars_file);

  // initialize the Factgen2 object that stores factorizations of P, P-1
  F.init(2, B_upper);

  // count the number of admissable pre-products
  int64 num_admissable = 0;

  // set P residue
  res_P_index = 3;

  // Now loop over odd pre-products P
  for(int64 P = 3; P < B_upper; P = P + 2){

    // testing
    //cout << "tabulate_car, loops with P = " << P << "\n";

    // retrieve factorizations of P, P-1
    P_factors = F.current;
    P_factors_len = F.currentlen;
    Pminus_factors = F.prev;
    Pminus_factors_len = F.prevlen;

    // construct Preproduct object
    Preproduct P_ob = Preproduct(P, P_factors, P_factors_len, Pminus_factors, Pminus_factors_len);

      // if admissable, construct Carmichaels
      if(P_ob.admissable){
     
        // Construct all Carmichael numbers with pre-product P.  First clear the qrs member variable
        qrs.clear();

        // if the largest prime dividing pre-product is large enough, do cross-over
        all_DDelta(P_ob);
        
        // testing
       // cout << "P = " << P << " generates " << qrs.size() << " many carmichaels\n";

        // print to file
        //output << "Carmichaels for P = " << P << " number of Cars is " << qrs.size() << "\n";
        for(long j = 0; j < qrs.size(); ++j){
          output << P << " " << qrs.at(j).first << " " << qrs.at(j).second << "\n";
        } // end for
      } // end if admissable
      // move the factorization window to next odd number
      F.next();
      F.next();

      // update the P index
      res_P_index += 2;
      if(res_P_index > total_residue) res_P_index -= total_residue;

  } // end for P 

  // close file and clear the qrs
  output.close();
  qrs.clear();
}


void SmallP_Carmichael::tabulate_all_CD(string cars_file){
  int64* P_factors;
  long   P_factors_len;
  int64* Pminus_factors;
  long   Pminus_factors_len;

  // file stream object
  ofstream output;
  output.open(cars_file);

  // initialize the Factgen2 object that stores factorizations of P, P-1
  F.init(2, B_upper);

  // count the number of admissable pre-products
  int64 num_admissable = 0;

  // Now loop over odd pre-products P
  for(int64 P = 3; P < B_upper; P = P + 2){

    // testing
    //cout << "all_CD, loops with P = " << P << " that has residue " << res_P_index << "\n";

    // retrieve factorizations of P, P-1
    P_factors = F.current;
    P_factors_len = F.currentlen;
    Pminus_factors = F.prev;
    Pminus_factors_len = F.prevlen;

    // construct Preproduct object
    Preproduct P_ob = Preproduct(P, P_factors, P_factors_len, Pminus_factors, Pminus_factors_len);

      // if admissable, construct Carmichaels
      if(P_ob.admissable){
     
        // Construct all Carmichael numbers with pre-product P.  First clear the qrs member variable
        qrs.clear();

        // if the largest prime dividing pre-product is large enough, do cross-over
        all_CD(P_ob);
        
        // testing
       // cout << "P = " << P << " generates " << qrs.size() << " many carmichaels\n";

        // print to file
        //output << "Carmichaels for P = " << P << " number of Cars is " << qrs.size() << "\n";
        for(long j = 0; j < qrs.size(); ++j){
          output << P << " " << qrs.at(j).first << " " << qrs.at(j).second << "\n";
        } // end for
      } // end if admissable
      // move the factorization window to next odd number
      F.next();
      F.next();

  } // end for P 

  // close file and clear the qrs
  output.close();
  qrs.clear();
}

/* D-Delta method for a single Preproduct, D pair.  Computes all divisors of (P-1)(P+D),
 * Calls completion check, which will write the pair (q,r) to vector qrs if Pqr is Carmichael.
 *
 * Later improvement: due to the integrality of C, if p | D then either P, Delta both 0 mod p or both not 0
 * To make Delta divisible by p, I have added a divisor_multiple to Odometer
 * To make Delta not divisible by p, remove the p power from the list when creating the Odometer
 */
void SmallP_Carmichael::DDelta(Preproduct& P, bigint D, libdivide::divider<int64>& fastD){
    //cout << "Inside DDelta with P = " << P.Prod << " and D = " << D << "\n";

    int64 Delta_bound;  // stores upper bound on Delta to ensure it isn't too big (making q too small)
    int64 div = 1;      // will store divisors of (P-1)(P+D)/2    
    int64 divisor_multiple = 1;  // primes that all Odomter divs must include

    // We set up an odometer, which requires primes and powers
    // Note this is not the final value of q, just the one needed to compute divisors.
    q_D = (P.Prod - 1) * (P.Prod + D) / 2;
    int64 qtemp = q_D;   
 
    // for each prime tracked, first check if D = 0 mod p
    for(long i = 0; i < num_residues; ++i){
      if(residues_D[res_D_index][i] == 0){
        // if P = 0 mod p, then Delta = 0 mod p, so add the prime as a must have
        // The way this is implemented, we take the prime out of q, then Odometer multiplies by must_divide at end
        if(residues_P[res_P_index][i] == 0){
          // of course, this is only done if q_D is divisible by the prime in the first place
          if(qtemp % primes[i] == 0){
            divisor_multiple *= primes[i];
            qtemp = qtemp / primes[i];
          }
        }else if(residues_P[res_P_index][i] == 1){
        // if P != 0 mod p, then Delta != 0 mod p, so remove all factors of p from q
          while(qtemp % primes[i] == 0){
            qtemp = qtemp / primes[i];
          }
        }
      }
      // nothing changed if the D residue is != 0      

    } // end for loop over residues

    // P_minus has the unique prime factors dividing P-1.
    // copy over prev into PplusD
    int64* PplusD = FD.prev;
    long PplusD_len = FD.prevlen;   
 
    // from PplusD and Pminus, compute full factorization of q_D (see Preproduct class) 
    int64* q_primes = new int64[PplusD_len + P.Pminus_len];
    long* q_exps   = new long[PplusD_len + P.Pminus_len];
    long q_primes_len = P.q_factorization(qtemp, PplusD, PplusD_len, q_primes, q_exps);  

    // Set up odometer to run through divisors of (P-1)(P+D)/2.  true means we are computing 
    // and storing divisors up front.  Passing false would mean divisors are computed on the fly
    Odometer q_od = Odometer(q_primes, q_exps, q_primes_len, divisor_multiple, true);
    div = q_od.get_div();

    // Run the code for divisor Delta = 1
    // apply completion check subroutine to see if this divisor Delta creates Carmichael
    bool some_carmichaels = completion_check(P, div, D, fastD);
    
    // now go to next divisor
    q_od.next_div();
    div = q_od.get_div();
    
    // Throw out the divisor if it is too big.  It needs to be small enough so q is bigger than p_{d-2}.
    // The appropriate bound is Delta < (P-1)(P+D)/(p_{d-2}-1)
    // Mult size check: P.Prod is at most 32 bits, so mult will fit in 64 bits, and 64-bit * okay
    Delta_bound = (P.Prod - 1) * (P.Prod + D);
    Delta_bound = Delta_bound / (P.largest_prime() - 1);
    
    // continue looking at divisors until it is back to 1
    while(div != q_od.initial_div){

      //testing
      //if(D == 13) cout << "inside DDelta, div = " << div << " and Delta_bound = " << Delta_bound << "\n";

      if(div < Delta_bound){

        // apply completion check subroutine to see if this divisor Delta creates Carmichael
        some_carmichaels = completion_check(P, div, D, fastD);
      } // end if div < Delta_bound
      
      // next iteration
      q_od.next_div();
      div = q_od.get_div();
    } //end while div != 1

    // free memory for q_primes and q_exps before next cycle
    delete[] q_primes;
    delete[] q_exps;

}

// CD method (Pinch algorithm).  Given Preproduct and D, compute Carmichael completions
// Something to note about multipilcation: P.Prod should be only 32 bits, so multiplication in 64 bits
void SmallP_Carmichael::CD(Preproduct& P, bigint D, libdivide::divider<int64>& fastD){ 

  // for CD method we generate all C in an interval.  These are the bounds.
  // lower bound is P^2/D and we want C > P^2/D, so we start at 1 + P^2/D
  int64 C_lower = 1 + (P.Prod * P.Prod) / D;
  // upper bound is (P^2 * (p_{d-2} + 3))/(D * (p_{d-2} + 1)); computation broken into two steps
  // +2 added at end to allow for rounding that happens with integer division
  int64 Cu_temp = (P.Prod * P.Prod) / D;
  int64 C_upper = Cu_temp + 2 * Cu_temp / (P.largest_prime() + 1) + 2;

  //if(D == 18) cout << "(P, D) = " << P.Prod << ", " << D << ": Bounds in CD method are " << C_lower << " and " << C_upper << "\n";

  // Delta bound to ensure q > p_{d-2}
  int64 Delta;
  int64 Delta_bound = (P.Prod - 1) * (P.Prod + D);
  Delta_bound = Delta_bound / (P.largest_prime() - 1);
  
  // helper variables
  bool q_integral;
  bool some_carmichaels;

  // loop over C
  for(int64 C = C_lower; C <= C_upper; ++C){
    // compute Delta and q_D
    // C*D is bounded by 2P^2, so multipication fits into 64 bits
    Delta = C * D - P.Prod * P.Prod;
    q_D = (P.Prod - 1) * (P.Prod + D);  

    // check integrality of q
    q_integral = q_D % Delta == 0;

    if(q_integral && Delta < Delta_bound){
      some_carmichaels = completion_check(P, Delta, D, fastD, C);

    }  

  } // end for C
}
    
/* Once I have the loop over divisors of (P-1)(P+D)/Delta, I need to perform the following steps:
    1) Compute C = (P^2 + Delta)/D, check that it is integral (unless C != 0, meaning given as parameter)
    2) Check Pqr for Korselt criterion
    3) check q and r for small prime factors (compositeness check)
    4) Compute q = (P-1)(P+D)/Delta + 1, check primality.  Don't check integrality, we know because div = Delta.
    5) Compute r = (P-1)(P+C)/Delta + 1, check integrality and primality
  Note that I know q is integral, because Delta chosen as divisor of (P-1)(P+D).
  primality of q, r determined through mpz_probab_prime_p, a gmp func which does Baillie-PSW
  Returns bool: true if there are carmichael completions.  All found are written as pairs to vector qrs
  Update: q, r, and mpz_versions now members of the class.
*/
bool SmallP_Carmichael::completion_check(Preproduct& P, int64 Delta, int64 D, libdivide::divider<int64>& D_div, int64 C_param){
  // setup output.  By default set it to (0, 0) which means false
  pair<int64, bigint> output;
  output.first = 0;   output.second = 0;

  // look up at store the preproduct value and the LCM from the Preproduct object
  int64 P_val = P.Prod;
  int64 LCM   = P.L;

  int64 C;
  if(C_param == 0){

    // compute C and check that it is integral
    int64 intermediate;

    // compute quotient and rem at the same time, compiler should only use 1 instruction
    intermediate = P_val * P_val + Delta;
    C = intermediate / D_div;
    // update: using libdivide, so need to write intermediate % D = intermediate - q * D
    // Note that this shouldn't require bigint.  Inter_rem should be okay with int64
    bigint inter_rem = intermediate - C * D;

    // if rem is not 0, C not integral, return false
    if(inter_rem != 0){
      return false;
    }
  }else{
    C = C_param;
  }

  // compute q.  Recall q_D already defined.
  // If DDelta method, it is (P-1)(P+D)/2.  If CD method, it is (P-1)(P+D)
  // We can tell which to use depending on whether C_param is non zero or not
  if(C_param == 0){
    q = q_D * 2 / Delta + 1;
  }else{
    q = q_D / Delta + 1;
  }

  // compute r, check it is integral.  Recall r = (P-1)(P+C)/Delta + 1
  bigint r_quo, r_rem; 
  r = (bigint)(P_val - 1) * (bigint)(P_val + C);          // note r initialized by constructor
  r_quo = r / Delta;
  r_rem = r % Delta;

  if(r_rem != 0) return false;
  r = r_quo + 1;

  // Check that Pqr satisfies Korselt criterion, i.e. Pqr = 1 mod lcm(L, q-1, r-1)
  // first compute product modulo L.  We work with reduced quantities since L is smaller than q, r
  // Cast quantities into bigints so that the multiplication will be bigint-*.  Lack of casting was 
  // a bit oversight in an earlier version.
  bigint modL = ( (bigint)(q % LCM) * (r % (bigint)LCM) % (bigint)LCM ) * (bigint)(P_val % LCM) % (bigint)LCM;
  if(modL != 1) return false;
  bigint modqminus = (r % (bigint)(q-1)) * (bigint)(P_val % (q-1)) % (bigint)(q-1);
  if(modqminus != 1) return false;
  bigint modrminus = ((bigint)q * (bigint)P_val) % (r-1);
  if(modrminus != 1) return false;
 
  // primality testing on q, r.  Will use a gmp func, so requires conversion to mpz
  //mpz_t q_big;
  //mpz_t r_big;
  //mpz_init(q_big); mpz_init(r_big); 

  // q can be converted directly.  r is possibly 128 bits, we need to use Dual_rep
  mpz_set_si(q_mpz, q);
  Dual_rep d;
  d.double_word = r;

  // Set the high bits, multiply by 2**64, then add over low bits
  // I think I have the si, ui correct here.  I tested it with examples where d.two_words[0] requires 64bits, 
  // and it worked fine.
  mpz_set_si(r_mpz, d.two_words[1]);
  mpz_mul_2exp(r_mpz, r_mpz, 64);
  mpz_add_ui(r_mpz, r_mpz, d.two_words[0]);

  /*
  // testing
  cout << "q in two different reps: " << q << " ";
  mpz_out_str(nullptr, 10, q_big);
  cout << "\n r in two different reps: " << r << " ";
  mpz_out_str(nullptr, 10, r_big);
  cout << "\n";
  */

  // check pseudo-primality of q, r.  mpz_probab_prime_p could return 0, 1, or 2.  0 for composite
  if(mpz_probab_prime_p(q_mpz, 0) == 0){
   // mpz_clear(q_big);  mpz_clear(r_big);
    return false;
  }
  if(mpz_probab_prime_p(r_mpz, 0) == 0){
   // mpz_clear(q_big);  mpz_clear(r_big);
    return false;  
  }

  // clear mpz
  //mpz_clear(q_big);   mpz_clear(r_big);

  // If we have gotten to this point we have passed all the checks above
  output.first = q;   output.second = r;
  // write to the vector qrs
  qrs.push_back(output);

  return true;
}

/* Construct Carmichaels for a range of pre-products P < B
 */
void SmallP_Carmichael::tabulate_car(long processor, long num_threads, string cars_file, bool verbose_output){
  int64* P_factors;
  long   P_factors_len;
  int64* Pminus_factors;
  long   Pminus_factors_len;
  
  // n is big enough in an unbounded computation to require mpz type
  mpz_t n;
  mpz_init(n);

  // let's also calculate the average value of L/P
  //double avg_ratio = 0;

  // file stream object
  ofstream output;
  output.open(cars_file);
  
  // Issue: writing mpz_t to a file.
  // Looking at stack overflow, write-quickly-gmp-variables-in-files, going to try FILE type
  //FILE* output;

  // set start value to the first odd number greater or equal to B_lower
  int64 start_P = B_lower;
  if(start_P % 2 == 0) start_P++;

  // initialize the Factgen2 object that stores factorizations of P, P-1
  F.init(start_P - 1, B_upper);
 
  F.print();

  // count the number of admissable pre-products
  int64 num_admissable = 0;

  // set the P residue
  res_P_index = start_P % total_residue;

  // Now loop over odd pre-products P
  for(int64 P = start_P; P < B_upper; P = P + 2){

    // retrieve factorizations of P, P-1
    P_factors = F.current;
    P_factors_len = F.currentlen;
    Pminus_factors = F.prev;
    Pminus_factors_len = F.prevlen;

    /*
    cout << "inside tabulate_car, considering P = " << P << ": ";
    for(long i = 0; i < P_factors_len; i++){
      cout << P_factors[i] << " ";
    }
    cout << "\n";
    */

    // construct Preproduct object
    Preproduct P_ob = Preproduct(P, P_factors, P_factors_len, Pminus_factors, Pminus_factors_len);

    // add ratio L/P to the running total
    //avg_ratio += P_ob.L / (P + 0.0) ;

    // check admissability.  If so, add to count
    if(P_ob.admissable) num_admissable++;

    // If Pp^2 >= X, throw out that preproduct
    bool bounded_pass;
    if(!bounded_cars){
      bounded_pass = true;
    }else{
      // this next line needs to be fixed. X is bigint, multiplication probably int64
      bounded_pass = P * P_factors[P_factors_len - 1] * P_factors[P_factors_len - 1] < X;
    }

    // if num_admissable has the correct residue and pass bounded check, do work, otherwise continue
    if( (num_admissable % num_threads) != (processor % num_threads) || !bounded_pass){
      F.next();
      F.next();

      // update the P index
      res_P_index += 2;
      if(res_P_index > total_residue) res_P_index -= total_residue;

    }else{
      // if admissable, construct Carmichaels
      if(P_ob.admissable){
    
        // testing
        //cout << "admissable preproduct considered: " << P_ob.Prod << "\n";
 
        // Construct all Carmichael numbers with pre-product P.  First clear the qrs member variable
        qrs.clear();

        // if the largest prime dividing pre-product is large enough, do cross-over
        preproduct_crossover(P_ob);
        
        // testing
        //cout << "P = " << P << " generates " << qrs.size() << " many carmichaels\n";

        // print to file
        //output << "Carmichaels for P = " << P << " number of Cars is " << qrs.size() << "\n";
        for(long j = 0; j < qrs.size(); ++j){

          // compute n.  Set it to r using dualrep, then multiply by q and by P
          Dual_rep d;
          d.double_word = qrs.at(j).second;
          // set high bits, multiply by 2**64, add low bits
          mpz_set_si(n, d.two_words[1]);
          mpz_mul_2exp(n, n, 64);
          mpz_add_ui(n, n, d.two_words[0]);

          // multiply by P and by q
          mpz_mul_si(n, n, P_ob.Prod);
          mpz_mul_si(n, n, qrs.at(j).first);          

          // if bounded, only print if n < X.  Also print if not bounded.
          // this line is not the correct approach to boundedness
          //if(n < X || !bounded_cars){

            // output depends on the input bool verbose_output.  If true, give n followed by factors
            if(verbose_output){

              // now print n followed by its factors, space separated.  First convert n to a string
              //char* n_cstr = NULL;
              //mpz_get_str(n_cstr, 10, n);
              //string n_str = string(n_cstr);

              output << n << " ";
              for(long k = 0; k < P_ob.Pprimes_len; k++){
                output << P_ob.Pprimes[k] << " ";
              } 
              output << qrs.at(j).first << " " << qrs.at(j).second << "\n";

              //delete[] n_cstr;

            }else{
              // otherwise, output preproduct P, followed by q then r
              output << P << " " << qrs.at(j).first << " " << qrs.at(j).second << "\n";
            }
          //}
        } // end for
      } // end if admissable
      // move the factorization window to next odd number
      F.next();
      F.next();

      // update the P index
      res_P_index += 2;
      if(res_P_index > total_residue) res_P_index -= total_residue;
    
    } // end if correct processor
  } // end for P 

  // close file and clear the qrs
  output.close();
  qrs.clear();
  mpz_clear(n);

  // to stdout print avg ratio
  //cout << "average ratio of L/P is " << avg_ratio / num_admissable << "\n";
}

/* Construct Carmichaels for prime pre-products P, using just D-Delta method
 */
void SmallP_Carmichael::tabulate_car_primeP(long processor, long num_threads, string cars_file){
  int64* P_factors;
  long   P_factors_len;
  int64* Pminus_factors;
  long   Pminus_factors_len;
  Preproduct P_ob;

  // file stream object
  ofstream output;
  output.open(cars_file);

  // initialize Factgen2 object
  F.init(2, B_upper);

  // count the number of prime pre-products
  int64 num_prime_P = 0;

  // set P residue
  res_P_index = 3;

  // loop over pre-products
  for(int64 P = 3; P < B_upper; ++P){
    // check if P is prime.  If not, continue
    if(!F.isprime_current()){
      F.next();
      F.next();

      // update the P index
      res_P_index += 2;
      if(res_P_index > total_residue) res_P_index -= total_residue;

    }else{
      //cout << "Found prime P = " << P << "\n";     

      // found a prime.  Increment count, set Pminus
      num_prime_P++;
      P_factors = F.current;
      P_factors_len = F.currentlen;
      Pminus_factors = F.prev;
      Pminus_factors_len = F.prevlen;

      // if prime count in correct residue class, construct cars
      if(num_prime_P % num_threads != processor){
        F.next();
        F.next();

        // update the P index
        res_P_index += 2;
        if(res_P_index > total_residue) res_P_index -= total_residue;

      }else{
        // construct preproduct object
        P_ob = Preproduct(P, P_factors, P_factors_len, Pminus_factors, Pminus_factors_len);
 
        qrs.clear();
        all_DDelta(P_ob);

        // write to file
        for(long j = 0; j < qrs.size(); ++j){
          output << P << " " << qrs.at(j).first << " " << qrs.at(j).second << "\n";
        }
 
        // advance window
        F.next();
        F.next();
      
        // update the P index
        res_P_index += 2;
        if(res_P_index > total_residue) res_P_index -= total_residue;
      }
    }// end if P prime

  } // end for P
  // close file
  output.close();
  qrs.clear();
}

/* Version that calls the cross-over function
 */
void SmallP_Carmichael::tabulate_car_primeP_crossover(long processor, long num_threads, string cars_file){
  int64* P_factors;
  long   P_factors_len;
  int64* Pminus_factors;
  long   Pminus_factors_len;
  Preproduct P_ob;

  // file stream object
  ofstream output;
  output.open(cars_file);

  // initialize Factgen2 object and P residue
  F.init(2, B_upper);
  res_P_index = 3;

  // count the number of prime pre-products
  int64 num_prime_P = 0;

  // loop over pre-products
  for(int64 P = 3; P < B_upper; ++P){
    // check if P is prime.  If not, continue
    if(!F.isprime_current()){
      F.next();
      F.next();

      // update the P index
      res_P_index += 2;
      if(res_P_index > total_residue) res_P_index -= total_residue;
    
    }else{
      //cout << "Found prime P = " << P << "\n";     

      // found a prime.  Increment count, set Pminus
      num_prime_P++;
      P_factors = F.current;
      P_factors_len = F.currentlen;
      Pminus_factors = F.prev;
      Pminus_factors_len = F.prevlen;

      // if prime count in correct residue class, construct cars
      if(num_prime_P % num_threads != processor){
        F.next();
        F.next();

        // update the P index
        res_P_index += 2;
        if(res_P_index > total_residue) res_P_index -= total_residue;
      
      }else{
        // construct preproduct object
        P_ob = Preproduct(P, P_factors, P_factors_len, Pminus_factors, Pminus_factors_len);

        // construct carmichaels with that particular preproduct.
        qrs.clear();
        preproduct_crossover(P_ob);

        // write to file
        for(long j = 0; j < qrs.size(); ++j){
          output << P << " " << qrs.at(j).first << " " << qrs.at(j).second << "\n";
        }
 
        // advance window
        F.next();
        F.next();

        // update the P index
        res_P_index += 2;
        if(res_P_index > total_residue) res_P_index -= total_residue;
      }  
    }// end if P prime

  } // end for P
  // close file
  output.close();
  qrs.clear();
}

/* Another version, but this one has the D crossover strategy
 * For each D, we choose either D-Delta method or CD method depending on the anticipated amount of work for each.
 * This involves calculating L_p, the length of the interval for CD method, and estimating the number of divisors
 * of (P-1)(P+D) for the D-Delta method. 
*/
void SmallP_Carmichael::preproduct_crossover(Preproduct& P){
  int64* PplusD;
  long PplusD_len;

  // testing
  //cout << "Running preproduct_crossover with P = " << P.Prod << "\n";
  
  // For the dynamic version we need L_p, defined by 
  // P^2 + L_p = P^2 (p_{d-2} + 3)/(p_{d-2} + 1), where p_{d-2} is largest prime in P
  // So L_p = P^2 ( (p_{d-2} + 3)/(p_{d-2} + 1) - 1)
  // But we will use the simpler estimation of 2 * P^2 / p_{d-2}
  int64 L_p = 2 * P.Prod * P.Prod / P.largest_prime();

  // We need a Factgen object for factorizations of P+D
  // Initialize to match D in [2 .. P-1]
  FD.init(P.Prod + 2, 2 * P.Prod);

  // count the number of times we do C-D
  int64 count_CD = 0;
  bool do_DDelta_method = true;
  int64 count_DDelta = 0;

  // calculate an estimate for the number of divisors.  Crossover to CD if L_P/D < divisor count
  int64 divisor_estimate = 1;

  // calling either CD or DDelta method will create Carmichael completions
  vector<pair<int64, bigint>> qrs;

  // Note: incomplete idea for only doing crossover in certain situation
  // if p (largest prime of P) satisfies P/p < 20, do D-Delta unconditionally, no crossover
  // Note 20 is "magical", just supposed to represent "small"
  // if(P / largestp >= 20) <then do crossover, i.e. the entire rest of the function>

  // Basic loop structure: for all D in [2..(P-1)], for all divisors
  // of the expression (P-1)(P+D)/2, do stuff.
  //
  // Initialize D residue and libdivide object
  res_D_index = 2;
  libdivide::divider<int64> fast_D;

  for(int64 D = 2; D < P.Prod; ++D){
    fast_D = libdivide::divider<int64>(D);

    // Start with code to decide whether to do D-Delta or C-D method
    // if count_CD > 20, do C-D.  20 chosen without investigation.
    if(count_CD > 20){
      do_DDelta_method = false;
    }else{
      // if we are in the DDelta method, then turn the FD incremental sieve
      if(do_DDelta_method){
        FD.next();
        // then FD.prev corresponds to P+D
        PplusD = FD.prev;
        PplusD_len = FD.prevlen;
      }
      // estimate the number of divisors of (P-1)(P+D).  Recall there are fewer if D = 0 mod p for small p
      divisor_estimate = P.Tau * pow(2, PplusD_len);
      // correction factor for rebalancing after speeding up DDelta method.  Constant based on experiments.
      divisor_estimate /= 4;

      // in the dynamic case, switch if L_P / D is less than the count of divisors
      // Divisor count of P-1 stored in Preproduct class as Tau, then include estimate for P+D divisor count
      // Need to rebalance.  Try this: for every p with D = 0 mod p, divide divisor count by 2
      if( L_p / D < divisor_estimate){
        do_DDelta_method = false;
      }
      
    } // end deciding which method

    // if D is small, do the D-Delta method
    if(do_DDelta_method){
      DDelta(P, D, fast_D);
      
      // testing
      //if(qrs.size() > 0) cout << "DDelta method for D = " << D << " found Carmichaels: ";
      //count_DDelta++;

    } // end if D small
    else{

      // turn counter and perform CD method
      count_CD++;
      CD(P, D, fast_D);

      //testing
      //if(qrs.size() > 0) cout << "CD method for D = " << D << " found Carmichaels: "; 

    } // end if D large i.e. end of else

    // note the Carmichaels are written to vector qrs by completion_check, called by either DDelta or CD

    // update D residue
    res_D_index++;
    if(res_D_index > total_residue) res_D_index -= total_residue;

  } // end for D
  //cout << "Crossover with P = " << P.Prod << " did DDelta method " << count_DDelta << " times and the CD method " << count_CD << " many times\n";
 
}

 
