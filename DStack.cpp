/*-- DStack.cpp----------------------------------------------------------
 
   This file implements Stack member functions.

-------------------------------------------------------------------------*/
 
#include <iostream>
#include <cassert>
#include <new>
using namespace std;

#include "DStack.h"

//--- Definition of Stack constructor
Stack::Stack(int numElements)
{
   assert (numElements > 0);  // check precondition
   myCapacity = 50;  // set stack capacity
                              // allocate array of this capacity
   leftFlag = true, rightFlag = true;
   leftFlagLoc = -100;
   rightFlagLoc = -100;
   myArray = new(nothrow) StackElement[myCapacity];
   if (myArray != 0)          // memory available
      myTop = -1;
   else
   {
      cerr << "Inadequate memory to allocate stack \n"
              " -- terminating execution\n";
      exit(1);
   }                          // or assert(myArray != 0);
}

//--- Definition of Stack copy constructor
Stack::Stack(const Stack & original)
: myCapacity(original.myCapacity), myTop(original.myTop)
{
   //--- Get new array for copy
   myArray = new(nothrow) StackElement[myCapacity];  
   if (myArray != 0)                 // check if memory available
      // copy original's array member into this new array
      for (int pos = 0; pos <= myTop; pos++) 
         myArray[pos] = original.myArray[pos];
   else
   {
      cerr << "*Inadequate memory to allocate stack ***\n";
      exit(1);
   }
}

//--- Definition of Stack destructor
Stack::~Stack()
{ 
   delete [] myArray;
}

//--- Definition of assignment operator
const Stack & Stack::operator=(const Stack & rightHandSide)
{
   if (this != &rightHandSide)                // check that not st = st
   {
      //-- Allocate a new array if necessary
      if (myCapacity != rightHandSide.myCapacity) 
      {
         delete[] myArray;                    // destroy previous array

         myCapacity = rightHandSide.myCapacity;  // copy myCapacity 
         myArray = new StackElement[myCapacity]; 
         if (myArray == 0)                    // check if memory available
         {
            cerr << "*** Inadequate memory ***\n";
            exit(1);
         }
        }

      myTop = rightHandSide.myTop;            // copy myTop member
      for (int pos = 0; pos <= myTop; pos++)  // copy stack elements
         myArray[pos] = rightHandSide.myArray[pos];

   }
   return *this;
}

//--- Definition of empty()
bool Stack::empty() const
{ 
   return (myTop == -1); 
}

//--- Definition of push()
void Stack::push(const StackElement & value)
{
   if (myTop < myCapacity - 1) //Preserve stack invariant
   { 
      myTop++;
      myArray[myTop] = value;
   }
   else
   {
      cerr << "*** Stack full -- can't add new value ***\n"
              "Must increase the stack's capacity.\n";
      exit(1);
   }
}

//--- Definition of display()
void Stack::display(ostream & out) const
{
   for (int i = myTop; i >= 0; i--) 
      out << myArray[i] << endl;
}

//--- Definition of top()
StackElement Stack::top() const
{
	if (!empty()) {
		if (myTop != -1)
			return (myArray[myTop]);
	}
   else
   {
      //cerr << "*** Stack is empty -- returning garbage value ***\n";
      //StackElement garbage;
      return -1;
   }
}

//--- Definition of pop()
void Stack::pop()
{
   if ( !empty() )
      myTop--;
   else
      cerr << "*** Stack is empty -- can't remove a value ***\n";
}

int Stack::length() {

	return myTop + 1;

}

StackElement Stack::bottom() {
	
	if (this->length() == 0) return -1;
	else return myArray[0];
}