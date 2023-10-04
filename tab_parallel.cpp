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

int main(int argc, char* argv[]) {
  std::cout << "This is tab_parallel with argc = " << argc << "\n";

  int thread = 0;
  int num_threads = 1;
  int size, rank;

  /* 
  // argc being 3 means two arguments given
  if(argc == 3){
    thread = atoi(argv[1]);
    num_threads = atoi(argv[2]);
  }
  */
   
  // set up MPI, including current processor (rank) and total number (size)
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  // rename to thread and num_threads, just for my sake
  thread = rank;
  num_threads = size;
   
  cout << "This is thread " << thread << " of " << num_threads << " total\n";

  string output_file = "cars6_" + to_string(thread) + ".txt";

  // The following are the bounds for full production up to 10^22
  // B is the upper bound on Carmichaels constructed, X is the crossover point
  
   
  bigint num_millions = 10000000000000000;
  bigint upper = num_millions * 1000000;

  long X = 70000000;
  
  /* 
  bigint num_millions = 1000000000;
  bigint upper = num_millions * 1000000;

  double one_third = 1.0 / 3;
  long X = ceil(pow(upper, one_third));
  */
  cout << "This is child process " << thread << "\n";
  std::cout << "Tabulating Carmichael numbers with d = 6 up to " << upper << "\n";
  std::cout << "where the small-large crossover point is " << X << "\n";  
  
  
  // code for large case
  LargePreproduct C = LargePreproduct(upper, X);

  auto start_large = high_resolution_clock::now();
  C.cars6_threaded(output_file, thread, num_threads);
  auto end_large = high_resolution_clock::now();

  auto duration_large = duration_cast<seconds>(end_large - start_large);
  cout << "Timing for large preproduct case: " << duration_large.count() << "\n\n";
  
  MPI_Finalize();
  return 0;

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
