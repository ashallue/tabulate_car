#include <iostream>
#include <fstream>
#include <vector>
#include "math.h"
#include "functions.h"
#include "libdivide.h"
#include "Stack.h"
#include "Factgen.h"
#include "LargeP_Carmichael.h"
#include "SmallP_Carmichael.h"
#include "Construct_car.h"
#include "Odometer.h"
#include "bigint.h"
#include "Pinch.h"
#include <chrono>
#include "postprocess.h"
#include "gmp.h"
#include "Pseudosquare.h"
#include <string>
#include "LargeP_Odometer.h"
#include "Preproduct.h"

using namespace std::chrono;


/* This main meant for testing purposes, and shorter timings
 */

int main(int argc, char* argv[]) {
  std::cout << "Hello World!\n";
  cout << "merging files\n";
  
  vector<string> files;
  files.push_back("/home/ashallue/tabulate_car/datafiles_shallue/cars1.txt");
  files.push_back("/home/ashallue/tabulate_car/datafiles_shallue/cars2.txt");
  files.push_back("/home/ashallue/tabulate_car/datafiles_shallue/cars3.txt");
  files.push_back("/home/ashallue/tabulate_car/datafiles_shallue/cars4.txt");
  files.push_back("/home/ashallue/tabulate_car/datafiles_shallue/cars5.txt");
  merge(files, "cars_merge.txt");

  //merge_two(files.at(0), files.at(1), "cars_merge.txt");

  cout << "\nTesting LargeP_Odometer\n";
  LargeP_Odometer odo = LargeP_Odometer(10000, 10);
  cout << "B = " << odo.B << " X = " << odo.X << " primes up to " << odo.prime_B << "\n";
  cout << "primes: ";
  for(long i = 0; i < odo.primes_count; ++i){
    cout << odo.primes[i] << " ";
  } 
  cout << "\n";
  cout << "P = " << odo.get_P() << " and its largest factor = " << odo.get_largest_prime() << "\n";
  cout << " curr_d = " << odo.curr_d << "\n";
  odo.next();
  cout << "P = " << odo.get_P() << " and its largest factor = " << odo.get_largest_prime() << "\n";
 
  //odo.large_products("test1.txt");
  cout << "\n";
  LargeP_Carmichael large_c = LargeP_Carmichael(1000000, 10);
  large_c.pinch(0, 1, "cars_pinch_large.txt");


 
  //odo2.trivial_large_products("test2.txt");

  cout << "\n";



  /*
  cout << "\nTesting LargeP construction\n";
  LargeP_Odometer od3 = LargeP_Odometer();
  for(long i = 0; i < 100; ++i){
    cout << "( " << od3.get_P() << ", " << od3.get_largest_prime() << " )" << " ";
    od3.next();
  }
  cout << "\n";

  cout << "The value of max_d for od3 is " << od3.max_d << "\n";
  LargeP_Odometer od4 = od3;
  cout << "The value of max_d for od4 is " << od4.max_d << "\n";
  LargeP_Odometer od5(od3);
  cout << "The value of max_d for od5 is " << od5.max_d << "\n";

  od3.large_products("odometer_output3.txt");
  od4.large_products("odometer_output4.txt");
  od5.large_products("odometer_output5.txt");
  */

  //LargeP_Carmichael lpconstruct = LargeP_Carmichael();
  //lpconstruct.tabulate_car(1, 1, "cars.txt");

  // Testing Factgen
 /*
  cout << "\nTesting Factgen\n";
  Factgen2 F = Factgen2();
  F.init(2, 100);
  for(long i = 0; i < 15; ++i){
    cout << F.prevval << " " << F.currentval << " : ";
    for(long j = 0; j < F.prevlen; ++j){
      cout << F.prev[j] << " ";
    }
    cout << "\n";

    // create preproduct object
    Preproduct P = Preproduct(F.currentval, F.current, F.currentlen, F.prev, F.prevlen);
    
    for(long i = 0; i < P.Pprimes_len; ++i){
      cout << P.Pprimes[i] << " ";
    }
    cout << ", ";
    
    cout << "Pminus = " << P.P - 1 << ", ";
    for(long i = 0; i < P.Pminus_len; ++i){
      cout << P.Pminus[i] << " ";
    }
    cout << " : ";
    cout << "Tau = " << P.Tau << ", admissable = " << P.admissable << "\n";


    // move sieve to next step
    F.next();
  } 
  */

  

  // construct Preproduct, need arrays for both P and P-1 
  long ps[] = {193, 257};
  long pms[] = {2, 5, 31};
  long es[] = {3, 1, 1};
  Preproduct P = Preproduct(49601, ps, 2, pms, 3);

  large_c.r_search_trial(P, 307, 1000);

  /* 
  auto start_new = high_resolution_clock::now(); 
  c1.preproduct_crossover(P);
  auto end_new = high_resolution_clock::now();
  auto duration_new = duration_cast<seconds>(end_new - start_new);
  cout << "timing for SmallP version: " << duration_new.count() << "\n";
  
  cout << "Carmichaels found: ";
  for(long i = 0; i < c1.qrs.size(); i++){
    cout << "(" << c1.qrs.at(i).first << ", " << c1.qrs.at(i).second << ") ";
  }
  cout << "\n";

  cout << "all DD method\n";
  SmallP_Carmichael c2 = SmallP_Carmichael();  
  auto start_dd = high_resolution_clock::now();
  c2.all_DDelta(P);
  auto end_dd = high_resolution_clock::now();
  auto duration_dd = duration_cast<seconds>(end_dd - start_dd);
  cout << "timing for all_DD method: " << duration_dd.count() << "\n";

  cout << "all CD method\n";
   
  SmallP_Carmichael c3 = SmallP_Carmichael();  
  auto start_cd = high_resolution_clock::now();
  c3.all_CD(P);
  auto end_cd = high_resolution_clock::now();
  auto duration_cd = duration_cast<seconds>(end_cd - start_cd);
  cout << "timing for all_CD method: " << duration_cd.count() << "\n";
  */
  /* 
  Construct_car other_c = Construct_car();
  auto start_old = high_resolution_clock::now();
  vector<pair<int64, bigint>> cars2 = other_c.preproduct_crossover(ps, 2, pms, 4, 252000);
  auto end_old = high_resolution_clock::now();
  auto duration_old = duration_cast<seconds>(end_old - start_old);
  cout << "timing for Construct_car version: " << duration_old.count() << "\n";
  
  cout << "Carmichaels found: ";
  for(long i = 0; i < cars2.size(); i++){
    cout << "(" << cars2.at(i).first << ", " << cars2.at(i).second << ") ";
  }
  cout << "\n";
  */

  /*
  cout << "All DDelta version:\n";
  cars1 = c.all_DDelta(P);
   
  cout << "Carmichaels found: ";
  for(long i = 0; i < cars1.size(); i++){
    cout << "(" << cars1.at(i).first << ", " << cars1.at(i).second << ") ";
  }
  cout << "\n";

  cout << "All CD version:\n";
  cars1 = c.all_CD(P); 
  cout << "Carmichaels found: ";
  for(long i = 0; i < cars1.size(); i++){
    cout << "(" << cars1.at(i).first << ", " << cars1.at(i).second << ") ";
  }
  cout << "\n";
  */
  //cout << "\nChecking file " << f1 << "\n";
  //car_smallp_file_check(f1, 1000000);

  //bigint n = 1009;
  //cout << "is " << n << " prime? " << ps.is_prime_pssquare(n) << "\n";

  //for(long i = 2; i < 129; i++){
  //  cout << "Is " << i << " a prime power? Answer: " << is_prime_power(i, nums, B) << "\n";
  //}

 
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
