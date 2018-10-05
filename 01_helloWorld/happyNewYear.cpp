/**HappyNewYear.cpp*/

//just a comment to test git features
//just a second comment to test git features

#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
	string sentence("Beta Robots wishes you a Happy New Year 2018 ;-)");
	cout << endl << "************************************************" << endl << endl;
	for (unsigned int ii=0; ii<sentence.size(); ii++)
	{
		usleep(100000);
		cout << sentence.at(ii);
		cout.flush();
	}
	usleep(500000);
	cout << endl << endl << "************************************************" << endl << endl;
	return 0;
}
