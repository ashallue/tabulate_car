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

using namespace std::chrono;


int main() {
  std::cout << "Hello World!\n";
  
  // could also try 10000991  = [ 7, 11, 13, 97, 103 ]

  Pinch CP = Pinch();
  Construct_car C = Construct_car();

  /*
  cout << "Testing carmichael_completion with given pre-product 10^7 + 127\n";
  vector<int64> P;   P.push_back(167); P.push_back(233); P.push_back(257);
  vector<int64> Pm;  Pm.push_back(2); Pm.push_back(151); Pm.push_back(33113);
  int64 L = C.admissable(10000127, P);

  auto start_new = high_resolution_clock::now(); 
  vector<pair<int64, bigint>> qrs = C.preproduct_construction(P, Pm, L);
  auto end_new = high_resolution_clock::now();
  auto duration_new = duration_cast<seconds>(end_new - start_new);
  

  // checki
  cout << "Carmichaels from the new algorithm\n";
  cout << "Took " << duration_new.count() << " seconds\n";
  for(long i = 0; i < qrs.size(); i++){
    cout << qrs.at(i).first << " " << qrs.at(i).second << "\n";
  }
  cout << "\n";
  
  qrs.clear();
  auto start_pinch = high_resolution_clock::now();
  qrs = CP.pinch_preproduct(P, L);
  auto end_pinch = high_resolution_clock::now();
  auto duration_pinch = duration_cast<seconds>(end_pinch - start_pinch);

  cout << "Carmichaels from pinch algorithm\n";
  cout << "Took " << duration_pinch.count() << "seconds\n";
  for(long i = 0; i < qrs.size(); i++){
    cout << qrs.at(i).first << " " << qrs.at(i).second << "\n";
  }
  */

  
  // timing code from geeksforgeeks.org
  
  int64 bound = 2000;
  cout << "timings for tabulation of Carmichaels with pre-product up to " << bound << "\n";

  auto start_new = high_resolution_clock::now();
  C.tabulate_car(bound, 0, 1, "cars.txt", "cars_none.txt");
  auto end_new = high_resolution_clock::now();

  auto start_pinch = high_resolution_clock::now();
  CP.tabulate_car(bound, 0, 1, "cars_pinch.txt", "cars_none_pinch.txt");
  auto end_pinch = high_resolution_clock::now();

  auto duration_new = duration_cast<seconds>(end_new - start_new);
  auto duration_pinch = duration_cast<seconds>(end_pinch - start_pinch);

  cout << "new timing: " << duration_new.count() << "\n";
  cout << "pinch timing: " << duration_pinch.count() << "\n";
  
 
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
