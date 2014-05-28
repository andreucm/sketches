/**shape.h*/

#ifndef shape_h
#define shape_h

#include <cstdlib> 
#include <iostream>
#include <string>

using namespace std;

class shape
{
	protected:
		string name;
	
	public:
		shape();
		shape(const string & strName);
		~shape();
		void setName(const string & strName);
		void printName();
		virtual float computeArea()=0;
		virtual void draw()=0;
};

shape::shape()
{
	cout << "shape constructor without arguments" << endl;
}

shape::shape(const string & strName)
{
	//sprintf(name, "%s", strName);
	name = strName;
	cout << "shape constructor with arguments: " << name << endl;
}

shape::~shape()
{
	cout << "shape destructor: " << name << endl;
}

void shape::setName(const string & strName)
{
	name = strName;
}

void shape::printName()
{
	//cout << name << endl;
	cout << name;
}

#endif
