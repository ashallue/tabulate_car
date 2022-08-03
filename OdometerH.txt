/* Andrew Shallue, Tabulating Carmichaels project.  2022
An odometer class, useful for stepping through all divisors.
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
    vector<int64> primes;
    vector<long>  powers;

    // curr_prime is an index, curr_power is an exponent
    long curr_prime;
    long curr_power;

    // exponent vector correpsonding to current divisor
    vector<long> div_exp;

  public:
    // default sets primes, powers to be empty vectors
    Odometer();

    // set the prime and powers
    Odometer(vector<int64> ps, vector<int64> pows);

    // rotate odometer, then update div
    void next_div();

    // print div_exp
    void print_exp();

    // return divisor corresponding to div_exp
    int64 get_div();

};

#endif