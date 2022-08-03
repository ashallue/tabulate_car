// Code by Jonathan Sorenson

#include "bigint.h"

void tostring(bigint x,char s[])
{
  if(x==0) { s[0]='0'; s[1]=0; return; }
  int digitstack[100];
  int top=0;
  int sign=1;
  if(x<0) {sign=-1; x=-x;}
  while(x>0) { digitstack[top++]=x%10; x=x/10; }
  int spos=0;
  if(sign<0) s[spos++]='-';
  while(top>0) s[spos++]='0'+digitstack[--top];
  s[spos]=0;
}

void print(bigint x, ostream & os)
{
  char s[100];
  tostring(x,s);
  os << s;
/*
  if(x<0) { os << "-"; print(-x,os); return; }
  if(x<10) { os << ((int) x ); return; }
  print(x/10,os);
  os << ( (int)(x%10));
*/
}

bigint atobi(char tmp[])
{
  bigint x=0;
  int i=0, sign=1;
  if(tmp[0]=='-') { sign=-1; i++; }
  if(tmp[0]=='+') { i++; }
  for(; i<strlen(tmp); i++)
    if('0'<=tmp[i] && tmp[i]<='9')
      { x=x*10+(tmp[i]-'0'); }
    else break;
  x *= sign;

  return x;
}

void read(bigint &x, istream &is)
{
  char tmp[100];
  is >> tmp;
  x=atobi(tmp);
}

ostream & operator<<(ostream & os, bigint x)
{
  print(x,os); return os;
}

istream & operator>>(istream & is, bigint &x)
{
  read(x,is); return is;
}

bigint min(bigint x, bigint y)
  { return x<y ? x : y; }


// code from this point on written by Andrew Shallue


// compute a^e mod n
// Uses mpz_t for the intermediate values, and uses gmp pow_mod
bigint pow_mod(bigint a, bigint e, bigint n){
  bigint output;


  return output;
}


/* A function that converts a string of digits into an integer of type bigint.
 * Performs a size check to make sure it is at most 128 bits
 */
bigint string_to_bigint(string num){
  // set up mpz_t for quotient, remainder, and input
  mpz_t input_num; mpz_init(input_num);
  mpz_t q;         mpz_init(q);
  mpz_t r;         mpz_init(r);

  // convert from string to character array (annoying)
  // I need extra length in my character array to account for the \0 character strcpy adds
  char* num_chars = new char[num.length() + 1];
  strcpy(num_chars, num.c_str()); 

  // set value of the input num, test its size.  Limit to 127 to make room for neg sign
  mpz_set_str(input_num, num_chars, 10);
  long int* num_bits = new long int;
  double d = mpz_get_d_2exp(num_bits, input_num);

  if(*num_bits > 127){
    cout << "Error in string_to_bigint, input num too big: " << num << "\n";
    return -1;
  }
  
  // now divide by 2^64 and capture the quotient and remainder, then convert them to signed ints 
  mpz_tdiv_q_2exp(q, input_num, 64);
  mpz_tdiv_r_2exp(r, input_num, 64);
  unsigned long q_int = mpz_get_ui(q);
  unsigned long r_int = mpz_get_ui(r);

  // use Dual_rep to convert to a bigint
  Dual_rep conversion;
  conversion.two_words[1] = q_int;
  conversion.two_words[0] = r_int;

  // free up memory
  delete[] num_chars;
  delete num_bits;
  mpz_clear(input_num); mpz_clear(q); mpz_clear(r);

  // return the bigint out of the Dual rep
  return conversion.double_word;
}

// convert from bigint n to mpz_t type m (passed by reference)
void bigint_to_mpz(bigint n, mpz_t &m){
  // we will need a Dual_rep to manage two words
  Dual_rep d;
  d.double_word = n;
 
  // now set the high bits.  It is signed since n is signed
  mpz_set_si(m, d.two_words[1]);

  // shift left 64 places, then add in low bits (unsigned to get full 64)
  mpz_mul_2exp(m, m, 64);
  mpz_add_ui(m, m, d.two_words[0]);

  return;
}


