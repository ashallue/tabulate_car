
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

using namespace std::chrono;

/* This executable combines both the small and large Preproduct cases to tabulate Carmichael numbers.
 * It is not meant for production code.  One flaw is that it is built to be serial, not parallel.
 * Another issue is that it is limited to tabulating Carmichael numbers with a set number of prime factors.
 * The purpose is testing: confirm that the techniques developed can repeat Pinch's tables.
 *
 * Written by Andrew Shallue, joint work with Jonathan Webster, June 2023
 * timing code from geeksforgeeks.org
 */

// expecting no arguments
int main(int argc, char* argv[]) {
  std::cout << "This is tab_serial, a program that tabulates Carmichaels on a single processor\n";

    std::cout << "num args = " << argc << "\n";
    std::cout << "0th arg is " << argv[0] << "\n";
    
    int total_jobs;
    int job_num;
    string filename;
    if(argc >= 2){
        job_num = atoi(argv[1]);
        total_jobs = atoi(argv[2]);
        std::cout << "job " << job_num  << " of a total of " << total_jobs << "\n";
        filename = "cars6large.txt";
    }else{
        filename = "default.txt";
        job_num = 0;
        total_jobs = 1;
    }

  string cars_large4 = "cars_large4_10to14.txt";
  string cars_large5 = "cars_large5_10to14.txt";
  string cars_large6 = "cars_large6_10to14.txt";
  string cars_large7 = "cars_large7_10to14.txt";
  string cars_large8 = "cars_large8_10to14.txt"; 
  string cars_large9 = "cars_large9_10to14.txt";  
  string cars_recursive10 = "cars_recursive10_10to17.txt";
  string cars_recursive11 = "cars_recursive11_10to17.txt";
  string cars_recursive12 = "cars_recursive12_10to17.txt";
  string cars_recursive13 = "cars_recursive13_10to17.txt";

  // B is the upper bound on Carmichaels constructed
  bigint num_millions_upper = 100000000000;
  bigint upper = num_millions_upper * 1000000;

  // for d = 3, I'll take preproduct upper bound to be B^{1/3}, then all are "small"
  double one_third = 1.0 / 3;
  bigint X3 = ceil(pow(upper, one_third));

  // for d = 4, I'll take the small-large crossover to be B^{1/3}
  bigint X4 = X3;

  /*
  ////////////////////////////////////////////
  // tabulating d = 3 Carmichaels
  std::cout << "Tabulating Carmichael numbers with d = 3 up to " << num_thousands_upper << " thousand, ";
 
  // code for small case 
  std::cout << "Starting small preproduct case\n"; 

  // preproduct bounds are 3 < P < crossover.  Bool signifies bounded construction rather than unbounded
  SmallP_Carmichael C1 = SmallP_Carmichael(3, X3, upper, true);

  auto start_small = high_resolution_clock::now();

  cout << "starting tabulation\n";
  // use appropriate thread, write to cars3_file, set output to verbose, i.e. identical to Pinch
  C1.tabulate_car(0, 1, cars3_file, true);

  auto end_small = high_resolution_clock::now();
 
  auto duration_small = duration_cast<milliseconds>(end_small - start_small);
  cout << "Timing for d = 3, small preproduct case: " << duration_small.count() << "\n\n";
  */ 
  ///////////////////////////////////////////////
  // tabulating d = 5 Carmichaels
 
  //std::cout << "Tabulating Carmichael numbers with d = 6 up to " << num_thousands_upper << " thousand, ";
  //std::cout << "where the small-large crossover point is " << X4 << "\n";  
 
   
  // code for small case
  //SmallP_Carmichael C2 = SmallP_Carmichael(3, 70000000, upper, false);
  
  /*
  // run on a single preproduct.  Need factorizations of P and P-1
  int64 P = 68800501;
  long Pfac_len = 6;
  int64 Pfac[Pfac_len] = {7, 11, 19, 31, 37, 41};
  long PMfac_len = 4;
  int64 PMfac[PMfac_len] = {2, 3, 5, 15289};
  Preproduct Pprod = Preproduct(P, Pfac, Pfac_len, PMfac, PMfac_len);
  // set residue index
  C2.res_P_index = P % C2.total_residue; 
  */

  /*
  // Another small case
  int64 P = 11;
  long Pfac_len = 1;
  int64 Pfac[Pfac_len] = {11};
  long PMfac_len = 2;
  int64 PMfac[PMfac_len] = {2, 5};
  Preproduct Pprod = Preproduct(P, Pfac, Pfac_len, PMfac, PMfac_len);
  C2.res_P_index = P % C2.total_residue;

  C2.preproduct_crossover(Pprod);
  // output stored in qrs vector
  for(long i = 0; i < C2.qrs.size(); i++){
    cout << P << " " << C2.qrs.at(i).first << " " << C2.qrs.at(i).second << "\n";
  }  
  */
  /*
  auto start_small = high_resolution_clock::now();

  cout << "starting tabulation\n";
  // use appropriate thread, write to cars3_file, set output to verbose, i.e. identical to Pinch
  C2.tabulate_car(0, 1, "cars15_small", true);

  auto end_small = high_resolution_clock::now();
 
  auto duration_small = duration_cast<milliseconds>(end_small - start_small);
  cout << "Timing for d = 7, small preproduct case: " << duration_small.count() << "\n\n";
  */
 
  /*    
  // code for large case
  std::cout << "Starting large preproduct case, limited to Carmichaels with 6 prime factors\n";

  bigint X = 70000000;
  LargePreproduct C3 = LargePreproduct(upper, X);
   
  bigint P = 691072624586401;
  long q = 3061;
  bigint L = 69743520;
  
  bigint P = 1026853825537;
  long q = 3061;
  bigint L = 17435880;
  
  bigint P = 800518548903151;
  long q = 9631;
  bigint L = 1520095500;

  vector<long> rs;

  C3.inner_loop_work(P, q, L, rs);

  cout << "number of cars found: " << rs.size() << "\n";

  for(int i = 0; i < rs.size(); i++){
    cout << P << " " << rs.at(i) << "\n";
  }
  */
  /*
  // large case tabulation
  cout << "starting tabulation\n";
  auto start_large = high_resolution_clock::now();
  //C3.cars4_threaded("cars4_large.txt", 0, 1);
  C3.cars_rec_threaded(6, "cars6_pinch.txt", 0, 1);
  auto end_large = high_resolution_clock::now();

  auto duration_large = duration_cast<seconds>(end_large - start_large);
  cout << "Timing for large 6 preproduct case (pinch version): " << duration_large.count() << "\n\n";
  */

  
  // code for large 6 case
  std::cout << "Starting large preproduct case\n";

  LargePreproduct C4 = LargePreproduct(upper, X4);

  cout << "starting tabulation\n";
  auto start_large = high_resolution_clock::now();

    
    C4.cars4_threaded(cars_large4, job_num, total_jobs);
    C4.cars5_threaded(cars_large5, job_num, total_jobs);
    C4.cars6_threaded(cars_large6, job_num, total_jobs);
    C4.cars7_threaded(cars_large7, job_num, total_jobs);
    C4.cars8_threaded(cars_large8, job_num, total_jobs);
    C4.cars9_threaded(cars_large9, job_num, total_jobs);
    C4.cars_rec_threaded(10, cars_recursive10, job_num, total_jobs);
    C4.cars_rec_threaded(11, cars_recursive11, job_num, total_jobs);
    C4.cars_rec_threaded(12, cars_recursive12, job_num, total_jobs);
    C4.cars_rec_threaded(13, cars_recursive13, job_num, total_jobs);
    

    /*
    C4.cars4_threaded_modified(cars_large4, job_num, total_jobs);
    C4.cars5_threaded_modified(cars_large5, job_num, total_jobs);
    C4.cars6_threaded_modified(cars_large6, job_num, total_jobs);
    C4.cars7_threaded_modified(cars_large7, job_num, total_jobs);
    C4.cars8_threaded_modified(cars_large8, job_num, total_jobs);
    C4.cars9_threaded_modified(cars_large9, job_num, total_jobs);
    C4.cars_rec_threaded_modified(10, cars_recursive10, job_num, total_jobs);
    C4.cars_rec_threaded_modified(11, cars_recursive11, job_num, total_jobs);
    C4.cars_rec_threaded_modified(12, cars_recursive12, job_num, total_jobs);
    C4.cars_rec_threaded_modified(13, cars_recursive13, job_num, total_jobs);
    */
  auto end_large = high_resolution_clock::now();

  auto duration_large = duration_cast<seconds>(end_large - start_large);
  cout << "Timing for large preproduct case with slower admissibility checks, is: " << duration_large.count() << "\n\n";


    /*
  // testing against two other ways of doing the large 6 case
  std::cout << "Starting large d = 6, with the modified function\n";

  LargePreproduct C5 = LargePreproduct(upper, X4);

  cout << "starting tabulation\n";
  start_large = high_resolution_clock::now();
  C5.cars6_threaded_modified("cars6_modifiedv1.txt", 0, 1);
  end_large = high_resolution_clock::now();

  duration_large = duration_cast<milliseconds>(end_large - start_large);
  cout << "Timing for large 6 modified: " << duration_large.count() << "\n\n";
 
  std::cout << "Starting large d = 6, with modified v2\n";

  LargePreproduct C6 = LargePreproduct(upper, X4);

  cout << "starting tabulation\n";
  start_large = high_resolution_clock::now();
  C6.cars6_threaded_modified_v2("cars6_modifiedv2.txt", 0, 1);
  end_large = high_resolution_clock::now();

  duration_large = duration_cast<milliseconds>(end_large - start_large);
  cout << "Timing for large 6 modified v2: " << duration_large.count() << "\n\n";
  */
  /* 
  ////////////////////////////////////////
  // code for recursive version
  LargePreproduct C4 = LargePreproduct(upper, X4);

  std::cout << "Doing large case again, this time 8 prime factors, recursive version\n";
  auto start_rec = high_resolution_clock::now();
  C4.cars_rec(8, rec_file);
  auto end_rec = high_resolution_clock::now();

  auto duration_rec = duration_cast<milliseconds>(end_rec - start_rec);
  cout << "Timing for recursive version: " << duration_rec.count() << "\n\n";
  */
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
