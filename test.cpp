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
#include "Pseudosquare.h"
#include <string>
#include "LargeP_Odometer.h"

using namespace std::chrono;


/* This main meant for testing purposes, and shorter timings
 */

int main(int argc, char* argv[]) {
  std::cout << "Hello World!\n";

  
  cout << "Testing producting primes up to X\n";
 
  // create num array
  long B = 100000;
  long nums[B];
  for(long i = 0; i < B; i++){
    nums[i] = i;
  }
  // turn it into a factor sieve
  factor_sieve(nums, B);  
  
  // product primes
  cout << "Prime product is " << prime_product(102, nums, B);
 
  cout << "Testing is_prime:\n";
  for(long i = 0; i < 200; ++i){
    cout << i << " " << nums[i] << " " << is_prime(i, nums, B) << "\n";
  }
 

  cout << "\nTesting LargeP_Odometer\n";
  LargeP_Odometer odo = LargeP_Odometer();
  cout << "B = " << odo.B << " X = " << odo.X << " primes up to " << odo.prime_B << "\n";
  cout << "primes: ";
  for(long i = 0; i < odo.primes.size(); ++i){
    cout << odo.primes.at(i) << " ";
  } 
  cout << "\n";
  //odo.large_products("test1.txt");

  LargeP_Odometer odo2 = LargeP_Odometer();
 
  //odo2.trivial_large_products("test2.txt");

  cout << "\n";

  /* Pseudosquare check 
  Pseudosquare pssquare;
  pssquare.print();
  pssquare.check_pseudosquares();
  */

  cout << "\nTesting setting large integer values from literals\n";
  
  string test_string1 = "295363487400900310880401";
  string test_string2 = "3655334429477057460046489";
  string test_string3 = "99492945930479213334049";

  bigint final_version = string_to_bigint(test_string1);
  cout << "original string is " << test_string1 << "\n";
  cout << "using function we get " << final_version << "\n";

  cout << "Now let's convert back to mpz_t\n";
  mpz_t num_big;  mpz_init(num_big);
  mpz_set_si(num_big, 1001);  
  cout << "Initial mpz_t is ";
  mpz_out_str(nullptr, 10, num_big);
  
  bigint_to_mpz(final_version, num_big);
  cout << "Resulting mpz_t is ";
  mpz_out_str(nullptr, 10, num_big);
  cout << "\n";

  mpz_clear(num_big);

  // testing post-processing routines
  cout << "\nTesting product_and_sort\n";
  string f1 = "/home/ashallue/tabulate_car/million_preproducts_again/cars1.txt";
  string f2 = "/home/ashallue/tabulate_car/million_preproducts_again/cars2.txt"; 
  string f3 = "/home/ashallue/tabulate_car/million_preproducts_again/cars3.txt";
  string f4 = "/home/ashallue/tabulate_car/million_preproducts_again/cars4.txt";
  string f5 = "/home/ashallue/tabulate_car/million_preproducts_again/cars5.txt";

  vector<string> fs;
  fs.push_back(f1);  fs.push_back(f2); fs.push_back(f3);
  //fs.push_back(f4);  fs.push_back(f5);

  vector<bigint> cars = product_and_merge(fs);
  cout << "Total number of cars: " << cars.size() << "\n";


  for(long i = 0; i < 5; i++){
    cout << cars.at(i) << "\n";
  }

  // testing prime powering
  cout << "\nTesting powering functions\n";

  string ns1 = "295363487400900310880401";
  string ns2 = "22539340290692258087863249";  // 7^(30)
  string ns3 = "282475249";  // 7^(10)
  bigint num1 = string_to_bigint(ns1);
  bigint num2 = string_to_bigint(ns2);
  bigint num3 = string_to_bigint(ns3);

  mpz_t m2;
  mpz_init(m2);
  bigint_to_mpz(num2, m2);
  mpz_out_str(nullptr, 10, m2);
  cout << "\n";
  cout << "back to bigint: " << mpz_to_bigint(m2);
  mpz_clear(m2);
  cout << "\n";

  bigint root1 = floor( pow(num1, 1.0 / 2) );
  bigint pow1 = pow(root1, 2);

  cout << num1 << " then " << root1 << " then " << pow1 << "\n";

  bigint root2 = floor( pow(num2, 1.0 / 30) );
  bigint pow2 = pow(root2, 30);

  cout << num2 << " then " << root2 << " then " << pow2 << "\n";
  
  bigint root3 = floor( pow(num3, 1.0 / 10) );
  bigint pow3 = pow(root3, 10);

  cout << num3 << " then " << root3 << " then " << pow3 << "\n";

  if(num2 == pow2) cout << "Yes the second ones are equal\n";
  if(num3 == pow3) cout << "yes the third ones are equal\n";

  cout << "Is " << num1 << " a power?  A: " << is_power(num1) << "\n";
  cout << "Is " << num2 << " a power?  A: " << is_power(num2) << "\n";
  cout << "Is " << num3 << " a power?  A: " << is_power(num3) << "\n";

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
