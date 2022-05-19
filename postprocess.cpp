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
