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
#include "functions.h"
#include "Pseudosquare.h"
#include "gmpprint.h"

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

/* Differs from the previous.  Assume the first number in each line is n and that the files are sorted by n.
 * Merge into a single outputfile
 */
void merge(vector<string> filenames, string outputfilename);

/* merge calls merge_two, which merges two files at a time
 */
void merge_two(string filename1, string filename2, string outputfilename);

/* Given a filename full of Carmichael numbers of the form P q r, confirm that it is indeed Carmichael.
 * Involves completely factoring P, checking q and r are prime with the Pseudosquares test, checking Korselt.
  Any not carmichael are sent to standard out.  We assume preproducts P are bounded above by B
 */
void car_smallp_file_check(string filename, int64 B);

/* Given filename with lines of the form n p1 p2 ... pd (pis prime factors of n), check if Carmichael.
 * Uses Pseudosquares class to do Pseudosquare primality proving on the pis, then checks Korselt.
 * Input stream is infile, carmichaels found written to carsfile, non-carmichaels written to mistakesfile.
 * Make sure to check Pseudosquares bound before running this function.
 */
void check_cars_factors(string infilename, string carsfilename, string mistakesfilename);

/* Given a filename with Carmichaels of the form n <factorization>, where the primes separated by spaces,
 * extract those which are less than the bound and have k prime factors.
 * Assumes the file is sorted by n.
 * Update: be careful with this function.  I think B should be bigint rather than int64
 */
void extract(string in_file, string out_file, int64 B, long k);

// given a file of carmichaels with full factorization, 
// filter out those that are < B, with preproducts < X
void filter_bounded(bigint B, bigint X, std::string infile, std::string outfile);

/* Solving the inverse problem: given (P q r) with P*q*r Carmichael,
 * print C, D, Delta
 * Recall: q - 1 = (P-1)(P+D)/Delta, r-1 = (P-1)(P+C)/Delta
 */
void inverse_problem(bigint P, bigint q, bigint r);

#endif
