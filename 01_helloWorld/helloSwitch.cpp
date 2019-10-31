/**hello_switch.cpp*/

#include <iostream>

using namespace std;

int main()
{
	int a=1;

	switch (a)
	{
		case 1:
		case 2:
		case 3:
			cout << "cases 1,2,3" << endl;

		case 1:
		case 2:
			cout << "cases 1,2" << endl;

		case 1:
			cout << "case 1" << endl;
			break;

		default:
			cout << "default" << endl;
			break;
	}

	return 0;
}
