/**classMain.cpp*/

#include <iostream>
#include "class.h"

using namespace std;

int main()
{
	//declares a pointer to objects of the class myClass
	myClass *mC;

	//allocates memory to store each object. Pointers sX are pointing to the memory area where the object is stored
	mC = new myClass(30, 'm');
	
	//invokes the printData method 
	mC->printData();

	//liberates the memory space where objects were stored
	delete mC;
	
	//returns main result
	return 0;
}
