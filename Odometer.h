/* Andrew Shallue, Tabulating Carmichaels project.  2022
An odometer class, useful for stepping through all divisors.

This is the second version. First stores primes and powers in vectors, 
and the divisor as an exponent vector.  New version has these as arrays, 
and adds a private data member to store the current div as an integer.

New addition (could say third versions): add an alternate way to step through
divisors, namely storing all divisors in an array and then next_div simply
goes through them linearly.

Another new addition: can specify a vector of primes, then all divisors must 
be divisible by the primes in the vector.
*/

#include <vector>
#include "int.h"
#include <algorithm>

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

    int64 multiple;

    // if store_divisors flag turned on, instead store all divisors in an array
    bool store_divisors;
    int64* all_divisors;
    long num_divisors;
    long curr_div_index;

  public:
    // the first divisor.  Important for detecting when odometer loops to start
    int64 initial_div;

    // default sets primes, powers to correspond to 2
    Odometer();

    // set the prime and powers
    // by default, do the space-efficient version.  If storage flag turned to true, 
    // this constructor will calculate and store all divisors in an array
    //
    // Addition: all divisors generated will be divisible by the multiple
    // This is implemented by generating all divisors, then multiplying by multiple at the end
    // No attempt made in this class to confirm that the multiple is "supposed" to divide the integer.
    Odometer(int64* ps, long* pows, long len, int64 given_multiple, bool storage = false);

    // destructor frees memory for primes, powers, div_exp
    ~Odometer();

    // rotate odometer, then update div
    // if store_divisors is true, simply move linearly to next divisor in array
    void next_div();

    // print div_exp
    void print_exp();

    // return divisor corresponding to div_exp
    int64 get_div();

  private:
    // recursive helper function that calculates divisors
    void create_divisors(long prime_index, long curr_position);

};

#endif
