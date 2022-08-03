/* Class that implements pseudosquare primality test.
 * Andrew Shallue, part of Carmichael tabulation join with Jonathan Webster
 * June 2022
 */

#include "Pseudosquare.h"

using namespace std;

// computation of jacobi symbol (a | n).  From Chapter 5 of Bach/Shallit
// For the algorithm to work, we need 0 < a < n and n odd
int Pseudosquare::jacobi(bigint a_in, bigint n_in){
  // copy inputs into new variables
  bigint a = a_in;    bigint n = n_in;

  // code to ensure a, n satisfy necessary requirements
  if(n % 2 == 0) cout << "Error in jacobi, input n is not odd. a = " << a << " n = " << n << "\n";
  a = a % n;
  if(a == 0) return 0;

  // not sure what this variable is for
  long t = 1;
  // temp used while swapping a and n
  bigint temp = 0;  

  // continue until a is 0
  while(a != 0){
    //cout << "Top of the loop, a = " << a << " n = " << n << "\n";

    // employ quadratic reciprocity in the next several lines
    while(a % 2 == 0){
      a = a / 2;
      if(n % 8 == 3 || n % 8 == 5){
        t = -1 * t;
      }
      
    } // end while a even
    
    // now swap a and n
    temp = a;
    a = n;
    n = temp;
   
    //cout << "After swap, a = " << a << " n = " << n << "\n";

    if(a % 4 == 3 && n % 4 == 3){
      t = -1 * t;
    }
    // reduce a
    a = a % n;

  } // end while a not 0
  // return
  if(n == 1){
    return t;
  }else{
    return 0;
  }
} 


// print primes with their corresponding L_p
void Pseudosquare::print(){
  cout << "Pseudosquares:\n";
  for(long i = 0; i < num_entries; ++i){
    cout << index_primes[i] << ": " << pssquares[i] << "\n";
  }
  cout << "\n";
  
  return;
}

// A function that tests if n is a pseudosquare with respect to p,
// i.e. n = 1 mod 8 and (n | q) = 1 for all odd primes q <= p.
// Does not check that n is the smallest integer with this property.
bool Pseudosquare::is_pseudosquare(bigint n, long p){
  //cout << "is_pseudosquare, input n = " << n << " p = " << p << "\n";
  
  // start by assuming n does satisfy conditions, flip to false if it doesn't
  bool output = true;
  long i = 0; 
 
  // check congruent to 1 mod 8
  if(n % 8 != 1){
    output = false;
  }else{
    // access primes via the primes array
    while(index_primes[i] <= p){
      if(jacobi(n, index_primes[i]) != 1){

        output = false;
        break;
      }
      // if we are at the end of the primes list, stop
      if(i < num_entries - 1){
        ++i;   // increment index
      }else{
        cout << "At the end of the primes array\n";
        break;
      }     
    }
  }
  // return
  return output;
}

// Check all the pseudosquares in stored in the class arrays.  Output is all printed.
// I am checking that is_pseudosquare returns true for the corresponding p.
// I am not checking that the n is the smallest with respect to that p.
void Pseudosquare::check_pseudosquares(){
  // count number of successful checks
  long count_good = 0;

  // loop over all entries
  for(long i = 0; i < num_entries; ++i){
    // check that it is a pseudosquare with respect to the corresponding prime
    if(is_pseudosquare(pssquares[i], index_primes[i])){
      count_good++;
      cout << "Good, n = " << pssquares[i] << " is a pseudosquare with respect to " << index_primes[i] << "\n";
    }else{
      cout << "Problem, n = " << pssquares[i] << " is not a pseudosquare with respect to " << index_primes[i] << "\n";
    }

  }  // end for 
  if(count_good == num_entries){
    cout << "All pseudosquares passed the check\n";
  }else{
    cout << "Not all entries are pseudosquares\n";
  }
  return;
}

