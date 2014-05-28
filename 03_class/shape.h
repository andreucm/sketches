/**shape.h*/

#ifndef shape_h
#define shape_h

#include <cstdlib> 
#include <iostream>

#define NOSURFACE -1

using namespace std;

class shape
{
	protected:
		char name[20];
	
	public:
		shape(); //constructor without arguments
		shape(char *strName); //constructor with arguments
		~shape(); //destructor
		void setName(char *strName); //sets the name of the shape
		void printName(); //prints the name of the shape to th stdoutput
		virtual float surface();//returns the surface of the shape
};
#endif
