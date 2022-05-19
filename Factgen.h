/* Generate factorizations of numbers in a range using an incremental sieve.
   Code written by Jonathan Sorenson
   Comments by Andrew Shallue, Factgen2 by Andrew Shallue.

   Note: only works for factorizations up to 20 primes.

   Key function is next().  This takes all the primes in the current factorization, 
   and for each such p pushes it onto the factorization stack for n+p, then increments n 
   and position.  In this way we get the next factorization in constant time, with a 
   total memory requirement which is sqrt(B), ignoring log factors.

   These factorizations are not complete, in the sense that if p^r divides n, p 
   appears only once in the list of factors.  But if p | n, then p must appear, 
   unless n is a prime > start + sqrt(stop), in which case the factorization list is empty.
*/


#ifndef FACTGEN_H
#define FACTGEN_H

#include "Stack.h"
#include <cmath>
#include "int.h"
#include "primetest.h"
#include <vector>
#include "functions.h"

using namespace std;

class Factgen
{
private:

  Stack *roll;  // An array of stacks, stores factorizations for all numbers in the interval
  int rollsize; // memory allocated for roll, which is sqrt(B)
  int pos;      // tracks n, but modulo rollsize, starting at 0

public:
  // public attributes
  int64 prev[20];      // Factorization of previous n
  int prevlen;         // length of factorization of prevn
  int64 prevn;         // previous n
  int64 start, stop;   // upper and lower bounds on the sieve
  int64 n;  

  // public member functions
  Factgen();    // not useful, default values
  ~Factgen();   // delete the roll

  void print();  // print top of the stack
  
  // I would have made this a non-standard constructor
  void init(int64 startint, int64 stopint);

  // get the next factorization
  void next();

  // A number is prime if its top factor is itself
  bool isprime();

  // find next prime through repeated calls to next()
  int64 nextprime();

  // Generate a vector of all divisors of prevn from the list of prime factors
  vector<int64> prevn_divisors();

};

// Stores and updates 2 factorizations, namely for n and n-1
// Written by Andrew Shallue, based on code by Jonathan Webster and Jonathan Sorenson

/* Update: if you don't call next, then the initial numbers are startint and startint+1

*/
class Factgen2
{
private:
  Factgen G;

public:
  // store the two factorizations in one array, starting at positions 0 and 20
  int64 factors[40];

  // arrays to break factors into the two lists of factors
  int64* prev;
  int64* current;

  // lengths and values
  int prevlen;
  int64 prevval;

  int currentlen;
  int64 currentval;

  // initialize, assign memory
  void init(int64 startint, int64 stopint);

  // call Factgen next(), and update previous and current
  void next();

  // print both prev and current factorizations
  void print();

  // returns true if current is prime
  bool isprime_current();

};


#endif
