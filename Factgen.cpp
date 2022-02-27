/* Generate factorizations of numbers in a range using an incremental sieve.
   Code written by Jonathan Sorenson
   Comments by Andrew Shallue, Factgen2 by Andrew Shallue.

   Note: only works for factorizations up to 20 primes.
*/

#include "Factgen.h"

using namespace std;

// not useful, default values
Factgen::Factgen(){

  // roll had space for one Stack
  roll = nullptr;
  prevlen = 0;
  prevn = 0;
  start = 0;
  stop = 0;
  n = 0;
}

// delete the roll
Factgen::~Factgen(){
  delete[] roll;
}  
  
// I would have made this a non-standard constructor
// Complexity is O(sqrt(B)) time and space
void Factgen::init(int64 startint, int64 stopint){

  // clear the roll, then re-allocate
  // memory needed is double the squareroot of the upper bound
  if(roll != NULL) delete[] roll;
  rollsize = 1 + sqrt(stopint);
  rollsize = 2 * rollsize;
  roll = new Stack[rollsize];

  // set stop and start, then prev stuff to 0
  // Note that pos corresponds to n.  Initially, n = start and pos = 0
  stop = stopint;
  start = startint;
  n = start;
  prevlen = 0;
  prevn = 0;
  pos = 0;

  // for primes up to the rollsize, push p onto the stack 
  // at position (p - start % p) % p.  Notice each prime only 
  // pushed onto one stack.
  for(long p = rollsize - 1; p >= 2; p--)
    if(primetest(p))
    {
      roll[ (p - (start % p)) % p ].push(p);
    }
//cout << "rollsize=" << rollsize << endl;
}

/* As an example, let (start, stop) = (2, 100).  Let p = 5.
Then (p - (start % p)) % p = (5 - 2) % 5 = 3.  Pos 3 corresponds to n 5.
Let (start, stop) = (10, 100).  Let p = 5.
Then (5 - (10 % 5)) % 5 = 0.  And 5 is a factor of 10.
*/

// get the next factorization
void Factgen::next(){
  // copy stuff & factor
  int64 r = n;
  prevn = n; prevlen = 0;

  // here is the factorization loop
  // continue until no more primes to grab
  while( !roll[pos].isempty() )
  {
    // pop prime p off of factorization for n, 
    // then add to factorization of the next multiple of p
    int64 p = roll[pos].pop();
    roll[ (pos + p) % rollsize ].push(p);

    // add p to factorization of previous n
    prev[ prevlen++ ] = p;
    while(r % p == 0) { r = r / p; } // cout << "div\n"; }
  }
  // It is possible that n is prime and greater than rollsize.
  // If so, then its stack will be empty.  We detect this by checking r, then add it to factorization.
  if(r > 1)
    { prev[ prevlen++ ] = r; }

  // increment n, and increment position to match
  n++;
  pos = (pos + 1) % rollsize;
}

// A number is prime if its top factor is itself
bool Factgen::isprime(){
  return (roll[pos].isempty() || roll[pos].gettop() == n);
}

// find next prime through repeated calls to next()
int64 Factgen::nextprime(){
  do{
    next();
  }while(prevlen != 1);    // stop when prevn is prime
  return prev[0];
}

void Factgen::print()
  {
    cout << "n=" << n << endl;
    cout << "Empty?" << roll[pos].isempty() << endl;
    cout << "stack top=" << roll[pos].gettop() << endl;
  }

// Generate a vector of all divisors of prevn from the list of prime factors
vector<int64> Factgen::prevn_divisors(){

  // initialize output divisors list with 1
  vector<int64> divs;
  divs.push_back(1);

  // variables for a copy, for the prime powers
  vector<int64> divs_copy;
  vector<int64> divs_temp = divs;
  int64 p;
  int64 p_pow = 1;
  vector<int64> p_powers;

  // consider all primes in the prevs array
  for(long i = 0; i < prevlen; ++i){ 

    // capture current prime and calculate prime powers
    p = prev[i];
    p_powers.clear();
    p_pow = p;
    while(prevn % p_pow == 0){
      p_powers.push_back(p_pow);
      p_pow = p_pow * p;
    }
    // loop stops when p_pow is one step too big
    p_pow = p_pow / p;

    // For each prime power in turn, multiply it by all the divs (using divs_copy)
    // then push onto the divs_temp vector.
    // After all prime powers processed, copy divs_temp over to divs.
    for(long j = 0; j < p_powers.size(); ++j){
      divs_copy = divs;
      for(long k = 0; k < divs_copy.size(); ++k){
        divs_copy.at(k) = divs_copy.at(k) * p_powers.at(j);
        divs_temp.push_back(divs_copy.at(k));
      } // end for over divs_copy
    } // end for over p_powers

    // now set divs to divs_temp
      divs = divs_temp;
      
  } // end for over the prev array

  return divs;
}

/*********************************
* Methods for Factgen2
**********************************/

// initialize, assign memory
void Factgen2::init(int64 startint, int64 stopint)
{
  // initialize the Factgen
  G.init( startint, stopint);
  G.next();  // call next so that there is a prev 
  
  prev = &factors[0];
  current = &factors[20];
  
  // set prev in Factgen2
  memcpy(prev, G.prev, sizeof G.prev);
  
  prevlen = G.prevlen;
  prevval = G.prevn ;
  
  // get the next factorization
  G.next();

  // now set current as the new prev
  memcpy(current, G.prev, sizeof G.prev);
  currentlen = G.prevlen;
  currentval = G.prevn ;
  
}

// call Factgen next(), and update previous and current
void Factgen2::next()
{
  G.next();
  
  // swap prev and current.  Important part is prev takes on the factors of current
  int64* temp = prev;
  prev = current;
  current = temp;

  // copy from G.prev to current
  memcpy(current, G.prev, sizeof G.prev);
  
  // reset len and val variables
  prevlen = currentlen;
  prevval = currentval;
  
  currentlen = G.prevlen;
  currentval = G.prevn;
}

// print both prev and current factorizations
void Factgen2::print(){
  // print factors for prev
  cout << prevval << ": ";
  for(long i = 0; i < prevlen; ++i){
    cout << prev[i] << " ";
  }
  cout << " , ";

  // print factors for current
  cout << currentval << ": ";
  for(long i = 0; i < currentlen; ++i){
    cout << current[i] << " ";
  }
  cout << "\n";

}