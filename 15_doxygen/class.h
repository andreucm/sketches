/**class.h*/

#ifndef class_h
#define class_h

#include <iostream>

using namespace std;

/**
 This class is created to learn about doxygen documenation. 
*/
class myClass
{
	protected:
		int ii;/**<ii is the a data member of the class holding a character */
		char cc;/**<cc is the a data member of the class holding a character */
	
	public:
		myClass(); /**<The default constructor. Sets ii to 0 and cc to c*/
		myClass(int iValue, char cValue); /**<Cosntructor with arguments. Sets ii and cc with the parameters*/
		~myClass(); /**<The default destructor. Nothing to do*/
		void printData(); /**<Prints the data to the std output*/
};
#endif
