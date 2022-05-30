/* Andrew Shallue, May 2022
 * Part of tabulating Carmichaels project with Jonathan Webster
 *
 * This file does testing between 64-bit ints, 128-bit ints, and mpz_t
 */

#include "bigint.h"
#include "int.h"
#include <iostream>
#include "gmp.h"
#include <math.h>

using namespace std;

// represent a 128 bit integer either as a bigint or as two 64-bit ints
union Dual_rep{
  bigint double_word;
  unsigned long int two_words[2];
};

int main(){
  cout << "hello world\n";

  // create ints requiring < 64 bits and > 64 bits
  int64 small;
  bigint large;

  small = pow(2, 10);
  large = pow(2, 100);
  large = large + small;

  cout << small << "\n";
  cout << large << "\n";

  // cast to mpz_t
  mpz_t small_zz;
  mpz_t large_zz;
  mpz_inits(small_zz, large_zz);
  mpz_init(small_zz);

  mpz_set_si(small_zz, small);
  mpz_set_si(large_zz, large);

  cout << "mpz versions:\n";
  mpz_out_str(nullptr, 10, small_zz);
  cout << "\n";
  mpz_out_str(nullptr, 10, large_zz);
  cout << "\n";

  // as one can see from running the above, 128bit data types don't 
  // convert nicely to mpz.  So I need to do something.
  // Using union, I can store the information as 128 bits or 64[2]
  Dual_rep d;
  //d.two_words[0] = 0;
  //d.two_words[1] = 0;
  d.double_word = large;
  cout << "Dual_rep version:";
  cout << "double_word: " << d.double_word << "\n";
  cout << "two_words: " << d.two_words[0] << " " << d.two_words[1] << "\n";

  // notice the output is 0, followed by 2^(100-64) = 2^(36)
  // now convert to mpz_t
  mpz_set_si(large_zz, d.two_words[1]);
  
  /*
  mpz_t large_upper;
  mpz_t large_lower;
  mpz_init_set_si(large_upper, d.two_words[1]);
  mpz_init_set_si(large_lower, d.two_words[0]);  
  */

  mpz_mul_2exp(large_zz, large_zz, 64);
  mpz_add_ui(large_zz, large_zz, d.two_words[0]);

  cout << "Conversion to mpz_t: \n";
  mpz_out_str(nullptr, 10, large_zz);
  cout << "\n";     

}
