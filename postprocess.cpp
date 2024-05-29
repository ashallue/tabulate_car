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

/* Merge two files.  Assume files are sorted by the first number in the line, merge into single outputfile
 * Standard algorithm.
 */
void merge_two(string filename1, string filename2, string outputfilename){
  // open files
  ifstream cars1;
  cars1.open(filename1);
  ifstream cars2;
  cars2.open(filename2);
  ofstream output;
  output.open(outputfilename);

  // variables
  string line1, line2;
  vector<bigint> nums1;
  vector<bigint> nums2;
  bigint num1 = 0;
  bigint num2 = 0;

  // grab the first line of each file
  getline(cars1, line1);
  getline(cars2, line2);

  // first keep looping as long as both files have lines left
  while(cars1 && cars2){

    // access first number in each line using istringstream
    // this solution from stackoverflow: reading-line-of-integers-into-a-vector
    istringstream numbers1_stream(line1);
    numbers1_stream >> num1;
    istringstream numbers2_stream(line2);
    numbers2_stream >> num2;

    // check the two numbers.  Smaller number: line goes to output, advance through file
    if(num1 <= num2){
      output << line1 << "\n";
      getline(cars1, line1);
    }else{
      output << line2 << "\n";
      getline(cars2, line2);
    } 
  } // end while getline

  // if file1 is empty, append all the lines of file2
  if(!cars1){
    while(cars2){
      output << line2 << "\n";
      getline(cars2, line2);
    }
  }
  // same for file2
  if(!cars2){
    while(cars1){
      output << line1 << "\n";
      getline(cars1, line1);
    }
  }

  // close files
  cars1.close();
  cars2.close();
  output.close();  
}


/* Differs from the previous version of merge.  
 * Assume the first number in each line is n and that the files are sorted by n.
 * Merge into a single outputfile
 */
void merge(vector<string> filenames, string outputfilename){
  // merge the first two files
  merge_two(filenames.at(0), filenames.at(1), outputfilename);  

  // Repeatedly call merge_two on the rest
  // Note: can't combine with previous output into new output without a different filename
  // So this code creates a temp file which should be deleted afterwards
  string temp = "merge_temp.txt";

  for(long i = 2; i < filenames.size(); ++i){
    if(i % 2 == 0){
      merge_two(filenames.at(i), outputfilename, temp);
    }else{
      merge_two(filenames.at(i), temp, outputfilename);
    }
  }

  // at the end, I want the output in outputfilename, not temp
  if(filenames.size() % 2 == 1) merge_two(temp, outputfilename, outputfilename);

}

/* Given a filename full of Carmichael numbers of the form P q r, confirm that it is indeed Carmichael.
 * Involves completely factoring P, checking q and r are prime with the Pseudosquares test, checking Korselt.
   Any not carmichael are sent to standard out.  We assume preproducts P are bounded above by B
   Note that using B here is bad notation.  Would have been better to use X, as X is our usual preproduct bound.
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

/* Given filename with lines of the form n p1 p2 ... pd (pis prime factors of n), check if Carmichael.
 * Uses Pseudosquares class to do Pseudosquare primality proving on the pis, then checks Korselt.
 * Input stream is infile, carmichaels found written to carsfile, non-carmichaels written to mistakesfile.
 * Make sure to check Pseudosquares bound before running this function.
 */
void check_cars_factors(string infilename, string carsfilenames, string mistakesfilename){
  // open the three files
  ifstream infile = open(infilename);
  ifstream carsfile = open(carsfilename, "w");
  ifstream mistakes = open(mistakesfilename, "w");

  // variables
  string line;
  vector<bigint> linenums; 
  bigint num;

  // get the first line, then loop over input file
  getline(infile, line);
  while(line){
  
    // use an istringstream to get the numbers in a line
    linenums.clear();
    istringstream numbers_stream(line);
    
    while(numbers_stream){ 
      // access nums, put them in the linenums vector
      numbers_stream >> num;
      linenums.push_back(num);
    }
 
    // testing
    cout << "line read was " << line << "\n";
    cout << "this parsed as ";
    for(long i = 0; i < linenums.size(); i++){
      cout << linenums.at(i) << " "; 
    }       
    cout << "\n";

    getline(infile, line);
  } // end while line
}

/* Given a filename with Carmichaels of the form n <factorization>, where the primes separated by spaces,
 * extract those which are less than the bound and have k prime factors.
 * Assumes the file is sorted by n.
 */
void extract(string in_file, string out_file, int64 B, long k){
  // create file objects and open them
  ifstream input_nums;
  input_nums.open(in_file);

  ofstream output_nums;
  output_nums.open(out_file);

  string line;
  vector<bigint> linenums;
  bigint num;
  char c_numstring[128]; 
  string numstring;

  // while there is a line to get, keep getting lines
  while(getline(input_nums, line)){
    // access each number in the line and place into a vector
    // this solution from stackoverflow: reading-line-of-integers-into-a-vector
    istringstream numbers_stream(line);
    linenums.clear();

    // notice this only works if the line has exactly three numbers
    // I tried using a while loop, but had problems with some variable not being intialized.  num?
    while(numbers_stream >> num){
      linenums.push_back(num);
    } // end for that reads the line

    // keep it if n < B (the 0 index num) and the count of factors is k (size - 1)
    if(linenums.at(0) >= B) break;
    if(linenums.size() - 1 == k){
      //tostring(linenums.at(0), c_numstring);
      //numstring = string(c_numstring);
      output_nums << line << "\n";
    }

  } // end while

  input_nums.close();
  output_nums.close();
}

/* Solving the inverse problem: given (P q r) with P*q*r Carmichael,
 * print C, D, Delta
 * Recall: q - 1 = (P-1)(P+D)/Delta, r-1 = (P-1)(P+C)/Delta
 */
void inverse_problem(bigint P, bigint q, bigint r){
  cout << "Given P = " << P << ", q = " << q << ", r = " << r << "\n";
  bigint C, D, Delta;
  

  // dumbest version: we will simply search linearly through D values
  for(D = 1; D < P * P; ++D){
    // check if q-1 is a divisor.  If so retrieve Delta
    if( ((P-1) * (P+D)) % (q-1) == 0 ){
      Delta = ((P-1) * (P+D)) / (q-1);

      // since Delta = CD - P^2, C = (Delta + P^2) / D
      if( (Delta + P * P) % D == 0 ){
        C = (Delta + P * P) / D;

        // now check that r matches.  If it does, print and return
        if( ((P-1) * (P+C)) % Delta == 0 && r - 1 == ((P-1) * (P + C)) / Delta ){
          cout << "C = " << C << ", D = " << D << ", Delta = " << Delta << "\n";
          return;
        }
      }
    }
  }
  cout << "Solution failed\n";
  return;
}
