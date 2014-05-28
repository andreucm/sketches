/**helloPeople.cpp*/

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	int ii;
	string name;

	//checks the argument counter
	if (argc<2)
	{
		cout << "I need somebody to say hello" << endl;
	}
	else
	{
		cout << "Hello ";
		for (ii=1; ii<argc; ii++)
		{
			cout << argv[ii] << ", "; //say hello to each one
		}
		cout << endl;

		cout << "(with strings) Hello ";
		for (ii=1; ii<argc; ii++)
		{
			name=argv[ii];
			cout << name << ", "; //say hello to each one
		}
		cout << endl;

		cout << "(concatenating strings) Hello ";
		name.erase();
		for (ii=1; ii<argc; ii++)
		{
			name+=argv[ii];
			name+=", ";
		}
		cout << name; //say hello to each one
		cout << endl;
	}
	return 0;
}
