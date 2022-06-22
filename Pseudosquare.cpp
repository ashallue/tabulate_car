/* Class that implements pseudosquare primality test.
 * Andrew Shallue, part of Carmichael tabulation join with Jonathan Webster
 * June 2022
 */

#include "Pseudosquare.h"

using namespace std;

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
  pssquares[0] = 73;
  pssquares[1] = 241;
  pssquares[2] = 1009;
  pssquares[3] = 2641;
  pssquares[4] = 8089;
  pssquares[5] = 18001;
  pssquares[6] = 53881;
  pssquares[7] = 87481;
  pssquares[8] = 117049;
  pssquares[9] = 515761;
  pssquares[10] = 1083289;
  pssquares[11] = 3206641;
  pssquares[12] = 3818929;
  pssquares[13] = 9257329;
  pssquares[14] = 22000801;
  pssquares[15] = 48473881;
  pssquares[16] = 48473881;
  pssquares[17] = 175244281;
  pssquares[18] = 427733329;
  pssquares[19] = 427733329;

  pssquares[20] = 898716289;
  pssquares[21] = 2805544681;
  pssquares[22] = 2805544681;
  pssquares[23] = 2805544681;
  pssquares[24] = 10310263441;
  pssquares[25] = 23616331489;
  pssquares[26] = 85157610409;
  pssquares[27] = 85157610409;
  pssquares[28] = 196265095009;
  pssquares[29] = 196265095009;
  pssquares[30] = 2871842842801;
  pssquares[31] = 2871842842801;
  pssquares[32] = 2871842842801;
  pssquares[33] = 26250887023729;
  pssquares[34] = 26250887023729;
  pssquares[35] = 112434732901969;
  pssquares[36] = 112434732901969;
  pssquares[37] = 112434732901969;
  pssquares[38] = 178936222537081;
  pssquares[39] = 178936222537081;
  
  pssquares[40] = 696161110209049;
  pssquares[41] = 696161110209049;
  pssquares[42] = 2854909648103881;
  pssquares[43] = 6450045516630769;
  pssquares[44] = 6450045516630769;
  pssquares[45] = 11641399247947921;
  pssquares[46] = 11641399247947921;
  pssquares[47] = 190621428905186449;
  pssquares[48] = 196640248121928601;
  pssquares[49] = 712624335095093521;
  pssquares[50] = 1773855791877850321;
  pssquares[51] = 2327687064124474441;
  pssquares[52] = 6384991873059836689;
  pssquares[53] = 8019204661305419761;
  pssquares[54] = 10198100582046287689;
  pssquares[55] = 10198100582046287689;
  pssquares[56] = 10198100582046287689;
  pssquares[57] = 69848288320900186969;
  pssquares[58] = 208936365799044975961;
  pssquares[59] = 533552663339828203681;

  pssquares[60] = 936664079266714697089;
  pssquares[61] = 936664079266714697089;
  pssquares[62] = 2142202860370269916129;
  pssquares[63] = 2142202860370269916129;
  pssquares[64] = 2142202860370269916129;
  pssquares[65] = 13649154491558298803281;
  pssquares[66] = 34594858801670127778801;
  pssquares[67] = 99492945930479213334049;
  pssquares[68] = 99492945930479213334049;
  pssquares[69] = 295363487400900310880401;
  pssquares[70] = 295363487400900310880401;
  pssquares[71] = 3655334429477057460046489;
  pssquares[72] = 4235025223080597503519329;
} 

