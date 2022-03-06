// Code written by Jonathan Sorenson

#include "int.h"

// testing for squares
bool issquare(int64 x)
{
  int64 root = (int64) llroundl(sqrtl((long double)x));
  return (root*root == x);
}

// GCD and inverse functions
int64 gcd(int64 x, int64 y)
{
  if(x<0) x= -x; if(y<0) y= -y;
  int64 r;
  while(y!=0) { r=x%y; x=y; y=r; }
  return x;
}

inline int64 lcm(int64 a, int64 b)
  { return (a/gcd(a,b))*b; }

int64 extgcd(int64 a, int64 b, int64 &x, int64 &y)
{
  int64 ux=1,uy=0,vx=0,vy=1,u=a,v=b,r,rx,ry,q;
  int64 asign=1, bsign=1;
  if(a<0) { a=-a; asign=-1; }
  if(b<0) { b=-b; bsign=-1; }
  while(v>0)
  {
    q=u/v; r=u-q*v;
    rx=ux-q*vx; ux=vx; vx=rx;  ry=uy-q*vy; uy=vy; vy=ry; 
    u=v; v=r;
  }
  x=asign*ux; y=bsign*uy;
  return u;
}

// returns the inverse of x modulo m
int64 inv(int64 x, int64 m)
{
  int64 a,b,g;
  g=extgcd(x%m,m,a,b);
  if(g!=1) return 0;
  if(a>0 && a<m) return a;
  if(a>0) return a%m;
  if(a>-m) return m+a;
  return m-((-a)%m);
}

int32 powmod(int32 xin, int32 e, int32 m)
{
  int64 y=1;
  int64 x=xin;
  while(e>0)
  {
    if(e%2==1) y=(y*x)%m; // if e is odd
    e >>= 1;  // e = e/2
    x = (x*x)%m;
  }
  return (int32)y;
}

int64 powmod(int64 xin, int64 e, int64 m)
{
  int128 y=1;
  int128 x=xin;
  while(e>0)
  {
    if(e%2==1) y=(y*x)%m; // if e is odd
    e >>= 1;  // e = e/2
    x = (x*x)%m;
  }
  return (int64)y;
}

// written by Andrew Shallue, same strategy as powmod above.
// Be carefule: no check that xin^e actually fits in an int64
int64 pow(int64 xin, long e){
  int64 y = 1;
  int64 x = xin;
 
  while(e > 0){
    // if e is odd, multiply by x, which is appropriate power of 2
    if(e % 2 == 1) y = y * x;
    // if e even or odd, divide it by 2 and square
    e >>= 1;
    x = x * x;
  }
  return y;
}

inline int16 legendre(int32 a, int32 p)
  { return powmod(a, (p-1)/2, p); }
  
inline int16 legendre(int64 a, int64 p)
  { return powmod(a, (p-1)/2, p); }
