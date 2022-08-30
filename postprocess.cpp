/* Various functions used after a primary tabulation has been performed
 * Part of the tabulating Carmichael project, joint with Jonathan Webster.
 * Author: Andrew Shallue, Illinois Wesleyan University, written May 2020
 *
 * implementation file
 */

#include "postprocess.h"

using namespace std;


/* Reads in a file of Carmichael numbers of the form P q r, computes the product, sorts resulting vector
 */
vector<bigint> product_and_sort(string cars_file){
  vector<bigint> sorted_cars;
  
  // open input file
  ifstream cars;
  cars.open(cars_file);

  // store line and the numbers in that line
  string line;
  vector<bigint> linenums;
  bigint product;

  // while there is a line to get, keep getting lines
  while(getline(cars, line)){
    // access each number in the line and place into a vector
    // this solution from stackoverflow: reading-line-of-integers-into-a-vector
    istringstream numbers_stream(line);
    linenums.clear();

    // notice this only works if the line has exactly three numbers
    // I tried using a while loop, but had problems with some variable not being intialized.  num?
    for(long i = 0; i < 3; ++i){
      bigint num;
      numbers_stream >> num;
      linenums.push_back(num);
    }

    //cout << "line = " << line << "\n";
    //cout << "number of nums in the line is " << linenums.size() << "\n";

    // now multiply numbers in the line
    product = 1;
    for(long i = 0; i < linenums.size(); ++i){
      product = product * linenums.at(i);
    }

    // push product onto the output vector
    sorted_cars.push_back(product);

  } // end while reading lines from file

  // sort vector of carmichaels
  sort(sorted_cars.begin(), sorted_cars.end());

  // close file and return the cars
  cars.close();
  return sorted_cars;
}


/* Given a vector of filenames, call product_and_sort on each one, then merge them using the standard library.
 * Resulting output is of type vector<bigint> and should be all Carmichael numbers
 */
vector<bigint> product_and_merge(vector<string> filenames){
  // when we call product_and_sort for each filename, each result is a vector of bigints, so we need vector of vector
  vector< vector<bigint> > lists_of_cars;
  // populate all those vectors by repeatedly calling product_and_sort
  for(long i = 0; i < filenames.size(); ++i){
    lists_of_cars.push_back( product_and_sort(filenames.at(i)) );
  }

  // I will gradually build up the output vector through a series of merges.
  // To make this work, create a bunch of vectors of increasing size
  long output_size = 0;
  vector< vector<bigint> > output;
  for(long i = 0; i < lists_of_cars.size(); ++i){
    output_size += lists_of_cars.at(i).size();
    vector<bigint> intermediate(output_size);
    output.push_back(intermediate);
  }

  // if only one file in the list, return it
  if(lists_of_cars.size() == 0) cout << "Error in product_and_merge, no Carmichaels\n";
  if(lists_of_cars.size() == 1) return lists_of_cars.at(0);

  // copy over the first cars list into the first output list
  for(long i = 0; i < lists_of_cars.at(0).size(); ++i){
    output.at(0).at(i) = lists_of_cars.at(0).at(i);
  }

  // now a for loop to do the merges.  The first merge is the 0,1 lists -> output.at(1)
  for(long j = 1; j < output.size(); ++j){
    // merge takes start and end of the two inputs lists, and the start of the output list
    // the first input list is the previous output list, the other is the next cars list
    merge( output.at(j-1).begin(),      output.at(j-1).end(),
           lists_of_cars.at(j).begin(), lists_of_cars.at(j).end(),
           output.at(j).begin()
         );
  }
 
  // return the last output list; it is the result of all the merges
  return output.at(output.size()-1);
}


/* Given a filename full of Carmichael numbers of the form P q r, confirm that it is indeed Carmichael.
 * Involves completely factoring P, checking q and r are prime with the Pseudosquares test, checking Korselt.
   Any not carmichael are sent to standard out.  We assume preproducts P are bounded above by B
 */
void car_smallp_file_check(string filename, int64 B){

  // create a factor sieve
  long* nums = new long[B];
  for(long i = 0; i < B; i++){
    nums[i] = i;
  }
  // calling factor_sieve replaces nums[i] with the largest prime factor of i
  factor_sieve(nums, B);

  // create Pseudosquares class
  Pseudosquare ps = Pseudosquare();

  // open input file
  ifstream cars;
  cars.open(filename);

  // store line and the numbers in that line
  string line;
  vector<bigint> linenums;
  vector<bigint> primes;
  vector<long>   P_primes;
  bigint         P_primes_prod;
  bigint         product;
  long           P;
  bigint q; bigint r;

  // while there is a line to get, keep getting lines
  while(getline(cars, line)){
    // access each number in the line and place into a vector
    // this solution from stackoverflow: reading-line-of-integers-into-a-vector
    istringstream numbers_stream(line);
    linenums.clear();

    // notice this only works if the line has exactly three numbers
    // I tried using a while loop, but had problems with some variable not being intialized.  num?
    for(long i = 0; i < 3; ++i){
      bigint num;
      numbers_stream >> num;
      linenums.push_back(num);
    } // end for that reads the line

    // create a vector of the prime factors of the carmichael
    primes.clear();
    P_primes.clear();
    P = linenums.at(0);   // assume P is the first num in the line
    // add the prime factors of P
    P_primes = unique_prime_divs(P, nums, B);
    
    // check that they product to P
    P_primes_prod = 1;
    for(long i = 0; i < P_primes.size(); ++i){
      P_primes_prod *= P_primes.at(i);
    }
    // if not, print the line as an error
    if(P_primes_prod != P){
      cout << "Not a Carmichael due to P not squarefree: ";
      print_vec(linenums);
    }

    // copy over P_primes to primes;  Notice change from long to bigint
    for(long i = 0; i < P_primes.size(); ++i){
      primes.push_back(P_primes.at(i));
    }

    q = linenums.at(1);   r = linenums.at(2);
    // now check primality of q and r
    if(ps.is_prime_pssquare(q) && ps.is_prime_pssquare(r)){
      primes.push_back(q);   primes.push_back(r);
    }else{
      // if one is not prime, cout an error
      cout << "Not a Carmichael due to q or r not prime: ";
      print_vec(linenums);
    }
    
    // At this point we know squarefree and all prime, so check Korselt
    P_primes_prod *= q;
    P_primes_prod *= r;
    
    // I have chosen P_primes_prod to now represent n
    for(long i = 0; i < primes.size(); ++i){
      if(P_primes_prod % (primes.at(i) - 1) != 1){
        cout << "Not a Carmichael due to Korselt failure: ";
        print_vec(linenums);
        cout << "becase n = " << P_primes_prod << " whose primes are: ";
        print_vec(primes);
      }
    }

  } // end while(getline)
  cars.close();
  delete[] nums;
  return;
}
