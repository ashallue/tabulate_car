// Code written by Jonathan Sorenson

#include "primetest.h"

// 32 and 64-bit strong pseudoprime test for the given base
// we have n-1=m*2^e with m odd
bool strong(int32 base, int32 n, int32 m, int16 e)
{
  int64 x=powmod(base, m, n);
  if(x==1 || x==n-1) return true;
  for(int i=0; i<e; i++)
  {
    x=(x*x)%n;
    if(x==n-1) return true;
    if(x==1) return false;
  }
  return false;
}
bool strong(int64 base, int64 n, int64 m, int16 e)
{
  int128 x=powmod(base, m, n);
  if(x==1 || x==n-1) return true;
  for(int i=0; i<e; i++)
  {
    x=(x*x)%n;
    if(x==n-1) return true;
    if(x==1) return false;
  }
  return false;
}

// fast test for 16-bit integers
// ψ2 = 1373653 > 2^15=32768
// so two strong ps tests will do it
bool primetest(int16 n)
{
//cout << "Prime test 16 bit of " << n << endl;
  if(n==1) return false;
  if(n==2 || n==3 || n==5) return true;
  if(n%2==0) return false; // n is even
  int16 m=(n-1)/2;
  int16 e=1;
  while(m%2==0) { e++; m=m/2; }
  if(!strong(2,n,m,e)) return false;
  return strong(3,n,m,e);
}

// fast test for 32-bit integers
// ψ4 = 3215031751 > 2^31=2147483648
// so four strong ps tests will do it
bool primetest(int32 n)
{
//cout << "Prime test 32 bit of " << n << endl;
  if(n==1) return false;
  if(n==2 || n==3 || n==5 || n==7) return true;
  if(n%2==0) return false; // n is even
  int32 m=(n-1)/2;
  int16 e=1;
  while(m%2==0) { e++; m=m/2; }
  if(!strong(2,n,m,e)) return false;
  if(!strong(3,n,m,e)) return false;
  if(!strong(5,n,m,e)) return false;
  return strong(7,n,m,e);
}

// test for 64-bit integers
// ψ9 = 3825 12305 65464 13051, below 2^63= 9223 37203 68547 75808,
// so we punt above this limit, for now
bool primetest(int64 n)
{
//cout << "Prime test 64 bit of " << n << endl;
  if(n<2147483648) return primetest((int32) n);
  if(n%2==0) return false; // n is even
  if(n<primetestcutoff) // we use 9 strong ps tests
  {
    int64 m=(n-1)/2;
    int16 e=1;
    while(m%2==0) { e++; m=m/2; }
    if(!strong((int64) 2,n,m,e)) return false;
    if(!strong((int64) 3,n,m,e)) return false;
    if(!strong((int64) 5,n,m,e)) return false;
    if(!strong((int64) 7,n,m,e)) return false;
    if(!strong((int64) 11,n,m,e)) return false;
    if(!strong((int64) 13,n,m,e)) return false;
    if(!strong((int64) 17,n,m,e)) return false;
    if(!strong((int64) 19,n,m,e)) return false;
    return strong((int64) 23,n,m,e);
  }
  else 
    return false; // for now...
}

