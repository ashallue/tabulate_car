#include <iostream>
#include <fstream>
#include <vector>
#include "math.h"
#include "functions.h"
#include "Stack.h"
#include "Factgen.h"
#include "Construct_car.h"
#include "postprocess.h"
#include "Odometer.h"
#include "bigint.h"
#include "Pinch.h"
#include <chrono>

using namespace std::chrono;


// expecting two arguments: 1) thread number for this instance, 2) total thread count
int main(int argc, char* argv[]) {
  std::cout << "Hello World!\n";
  cout << "Argument 1: " << argv[1] << "\n";
  cout << "Argument 2: " << argv[2] << "\n";
  

  // Code for tabulating Carmichael numbers with fixed pre-product

  // retrieve thread number and total from input
  long thread = atoi(argv[1]);
  long num_threads = atoi(argv[2]);

  // create file names for carmichaels, and admissable pre-products w/o carmichaels
  string cars_file = "cars" + to_string(thread) + ".txt";
  string none_file = "cars_none" + to_string(thread) + ".txt";

  Construct_car C = Construct_car();

  // timing code from geeksforgeeks.org
  
  // we want all pre-products up to 10^8, which is 10^5 thousands
  int64 num_thousands = 2;
  int64 bound = num_thousands * 1000;
  cout << "Timings for tabulation of Carmichaels with pre-product up to " << bound << "\n";
  cout << "This is thread " << thread << " of " << num_threads << " total\n";

  auto start_new = high_resolution_clock::now();

  //C.tabulate_car(bound, 0, 1, "cars.txt", "cars_none.txt");
  C.tabulate_car(bound, thread, num_threads, cars_file, none_file);

  auto end_new = high_resolution_clock::now();
 
  auto duration_new = duration_cast<seconds>(end_new - start_new);

  cout << "new timing: " << duration_new.count() << "\n";
  
  
  // This code computes Carmichaels in two different ways as a check
  long car_bound = 500000;
  vector<bigint> cars = product_and_sort("cars0.txt");
  vector<long> trivial_cars = trivial_car_tab(car_bound);

  // check that they match
  bool all_match = true;
  cout << "new alg has " << cars.size() << " many cars\n";
  cout << "trivial alg has " << trivial_cars.size() << " many cars\n";
  for(long i = 0; i < trivial_cars.size(); i++){
    cout << "new: " << cars.at(i) << " vs trivial: " << trivial_cars.at(i) << "\n";
    
  }
   
   
 
}
