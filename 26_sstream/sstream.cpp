/**sstreamTest.cpp*/

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main()
{
	string string1;
	ostringstream stream1;
	double aa;
	unsigned int ii;
	unsigned int iMax = 5;
	
	for (ii=0; ii<iMax; ii++)
	{
		stream1.str("");
		stream1 << "interation: " << ii << endl;
		string1 = stream1.str();
		cout << string1;
		//stream1.clear();
	}
	
	stream1.fill('0');
	for (ii=0; ii<iMax; ii++)
	{
		//stream1.seekp(ios_base::beg);//this will overwrite from the begining position. If new content is of the same size it seems a data clear , but it's no t the same !
		stream1.str(""); //that is much better to clean the content
		stream1.clear();
		stream1 << "iteration: ";
		stream1.width(3);	
		stream1 << ii << endl;
		string1 = stream1.str();
		cout << string1;
		//stream1.clear();
	}

	return 0;
}