/* Pocklington test written by Andrew Shallue.  
   First applies strong test to the first 5 prime bases.  If composite, provably composite.
   If the number passes, attempts to prove it is prime.  This part could be slow, but is 
   expected to take polynomial time.
   Input is n along with the prime factors of n-1.
*/
bool pocklington(const int64 &n, const vector<int64> &primes)
{
    // check divisibility by primes < 1000

    /* PowerMod will raise error if given 1 as an argument */
    if (n == 2 || n == 3 || n == 5 || n == 7 || n == 11) return true;
    // exclude even n > 2
    if (n % 2 == 0) return false;

    // Apply strong psp test to bases 2, 3, 5, 7, 11
    // first compute m, e
    int64 m = ( n - 1 ) / 2;
    int16 e = 1;
    while(m % 2 == 0) { e++; m = m / 2; }
    if(!strong((int64) 2,n,m,e)) return false;
    if(!strong((int64) 3,n,m,e)) return false;
    if(!strong((int64) 5,n,m,e)) return false;
    if(!strong((int64) 7,n,m,e)) return false;
    if(!strong((int64) 11,n,m,e)) return false;

    //cout << "past the strong tests\n";

    // in English: for every prime q | n-1, find a with a^(n-1) = 1 mod n, gcd(a^{(n-1)/q}-1, n) =1 
    // first get (pointers to) the primes of f

    // Output boolean is True unless proven otherwise
    bool output = true;

    // boolean for the inner loop
    bool done;
    // these are used in the inner loop for powers needed
    int64 full_power;
    int64 sans_q_power;
    // these variables will store gcds
    int64 g1; int64 g2; int64 g3;

    // Outer loop is over the prime_factors vector, so checking all q | n-1
    for (long i = 0; i < primes.size(); i++) {
        //cout << "prime = " << ps->at(i) << "\n";

        // break out of the loop if output is false, since that means n proven composite
        if (output == false) {
            break;
        }

        // we are looking for a generator a.  We start at 2 and continue until done
        done = false;
        int64 a = 2;
        while (!done) {
            //cout << "a = " << a << "\n";

            // compute the two powers needed
            full_power = powmod(a, n-1, n);
            sans_q_power = powmod(a, (n-1)/primes.at(i), n);
            sans_q_power -= 1;

            // compute gcds with n.  Unlikely to catch a factor, but worth checking
            g1 = gcd(a, n);
            g2 = gcd(full_power, n);
            g3 = gcd(sans_q_power, n);

            // if gcd(a,n) is nontrivial, then n composite
            if (g1 != 1 && g1 != n) {
                output = false;
                done = true;
            }
            // if gcd(full_power, n) is nontrivial, then n composite
            else if (g2 != 1 && g2 != n) {
                output = false;
                done = true;
            }
            // if gcd(sans_q_power, n) is nontrivial, then n composite
            else if (g3 != 1 && g3 != n) {
                output = false;
                done = true;
            }
            // if a^(n-1) = 1 mod n and gcd(a^{(n-1)/q}-1, n) = 1, we've passed for this prime
            else if (full_power == 1 && g3 == 1) {
                done = true;
                //cout << "for q = " << ps->at(i) << " found a = " << a << "\n";
            }
            // otherwise increment a and try again.  Stop if a = n
            else {
                a++;
                if (a == n) {
                    done = true;
                    output = false;
                }
            }
        } // end while loop

    } // end for loop
    return output;
}

/* Trial division by primes up to 1000.  
 *  * If output is false, provably composite.  If true, probably prime
 *   */
bool trial_thousand(const int64 &n){
  // note primes up to 1000 come from int.h
  int64 p = primes[0];
  long index = 0;

  // stop when we run out of primes, or if p >= r, or if factor found
  while (index < primeslen && p < n && n % p != 0){

  //  cout << "Trial division, n = " << n << " p = " << p << "\n";

    index++;
    p = primes[index];
  } // end while

  // now see if loop ended because factor found
  if (index == primeslen || p == n){
    return true;
  } else{
    return false;
  }
}

/* Checks if n is a perfect power of an integer or not.
 * Return value is a boolean - true if perfect power.
 */
bool perfect_pow(const bigint &n){

  int64 root;  // stores roots

  // strategy: check if n is a kth root for k up to log_2(n)
  long k_bound = ceil( log(n) / log(2) );

  for(long k = 2; k <= k_bound; ++k){
    // for each such k, take the root and round
    root = round( pow (n, 1.0 / k) );

    // now compute root^k and check if it equals n
    // if yes, return true.  if no, continue
    if( pow(root, k) == n ){
      return true;
    }
  }
  // if we have gotten to this point, not a perfect power
  return false;
}




