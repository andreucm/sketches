
#include "class.h"

myClass::myClass()
{
	ii=0; 
	cc='c';
}

myClass::myClass(int iValue, char cValue)
{
	ii=iValue;
	cc=cValue;
}

myClass::~myClass()
{
	//
}

void myClass::printData()
{
	cout << "ii: " << ii << endl << "cc: " << cc << endl;
}
