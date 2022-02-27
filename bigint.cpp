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