// constructor 
Pseudosquare::Pseudosquare(){
  // manually set all 73 of the primes that index the pseudosquares
  index_primes[0] = 3;
  index_primes[1] = 5;
  index_primes[2] = 7;
  index_primes[3] = 11;
  index_primes[4] = 13;
  index_primes[5] = 17;
  index_primes[6] = 19;
  index_primes[7] = 23;
  index_primes[8] = 29;
  index_primes[9] = 31;
  index_primes[10] = 37;
  index_primes[11] = 41;
  index_primes[12] = 43;
  index_primes[13] = 47;
  index_primes[14] = 53;
  index_primes[15] = 59;
  index_primes[16] = 61;
  index_primes[17] = 67;
  index_primes[18] = 71;
  index_primes[19] = 73;

  index_primes[20] = 79;
  index_primes[21] = 83;
  index_primes[22] = 89;
  index_primes[23] = 97;
  index_primes[24] = 101;
  index_primes[25] = 103;
  index_primes[26] = 107;
  index_primes[27] = 109;
  index_primes[28] = 113;
  index_primes[29] = 127;
  index_primes[30] = 131;
  index_primes[31] = 137;
  index_primes[32] = 139;
  index_primes[33] = 149;
  index_primes[34] = 151;
  index_primes[35] = 157;
  index_primes[36] = 163;
  index_primes[37] = 167;
  index_primes[38] = 173;
  index_primes[39] = 179;
  
  index_primes[40] = 181;
  index_primes[41] = 191;
  index_primes[42] = 193;
  index_primes[43] = 197;
  index_primes[44] = 199;
  index_primes[45] = 211;
  index_primes[46] = 223;
  index_primes[47] = 227;
  index_primes[48] = 229;
  index_primes[49] = 233;
  index_primes[50] = 239;
  index_primes[51] = 241;
  index_primes[52] = 251;
  index_primes[53] = 257;
  index_primes[54] = 263;
  index_primes[55] = 269;
  index_primes[56] = 271;
  index_primes[57] = 277;
  index_primes[58] = 281;
  index_primes[59] = 283;

  index_primes[60] = 293;
  index_primes[61] = 307;
  index_primes[62] = 311;
  index_primes[63] = 313;
  index_primes[64] = 317;
  index_primes[65] = 331;
  index_primes[66] = 337;
  index_primes[67] = 347;
  index_primes[68] = 349;
  index_primes[69] = 353;
  index_primes[70] = 359;
  index_primes[71] = 367;
  index_primes[72] = 373;

  // now set the pssquares array that holds the pseudosquare values
  pssquares[0] = string_to_bigint("73");
  pssquares[1] = string_to_bigint("241");
  pssquares[2] = string_to_bigint("1009");
  pssquares[3] = string_to_bigint("2641");
  pssquares[4] = string_to_bigint("8089");
  pssquares[5] = string_to_bigint("18001");
  pssquares[6] = string_to_bigint("53881");
  pssquares[7] = string_to_bigint("87481");
  pssquares[8] = string_to_bigint("117049");
  pssquares[9] = string_to_bigint("515761");
  pssquares[10] = string_to_bigint("1083289");
  pssquares[11] = string_to_bigint("3206641");
  pssquares[12] = string_to_bigint("3818929");
  pssquares[13] = string_to_bigint("9257329");
  pssquares[14] = string_to_bigint("22000801");
  pssquares[15] = string_to_bigint("48473881");
  pssquares[16] = string_to_bigint("48473881");
  pssquares[17] = string_to_bigint("175244281");
  pssquares[18] = string_to_bigint("427733329");
  pssquares[19] = string_to_bigint("427733329");

  pssquares[20] = string_to_bigint("898716289");
  pssquares[21] = string_to_bigint("2805544681");
  pssquares[22] = string_to_bigint("2805544681");
  pssquares[23] = string_to_bigint("2805544681");
  pssquares[24] = string_to_bigint("10310263441");
  pssquares[25] = string_to_bigint("23616331489");
  pssquares[26] = string_to_bigint("85157610409");
  pssquares[27] = string_to_bigint("85157610409");
  pssquares[28] = string_to_bigint("196265095009");
  pssquares[29] = string_to_bigint("196265095009");
  pssquares[30] = string_to_bigint("2871842842801");
  pssquares[31] = string_to_bigint("2871842842801");
  pssquares[32] = string_to_bigint("2871842842801");
  pssquares[33] = string_to_bigint("26250887023729");
  pssquares[34] = string_to_bigint("26250887023729");
  pssquares[35] = string_to_bigint("112434732901969");
  pssquares[36] = string_to_bigint("112434732901969");
  pssquares[37] = string_to_bigint("112434732901969");
  pssquares[38] = string_to_bigint("178936222537081");
  pssquares[39] = string_to_bigint("178936222537081");
  
  pssquares[40] = string_to_bigint("696161110209049");
  pssquares[41] = string_to_bigint("696161110209049");
  pssquares[42] = string_to_bigint("2854909648103881");
  pssquares[43] = string_to_bigint("6450045516630769");
  pssquares[44] = string_to_bigint("6450045516630769");
  pssquares[45] = string_to_bigint("11641399247947921");
  pssquares[46] = string_to_bigint("11641399247947921");
  pssquares[47] = string_to_bigint("190621428905186449");
  pssquares[48] = string_to_bigint("196640248121928601");
  pssquares[49] = string_to_bigint("712624335095093521");
  pssquares[50] = string_to_bigint("1773855791877850321");
  pssquares[51] = string_to_bigint("2327687064124474441");
  pssquares[52] = string_to_bigint("6384991873059836689");
  pssquares[53] = string_to_bigint("8019204661305419761");
  pssquares[54] = string_to_bigint("10198100582046287689");
  pssquares[55] = string_to_bigint("10198100582046287689");
  pssquares[56] = string_to_bigint("10198100582046287689");
  pssquares[57] = string_to_bigint("69848288320900186969");
  pssquares[58] = string_to_bigint("208936365799044975961");
  pssquares[59] = string_to_bigint("533552663339828203681");

  pssquares[60] = string_to_bigint("936664079266714697089");
  pssquares[61] = string_to_bigint("936664079266714697089");
  pssquares[62] = string_to_bigint("2142202860370269916129");
  pssquares[63] = string_to_bigint("2142202860370269916129");
  pssquares[64] = string_to_bigint("2142202860370269916129");
  pssquares[65] = string_to_bigint("13649154491558298803281");
  pssquares[66] = string_to_bigint("34594858801670127778801");
  pssquares[67] = string_to_bigint("99492945930479213334049");
  pssquares[68] = string_to_bigint("99492945930479213334049"); 
  pssquares[69] = string_to_bigint("295363487400900310880401");
  pssquares[70] = string_to_bigint("295363487400900310880401");
  pssquares[71] = string_to_bigint("3655334429477057460046489");
  pssquares[72] = string_to_bigint("4235025223080597503519329");
 
} 

