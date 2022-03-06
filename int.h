// Code written by Jonathan Sorenson

#ifndef INT_H
#define INT_H

// Integer utility routines and type definitions

#include<cstdint>
#include<cmath>
#include<cstdint>

#include "bigint.h"

using namespace std;

// types

typedef int16_t int16;
typedef int32_t int32;
typedef uint64_t uint64;
typedef int64_t int64;
typedef char bit;
// we have int128 from bigint.h


const int primeslen = 168;
const int16 primesmax = 1000;
const int16 primes[] = {
   2,   3,   5,   7,  11,  13,  17,  19,  23,  29,
  31,  37,  41,  43,  47,  53,  59,  61,  67,  71,
  73,  79,  83,  89,  97, 101, 103, 107, 109, 113,
 127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
 179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
 283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
 353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
 419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
 467, 479, 487, 491, 499, 503, 509, 521, 523, 541,
 547, 557, 563, 569, 571, 577, 587, 593, 599, 601,
 607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
 661, 673, 677, 683, 691, 701, 709, 719, 727, 733,
 739, 743, 751, 757, 761, 769, 773, 787, 797, 809,
 811, 821, 823, 827, 829, 839, 853, 857, 859, 863,
 877, 881, 883, 887, 907, 911, 919, 929, 937, 941,
 947, 953, 967, 971, 977, 983, 991, 997
};

// testing for squares
bool issquare(int64 x);

// GCD and inverse functions
int64 gcd(int64 x, int64 y);

inline int64 lcm(int64 a, int64 b);

int64 extgcd(int64 a, int64 b, int64 &x, int64 &y);

// returns the inverse of x modulo m
int64 inv(int64 x, int64 m);

// modular exponentiation: 32 and 64-bit versions

int32 powmod(int32 xin, int32 e, int32 m);

int64 powmod(int64 xin, int64 e, int64 m);

// written by Andrew Shallue, same strategy as powmod above.
// Be careful: no check that xin^e actually fits in an int64
int64 pow(int64 xin, long e);

inline int16 legendre(int32 a, int32 p);

inline int16 legendre(int64 a, int64 p);

#endif
