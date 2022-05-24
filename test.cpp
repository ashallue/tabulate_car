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


/* This main meant for testing purposes, and shorter timings
 */

int main(int argc, char* argv[]) {
  std::cout << "Hello World!\n";
  
  // could also try 10000991  = [ 7, 11, 13, 97, 103 ]

  // testing merge on arrays
  int64* test_array = new int64[6];
  test_array[0] = 1;
  test_array[1] = 5;
  test_array[2] = 8;
  test_array[3] = 9;
  test_array[4] = 6;
  test_array[5] = 0;
  for(long i = 0; i < 6; i++){
    cout << test_array[i] << " ";
  }
  cout << "\n";

  int64* test_arr2 = new int64[4];
  test_arr2[0] = 2;
  test_arr2[1] = 9;
  test_arr2[2] = 10;
  test_arr2[3] = 5;

  for(long i = 0; i < 4; i++){
    cout << test_arr2[i] << " ";
  }
  cout << "\n";
  
  cout << "\nTesting Odometer\n";
  long num_ps = 4;
  int64* ps = new int64[num_ps];
  int64* es = new int64[num_ps];

  ps[0] = 2;  ps[1] = 3;  ps[2] = 5;  ps[3] = 11;
  es[0] = 3;  es[1] = 1;  es[2] = 1;  es[3] = 1;
  Odometer od = Odometer(ps, es, num_ps);
  cout << "listing divisors:\n";
  for(long i = 0; i < 20; i++){
    cout << od.get_div() << " ";
    od.next_div();
  }
  cout << "\n";

  cout << "\nTesting exp_in_factorization\n";
  cout << exp_in_factorization(2, 64) << "\n";
  cout << exp_in_factorization(2, 12) << "\n";
  cout << exp_in_factorization(3, 81) << "\n";
  cout << exp_in_factorization(3, 12) << "\n";
  cout << exp_in_factorization(5, 12) << "\n";

  // free memory for the above
  delete[] test_array;
  delete[] test_arr2;
  delete[] ps;
  delete[] es;

  Pinch CP = Pinch();
  Construct_car C = Construct_car();
  
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

 
  // timing code from geeksforgeeks.org
  // Comparison of three methods: D-Delta, CD, and crossover
  /*
  long size = atoi(argv[1]);
 
  int64 num_thousands = size * 10;
  int64 bound = num_thousands * 1000;
  cout << "Timings for tabulation of Carmichaels with pre-product up to " << bound << "\n";

  auto start_new = high_resolution_clock::now();
  C.tabulate_car_primeP(bound, 0, 1, "cars.txt");
  auto end_new = high_resolution_clock::now();
 
  
  auto start_pinch = high_resolution_clock::now();
  CP.tabulate_car_primeP(bound, 0, 1, "cars_pinch.txt");
  auto end_pinch = high_resolution_clock::now();
 
  auto start_crossover = high_resolution_clock::now();
  C.tabulate_car_primeP_crossover(bound, 0, 1, "cars_crossover.txt");
  auto end_crossover = high_resolution_clock::now();
 

  auto duration_new = duration_cast<seconds>(end_new - start_new);
  auto duration_pinch = duration_cast<seconds>(end_pinch - start_pinch);
  auto duration_crossover = duration_cast<seconds>(end_crossover - start_crossover);

  cout << "new timing: " << duration_new.count() << "\n";
  cout << "pinch timing: " << duration_pinch.count() << "\n";
  cout << "crossover timing: " << duration_crossover.count() << "\n";
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
