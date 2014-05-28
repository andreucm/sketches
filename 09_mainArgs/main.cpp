
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{

	float param1, param2;
	unsigned char param3;
	bool isV = false;
	
	if (argc!=5)
	{
		cout << "Bad parameters! EXIT" << std::endl;
		return -1;
	}

	cout << "argv[0]= " << argv[0] << endl;
	param1=atof(argv[1]);
	param2=atof(argv[2]);
	if ( *argv[3] == 'v' ) isV=true;
	cout << "param1= " << param1 << endl;
	cout << "param2= " << param2 << endl;
	cout << "isV= " << isV << endl;
	cout << "fileName= " << argv[4] << endl;

	//myClass * myObj;
	//myObj = new myClass(param1, param2);

	return 0;
}
