#include <iostream>
#include <fstream>
#include <vector>
#include "math.h"
#include "functions.h"
#include "Stack.h"
#include "Factgen.h"
#include "Construct_car.h"
#include "LargePreproduct.h"
#include "SmallP_Carmichael.h"
#include "postprocess.h"
#include "Odometer.h"
#include "bigint.h"
#include "Pinch.h"
#include <chrono>
#include <mpi.h>

using namespace std::chrono;

/* This executable focuses on the large preproduct case of tabulating carmichael numbers
 * MPI is used to create a parallel process, but the underlying algorithm is embarassingly parallel.
 *
 * Written by Andrew Shallue, joint work with Jonathan Webster, June 2023
 * timing code from geeksforgeeks.org
 */

// expecting two arguments: thread number and total number of threads
int main(int argc, char* argv[]) {
  std::cout << "This is tab_parallel\n";

  int thread = 0;
  int num_threads = 1;

  // argc being 3 means two arguments given
  if(argc == 3){
    thread = atoi(argv[1]);
    num_threads = atoi(argv[2]);
  }

  // set up MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &num_threads);
  MPI_Comm_size(MPI_COMM_WORLD, &thread);
 
  cout << "This is thread " << thread << " of " << num_threads << " total\n";

  string output_file = "cars4_" + to_string(thread) + ".txt";

  // B is the upper bound on Carmichaels constructed
  int64 num_thousands_upper = 100000000000;
  int64 upper = num_thousands_upper * 1000;

  // X is the crossover point, the lower bound on preproducts constructed
  double one_third = 1.0 / 3;
  bigint X = ceil(pow(upper, one_third));


  std::cout << "Tabulating Carmichael numbers with d = 4 up to " << num_thousands_upper << " thousand, ";
  std::cout << "where the small-large crossover point is " << X << "\n";  

  // code for large case
  LargePreproduct C = LargePreproduct(upper, X);

  auto start_large = high_resolution_clock::now();
  C.cars4_threaded(output_file, thread, num_threads);
  auto end_large = high_resolution_clock::now();

  auto duration_large = duration_cast<seconds>(end_large - start_large);
  cout << "Timing for large preproduct case: " << duration_large.count() << "\n\n";

 /* 
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
   
 */  
 
}
