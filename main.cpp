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


int main(int argc, char* argv[]) {
  std::cout << "Hello World!\n";
  cout << "Argument 1: " << argv[1] << "\n";
  cout << "Argument 2: " << argv[2] << "\n";
  
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

  // free memory for the above
  delete[] test_array;
  delete[] test_arr2;
  delete[] ps;
  delete[] es;

  // Code for tabulating Carmichael numbers with fixed pre-product

  // retrieve thread number and total from input
  long thread = atoi(argv[1]);
  long num_threads = atoi(argv[2]);

  // create file names for carmichaels, and admissable pre-products w/o carmichaels
  string cars_file = "cars" + to_string(thread) + ".txt";
  string none_file = "cars_none" + to_string(thread) + ".txt";

  Pinch CP = Pinch();
  Construct_car C = Construct_car();

  // timing code from geeksforgeeks.org
  
  int64 num_thousands = 10000;
  int64 bound = num_thousands * 1000;
  cout << "Timings for tabulation of Carmichaels with pre-product up to " << bound << "\n";
  cout << "This is thread " << thread << " of " << num_threads << " total\n";

  auto start_new = high_resolution_clock::now();

  //C.tabulate_car(bound, 0, 1, "cars.txt", "cars_none.txt");
  C.tabulate_car(bound, thread, num_threads, cars_file, none_file);

  auto end_new = high_resolution_clock::now();
 
  /*
  auto start_pinch = high_resolution_clock::now();
  CP.tabulate_car(bound, 0, 1, "cars_pinch.txt", "cars_none_pinch.txt");
  auto end_pinch = high_resolution_clock::now();
  */

  auto duration_new = duration_cast<seconds>(end_new - start_new);
  //auto duration_pinch = duration_cast<seconds>(end_pinch - start_pinch);

  cout << "new timing: " << duration_new.count() << "\n";
  //cout << "pinch timing: " << duration_pinch.count() << "\n";
  
 
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
