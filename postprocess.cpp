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
  bigint num;

  // while there is a line to get, keep getting lines
  while(getline(cars, line)){
    // access each number in the line and place into a vector
    // this solution from stackoverflow: reading-line-of-integers-into-a-vector
    istringstream numbers_stream(line);
    linenums.clear();

    while(numbers_stream >> num){
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
