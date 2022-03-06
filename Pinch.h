/* Implementation of Pinch's algorithm for tabulating Carmichael numbers.
 * Code written by Andrew Shallue
 * Part of a project joint with Jonathan Webster
 *
 * Feb 2022
 *
 * Ref: Carmichael numbers up to 10^15, Math Comp, Vol 61, num 203, 1993
 * One change: pre-products are generated through Factgen incremental sieve, rather than the backtracking search proposed by Pinch.  
 * This is so that the pre-product generation does not affect timing comparisons between Pinch and Shallue-Webster
 */

#include "functions.h"
#include "Factgen.h"
#include "int.h"
#include "bigint.h"
#include "Odometer.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#ifndef PINCH
#define PINCH

class Pinch{
  private:
    Factgen F;    // object for incremental sieve

  public:
    // Tests whether a given pre-product P is admissable, 
    // i.e. that gcd(p-1, P) = 1 for all p | P, and squarefree
    // returns 0 if not admissable
    int64 admissable(int64 P, int64* Pprimes, long Pprimes_len);


    vector<pair<int64, bigint>> pinch_preproduct(int64* P, long P_len, int64 L);

    /* Construct Carmichaels for a range of pre-products P
 *  *   We use a factgen2 object, write to a file.  Only process pre-products that fall into residue class of the core.
 *   */
    void tabulate_car(int64 B, long processor, long num_threads, string cars_file, string admissable_file);
 
};


#endif
