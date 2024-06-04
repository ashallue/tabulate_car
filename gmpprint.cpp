#include "gmpprint.h"


ostream & operator<<(ostream & os, const mpz_t x){
  // allocate memory for c-string that can fit 1000 digits
  const int gmpprintstrlen = 1002;
  char gmpprintstr[gmpprintstrlen];

  // if less than the length bound, get the string version and feed it to the stream
  if(mpz_sizeinbase(x, 10) + 2 <= gmpprintstrlen){
    mpz_get_str(gmpprintstr, 10, x);
    os << gmpprintstr;

  // otherwise truncate the mpz and print that
  }else{
    os << mpz_get_d(x);
  }

  return os;
}

// function that takes a string made up of strings of digits separated by spaces
// I assume that the first number is an arbitrarily long n, and the rest are smaller factors
void parse_mpz(string char_nums, mpz_t &n, vector<bigint> &factors){

  // repeatedly do the following until the string is empty:
  // find the position of the next space, grab the number as a substring, convert to c_string and then to mpz
  int index = 0;
  int space_pos;
  string num_str;
  int count = 0;
  
  while(index < char_nums.length()){
    // find the space
    space_pos = char_nums.find(' ', index);
    cout << "in while loop, index = " << index << " and space_pos = " << space_pos << "\n";

    // substring args are start position, length of substring
    // if space_pos = -1, it means no space found, so go until the end of the string
    if(space_pos == -1){
      num_str = char_nums.substr(index);
    }else{
      num_str = char_nums.substr(index, space_pos - index);
    }

    // if count is 0, this is the first num, set n.  Otherwise add to the vector
    if(count == 0){
      mpz_set_str(n, num_str.c_str(), 10); 
    }else{
      factors.push_back(string_to_bigint(num_str));      
    }
    count++;
     
    // update index.  If space_pos is -1, we are at the end of the string
    if(space_pos == -1){
      index = char_nums.length();
    }else{
      index = space_pos + 1;
    }
  }
}

/* This was an idea I had for a read function, but I was getting ambiguity errors.
// Possibly it is the correct idea, but conflicts with an existing GMP function.
istream & operator>>(istream & is, mpz_t &x){
  // allocate memory for a c-string that can fit 1000 digits
  const int gmplen = 1002;
  char gmpintstr[gmplen];

  // gather characters until a space or newline is read
  char c;
  is >> c;
  int index = 0;
  while(c != ' ' && c!= '\n'){
    gmpintstr[index] = c;
 
    // get the next character
    is >> c;
    index++;
  }
  // put an end character
  gmpintstr[index] = '\0';

  // now set the mpz and return the stream
  mpz_set_str(x, gmpintstr, 10);
  return is;
  
}
*/

