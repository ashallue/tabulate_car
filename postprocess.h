/* Various functions used after a primary tabulation has been performed
 * Part of the tabulating Carmichael project, joint with Jonathan Webster.
 * Author: Andrew Shallue, Illinois Wesleyan University, written May 2020
 */
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include "bigint.h"
#include <algorithm>

using namespace std;

#ifndef POSTPROCESS_H
#define POSTPROCESS_H


/* Reads in a file of Carmichael numbers of the form P q r, computes the product, then sorts resulting list
 */
vector<bigint> product_and_sort(string cars_file);

/* Given a vector of filenames, call product_and_sort on each one, then merge them using the standard library.
 * Resulting output is of type vector<bigint> and should be all Carmichael numbers
 */
vector<bigint> product_and_merge(vector<string> filenames);

#endif
