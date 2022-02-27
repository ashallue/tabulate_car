// Code written by Jonathan Sorenson

#ifndef _BIGINT
#define _BIGINT

#include <iostream>
#include <cstring>

using namespace std;

typedef int bigint __attribute__ ((__mode__ (__TI__)));
typedef bigint int128;

void tostring(bigint x,char s[]);

void print(bigint x, ostream & os = cout);

bigint atobi(char tmp[]);

void read(bigint &x, istream &is = cin);

// Jonathan had these next three functions inline, but that wouldn't compile 
// on Hyperion for some reason, so I removed the inline keyword

ostream & operator<<(ostream & os, bigint x);

istream & operator>>(istream & is, bigint &x);

bigint min(bigint x, bigint y);

#endif
