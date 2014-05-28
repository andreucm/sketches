
#include "shape.h"

shape::shape()
{
	cout << "shape constructor without arguments" << endl;
}

shape::shape(char *strName)
{
	sprintf(name, "%s", strName);
	cout << "shape constructor with arguments: " << name << endl;
}

shape::~shape()
{
	cout << "shape destructor: " << name << endl;
}

void shape::setName(char *strName)
{
	sprintf(name, "%s", strName);
}

void shape::printName()
{
	cout << name << endl;
}

float shape::surface()
{
	return NOSURFACE;
}
