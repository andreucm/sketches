/**classMain.cpp*/

#include <iostream>
#include "shape.h"

using namespace std;

int main()
{
	//declares two pointers to objects of the class shape
	shape *s1;
	shape *s2;

	//declares also an object of the class shape
	shape s3;

	//allocates memory to store each object. Pointers sX are pointing to the memory area where the object is stored
	s1 = new shape((char*)"square");
	s2 = new shape();
	
	//sets the name of the unnamed shapes s2 and s3
	s2->setName((char*)"triangle");
	s3.setName((char*)"circle");

	//invokes the printName method for each object
	s1->printName();
	s2->printName();
	s3.printName();

	//liberates the memory space where objects were stored
	delete s1;
	delete s2;

	//s3 will be deleted at the end of the main (as a main function variable)

	return 0;
}
