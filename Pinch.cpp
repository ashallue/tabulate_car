/* Implementation file for Pinch's carmichael tabulation algorithm
 * Andrew Shallue, Feb 2022
 */

#include "Pinch.h"

using namespace std;

// Given admissable pre-product, construct all Carmichaels of the form Pqr
// output is a list of pairs (qr).
// Note q, r only probably prime, tested by trial division of primes < 1000
vector<pair<int64, bigint>> Pinch::pinch_preproduct(int64* P, long P_len, int64 L){
  vector<pair<int64, bigint>> output;

  // construct product of all distinct primes in P
  int64 P_product = 1;
  for(long i = 0; i < P_len; ++i){
    P_product = P_product * P[i];
  }

  //cout << "Pinch on preproduct = " << P_product << "\n";

  bigint C_lower;  // lower and upper bounds on C
  bigint C_upper;
  int64 largestp = P[ P_len-1 ];
  
  // stores the potential last 2 prime factors q, r
  int64 q;
  bigint r;
  int64 Delta;    // Delta = CD - P^2

  // boolean values for whether q, r are integral, prime
  bool q_integral, r_integral;
  bool q_prime, r_prime;

  // reduced values for testing Korselt
  bigint modL, modqminus, modrminus;

  // loop over D, in range [2, P-1]
  for(int64 D = 2; D < P_product; ++D){
    //cout << "D = " << D << "\n";

    
    // loop over C such that P^2 < CD < P^2 (largestp + 3)/(largestp + 1)
    // Note that I can have CD = P^2, which would make Delta = 0
    C_lower = 1 + (P_product * P_product) / D ;
    C_upper = (P_product * P_product * (largestp + 3)) / (D * (largestp + 1)) ;

    // surprised it is C <= C_upper, but testing reveals < misses Carmichaels
    for(int64 C = C_lower; C <= C_upper; ++C){

      //cout << "Tabulating according to Pinch, P = " << P_product << " D = " << D << " C = " << C << "\n";
 
      // compute Delta, along with numerators of q, r
      Delta = C * D - P_product * P_product;
      q = (P_product - 1) * (P_product + D);
      r = (P_product - 1) * (P_product + C); 

      // we need Delta < (P-1)(P+D) / p_{d-2} - 1 to ensure q > p_{d-2}
      int64 Delta_bound = (P_product - 1) * (P_product + D);
      Delta_bound = Delta_bound / (largestp - 1) ;      

      // test q, r for integrality
      q_integral = q % Delta == 0;
      r_integral = r % Delta == 0;     

      // we only do further work if both are integral, and Delta < bound
      if(q_integral && r_integral && Delta < Delta_bound){
        // compute q, r
        q = 1 + q / Delta;
        r = 1 + r / Delta;

        // now primality testing on q, r.  Currently just doing trial division by primes up to 1000
        q_prime = trial_thousand(q);
        r_prime = trial_thousand(r);
      
        // if both are prime, check korselt
        if(q_prime && r_prime){
          
          // we compute Pqr mod L
          modL = ( (q % L) * (r % L) % L ) * (P_product % L) % L;
          // Pqr mod q-1 same as Pr mod q-1.  Similar for r
          modqminus = (r % (q-1)) * (P_product % (q-1)) % (q-1);
          modrminus = (q * P_product) % (r-1); 

          // if all three are 1, add (q, r) pair to output
          if(modL == 1 && modqminus == 1 && modrminus == 1){

            pair<int64, bigint> qr_found;
            qr_found.first = q;
            qr_found.second = r;
            output.push_back(qr_found);
          }
        } // end q, r prime

      } // end if integral

    } // end for C
     
  } // end for D 

  return output;
}

/* Construct Carmichaels for a range of pre-products P
 *  *   We use a factgen2 object, write to a file.  Only process pre-products that fall into residue class of the core.
 *   */
