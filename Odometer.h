/* Andrew Shallue, Tabulating Carmichaels project.  2022
An odometer class, useful for stepping through all divisors.

This is the second version. First stores primes and powers in vectors, 
and the divisor as an exponent vector.  New version has these as arrays, 
and adds a private data member to store the current div as an integer.
*/

#include <vector>
#include "int.h"

using namespace std;

#ifndef ODOMETER_H
#define ODOMETER_H

// Step through all divisors of a number given in factored form.
// The leftmost entry is the least significant, that rotates most often
class Odometer{

  private:
    // primes and powers are intended to stay constant
    // the divisors generated are of this number
    int64* primes;
    long*  powers;
    long   num_length;

    // curr_prime is an index, curr_power is an exponent
    long curr_prime;
    long curr_power;

    // exponent array correpsonding to current divisor
    long* div_exp;
    int64 div;

  public:
    // default sets primes, powers to correspond to 2
    Odometer();

    // set the prime and powers
    Odometer(int64* ps, long* pows, long len);

    // destructor frees memory for primes, powers, div_exp
    ~Odometer();

    // rotate odometer, then update div
    void next_div();

    // print div_exp
    void print_exp();

    // return divisor corresponding to div_exp
    int64 get_div();

};

#endif
