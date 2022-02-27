/* Stack class with implementation.
   Code written by Jon Sorenson
   Comments added by Andrew Shallue

   This class intended to be used to store the squarefree part of a factorizations.
   The product of the first 20 primes is greater than 10^26 and 2^88, 
   so this class works certainly works for integers up to 64 bits.

   Notice the lack of bounds checking.  This is because performance is at a 
   premium for the intended application.

   Questions: 1) What does inline do?  First look at Documentation didn't help means
   2) items[top++] = x appears to do assignment first, then increment.  Is that right?

   Answer to 2) https://stackoverflow.com/questions/55085938/post-increment-vs-assignment-in-c-operation-precedence-table
   b++ increments b, but evaluates to the previous value.
   compare to ++b, which increments first and then evaluates to the new value
*/

#include <iostream>

using namespace std;

#ifndef STACK_H
#define STACK_H

class Stack
{
public:
  // size limited to 20 longs
  long items[20];
  // top is the index above of the top element
  long top;

public:

  // constructor initializes the top value to 0
  // since no -1 index, this means the stack is empty
  Stack() { top = 0; }

  // store x in current top position, then increment top
  inline void push(int x) { items[top++] = x; }
  // decrement top, then return val at position top, i.e. top of the stack
  inline int pop()        { return items[--top]; }

  // Stack is empty if top is 0
  inline bool isempty()   { return(top == 0); }
  // peak.  Note the top element is just below top index
  inline int gettop()     { return items[top - 1]; }

  // print function added by Andrew
  inline void print(){ 
    for(long i = 0; i < top; ++i){ 
      cout << items[i] << " ";
    } 
    cout << "\n";
  }
}; // end stack class

#endif
