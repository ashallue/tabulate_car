/* Andrew Shallue, Tabulating Carmichaels project.  2022
 * This class tabulates Carmichaels with large pre-products.
 * It utilizes LargeP_Odometer to generate large pre-products, then this class
 * finds primes q and r so that Pqr is Carmichael.
 * Note that primality of q and r is based on the Baille-PSW pseudoprime test.
 */

using namespace std;

#ifndef TABULATE_LARGE
#define TABULATE_LARGE

#include "bigint.h"

class Tabulate_large{
  private:
    LargeP_Odometer od;
  public:
    bigint car_bound;   // bound on the Carmichaels constructed
    long   P_bound;     // lower bound on pre-products

  public:
    Tabulate_large();


};

#endif 
