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
 
  auto start_crossover_static = high_resolution_clock::now();
  C2.tabulate_car_primeP_crossover(bound, 0, 1, "cars_crossover.txt", false);
  auto end_crossover_static = high_resolution_clock::now();
 
  auto start_crossover_dyn = high_resolution_clock::now();
  C3.tabulate_car_primeP_crossover(bound, 0, 1, "cars_crossover.txt", true);
  auto end_crossover_dyn = high_resolution_clock::now();

  auto duration_new = duration_cast<seconds>(end_new - start_new);
  auto duration_pinch = duration_cast<seconds>(end_pinch - start_pinch);
  auto duration_crossover_static = duration_cast<seconds>(end_crossover_static - start_crossover_static);
  auto duration_crossover_dyn = duration_cast<seconds>(end_crossover_dyn - start_crossover_dyn);

  cout << "new timing: " << duration_new.count() << "\n";
  cout << "pinch timing: " << duration_pinch.count() << "\n";
  cout << "static crossover timing: " << duration_crossover_static.count() << "\n";
  cout << "dynamic crossover timing: " << duration_crossover_dyn.count() << "\n";
   

  

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
