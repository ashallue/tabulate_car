#include <iostream>
#include <fstream>
#include <vector>
#include "math.h"
#include "functions.h"
#include "Stack.h"
#include "Factgen.h"
#include "Construct_car.h"
#include "Odometer.h"
#include "bigint.h"
#include "Pinch.h"
#include <chrono>
#include "postprocess.h"
#include "gmp.h"

using namespace std::chrono;


/* This main meant for timings and counts that compare different code versions
 * Andrew Shallue, part of tabulating Carmichaels project.
 */

int main(int argc, char* argv[]) {
  std::cout << "Hello World!\n";
  

  Pinch CP = Pinch();
  Construct_car C1 = Construct_car();
  Construct_car C2 = Construct_car();
  Construct_car C3 = Construct_car();

  /*  
  // test preproduct with P = 3
  int64* P_ps = new int64[1];
  P_ps[0] = 3; 
  int64* P_m = new int64[1];
  P_m[0] = 2;

  vector<pair<int64, bigint>> test_output;
  test_output = C.preproduct_construction(P_ps, 1, P_m, 1, 2);  
  cout << "\n preproduct_crossover with small P\n";
  for(long i = 0; i < test_output.size(); i++){
    cout << test_output.at(i).first << " " << test_output.at(i).second << "\n";
  } 
  */

  
  // timing code from geeksforgeeks.org
  // Comparison of three methods: D-Delta, CD, and crossover
  // limited to prime preproducts.
    
  long size = atoi(argv[1]);
 
  int64 num_thousands = size * 10;
  int64 bound = num_thousands * 1000;
  cout << "Timings for tabulation of Carmichaels with prime pre-product up to " << bound << "\n";

  auto start_new = high_resolution_clock::now();
  C1.tabulate_car_primeP(bound, 0, 1, "cars.txt");
  auto end_new = high_resolution_clock::now();
 
  
  auto start_pinch = high_resolution_clock::now();
  CP.tabulate_car_primeP(bound, 0, 1, "cars_pinch.txt");
  auto end_pinch = high_resolution_clock::now();
 
  auto start_crossover_dyn = high_resolution_clock::now();
  C3.tabulate_car_primeP_crossover(bound, 0, 1, "cars_crossover.txt");
  auto end_crossover_dyn = high_resolution_clock::now();

  auto duration_new = duration_cast<seconds>(end_new - start_new);
  auto duration_pinch = duration_cast<seconds>(end_pinch - start_pinch);
  auto duration_crossover_dyn = duration_cast<seconds>(end_crossover_dyn - start_crossover_dyn);

  cout << "new timing: " << duration_new.count() << "\n";
  cout << "pinch timing: " << duration_pinch.count() << "\n";
  cout << "static crossover has been removed.  Earlier testing showed it is better for prime preproducts" << "\n";
  cout << "dynamic crossover timing: " << duration_crossover_dyn.count() << "\n";
  

  /*
  // This code times intialization and garbage collection for different types of variables
  long count = 10000000000;
  cout << "Timing initialization and malloc for " << count << " many variables\n";

  auto start_mpz = high_resolution_clock::now();
  for(long i = 0; i < count; ++i){
    mpz_t x;
    mpz_init(x);
    mpz_clear(x);
  }
  auto end_mpz = high_resolution_clock::now();
  auto duration_mpz = duration_cast<seconds>(end_mpz - start_mpz);
  cout << "timing for mpz_t: " << duration_mpz.count() << "\n";

  auto start_long = high_resolution_clock::now();
  for(long i = 0; i < count; ++i){
    long* x;
    x = new long;
    delete x;
  }
  auto end_long = high_resolution_clock::now();
  auto duration_long = duration_cast<seconds>(end_long - start_long);
  cout << "timing for long pointer: " << duration_long.count() << "\n";
   
  auto start_stacklong = high_resolution_clock::now();
  for(long i = 0; i < count; ++i){
    long x;
  }
  auto end_stacklong = high_resolution_clock::now();
  auto duration_stacklong = duration_cast<seconds>(end_stacklong - start_stacklong);
  cout << "timing for long on the stack: " << duration_stacklong.count() << "\n";
  */
  /* 
  // This code computes Carmichaels in two different ways as a check
  vector<bigint> cars = C.product_and_sort("cars.txt");
  vector<long> trivial_cars = trivial_car_tab(1000000);

  // check that they match
  bool all_match = true;
  cout << "new alg has " << cars.size() << " many cars\n";
  cout << "trivial alg has " << trivial_cars.size() << " many cars\n";
  for(long i = 0; i < cars.size(); i++){
    cout << "new: " << cars.at(i) << " vs trivial: " << trivial_cars.at(i) << "\n";
    
  }
  */
   
 
}