void Pinch::tabulate_car(int64 B, long processor, long num_threads, string cars_file, string admissable_file){
  int64* P_factors;
  long   P_len;
  vector<pair<int64, bigint>> qrs;
  int64 LCM;

  // file stream objects
  ofstream output;
  output.open(cars_file);

  ofstream admissable_w_zero;
  admissable_w_zero.open(admissable_file);

  // initialize Factgen object
  F.init(3, B);  

  // loop over pre-products P
  for(int64 P = 2; P < B; ++P){
    //cout << "Pinch with preproduct " << P << "\n";

    // if P has the correct residue, do work, otherwise continue
    if(P % num_threads != processor){
      F.next();
    } else{
      // clear qrs
      qrs.clear();

      // allocate memory for P_factors;
      P_len = F.prevlen;
      P_factors = new int64[P_len];

      // retrieve factorizations of P
      for(long i = 0; i < F.prevlen; ++i){
        P_factors[i] = F.prev[i];
      }

      // check admissability
      LCM = admissable(F.prevn, P_factors, P_len);
  
      // testing
      /*
      cout << "P = " << P << ": ";
      for(long i = 0; i < P_factors.size(); i++){
        cout << P_factors.at(i) << " ";
      }
      cout << " L = " << LCM << "\n";
      */ 

      // if admissable, construct Carmichaels
      if(LCM != 0){
        qrs = pinch_preproduct(P_factors, P_len, LCM);

        // if no carmichaels, write pre-product to other file
        if(qrs.size() == 0){
          admissable_w_zero << F.prevn << "\n";
        }else{
          // if there are some, write to cars file
          for(long j = 0; j < qrs.size(); ++j){
            output << P << " " << qrs.at(j).first << " " << qrs.at(j).second << "\n";
          }
        }// end if no carmichaels

      } // end if LCM != 0
      // move factorization window and free memory for P_factors
      F.next();
      delete[] P_factors;

    } // end if correct thread
  } // end for P
  // close iles
  output.close();
  admissable_w_zero.close();
}

/* similar, but restrict to prime pre-products
 */
void Pinch::tabulate_car_primeP(int64 B, long processor, long num_threads, string cars_file){
  int64* P_factors;
  long   P_factors_len;
  int64* Pminus_factors;
  long   Pminus_factors_len;
  vector<pair<int64, bigint>> qrs;

  // file stream object
  ofstream output;
  output.open(cars_file);

  //initialize Factgen object
  F.init(3, B);

  // count the number of prime pre-products
  int64 num_prime_P = 0;

  // loop over pre-products
  for(int64 P = 2; P < B; ++P){
    //cout << "P = " << P << " vs prevn = " << F.prevn << " vs " << F.isprime() << "\n"; 

    // check if P is prime.  If not, continue
    if(F.prevlen != 1 || F.prev[0] != F.prevn){
      F.next();
    }else{
      //cout << "Found prime P = " << P << "\n";

      // found a prime.  Increment count, set Pminus
      num_prime_P++;
      P_factors = F.prev;
      P_factors_len = F.prevlen;

      // if prime count in correct residue class, construct cars
      if(num_prime_P % num_threads != processor){
        F.next();
      }else{
        qrs.clear();
        qrs = pinch_preproduct(P_factors, P_factors_len, P-1);

        // write to file
        for(long j = 0; j < qrs.size(); ++j){
          output << P << " " << qrs.at(j).first << " " << qrs.at(j).second << "\n";
        }

        // advance window
        F.next();
      }  // end if correct processor

    } // end if prime   
  } // end for P
}

// Tests whether a given pre-product P is admissable, 
// i.e. that gcd(p-1, P) = 1 for all p | P, and squarefree
// returns 0 if not admissable
int64 Pinch::admissable(int64 P, int64* Pprimes, long Pprimes_len){
  // construct product of the prime factors of P
  int64 P_product = 1;
  for(long i = 0; i < Pprimes_len; ++i){
    P_product = P_product * Pprimes[i];
  }

  // not squarefree if this product is not P
  if(P_product != P) return 0;

  int64 g; // stores gcds

  // assume true, and check for evidence otherwise
  // As we go along, compute the lcm L
  int64 L = 1;
  for(long i = 0; i < Pprimes_len; ++i){

    // compute gcd(p-1, P).  
    int64 prime = Pprimes[i];
    g = gcd(prime - 1, P_product);

    // Not admissable if gcd not 1
    if(g != 1){
      L = 0;
    }else{
      // otherwise, L = lcm(prevL, p-1)
      g = gcd(L, prime - 1);
      L = L * (prime - 1) / g;
    }                                                                          
  }
  return L;
}


