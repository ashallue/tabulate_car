/* Class that implements the pseudosquares primality test.
 * Stores the pseudosquares, has a primality test, and has functions
 * for various test procedures.
 *
 * Andrew Shallue, part of carmichael tabulation joint with Jonathan Webster
 * June 2022
 *
 * List of pseudosquares found at https://cr.yp.to/focus.html#focus-paper
 * At the current time, the table has 73 entries.
 * The largest is L_{373} = 4235025223080597503519329, an 82-bit number.
 * So this class can prove primality for numbers up to 81 bits.
 */

#include <vector>
#include <iostream>
#include "bigint.h"

using namespace std;

#ifndef PSEUDOSQUARE_H
#define PSEUDOSQUARE_H

class Pseudosquare{
  public:
    // There are 73 entries in the table of pseudosquares
    static const int num_entries = 73;
    // pseudosquares are of the form L_p = s
    // store index p in index_primes, store the s in pssquares
    int index_primes[num_entries];
    bigint pssquares[num_entries];

    Pseudosquare();
};

#endif
