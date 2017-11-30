/**helloWorld.cpp*/

//just a comment to test git features

#include <iostream>
//#include <string>
//#include <cstdlib>

using namespace std;

int main()
{
	double x=2.0/3.0;
	streamsize nn;
	nn=cout.precision(20);
	cout << x << endl;
	cout.precision(nn);
	cout << x << endl;

	cout << "Hello World!" << endl;
	
	//int ret = setenv("ZZZ",":0.0", 0);
	//cout << "ret = " << ret << endl;
	
	//playing with data types
	unsigned short int usi; 
	unsigned int ui; 
	unsigned long long int ulli; 
	float flt;
	double dbl = -15.34e22; 	
	cout << "Size of unsigned short int = " << sizeof(usi) << endl;
	cout << "Size of unsigned int = " << sizeof(ui) << endl;
	cout << "Size of unsigned long long int = " << sizeof(ulli) << endl;
	cout << "Size of float = " << sizeof(flt) << endl;
	cout << "Size of double= " << sizeof(dbl) << endl;
	
	char cDouble[8];
	double dDouble;
	*(&cDouble[0]) = *(&dbl);
	*(&dDouble) = (cDouble[8]&0xff)|(cDouble[9]<<8)|(cDouble[10]<<16)|(cDouble[11]<<24);;
	cout << dbl << endl;
	cout << dDouble << endl;
	
	//playing with logical ops
	int i1, i2, i3, status;
	i1 = 0x1;
	i2 = 0x2;
	i3 = 0x4;
	
	if ( (i1 | i2) )
	{
		cout << "this appears if (i1|i2) != 0" << endl;
	}
	
	status=0;
	cout << hex;
	cout << "i1=" << i1 << "; i2=" << i2 << "; status=" << status << endl;
	status = i1|i2; cout << "status = i1|i2 = " << status << endl;
	status = status & !i2; cout << "status = status & !i2 = " << status << endl;
	cout << "i2=" << i2 << "; !i2=" << !i2 << "; ~!i2=" << ~!i2 << "; x!i2=" << ~i2 << endl;
	status = i1|i2;
	status &= ~i2; cout << "status &= ~i2 = " << status << endl;
	status |= i3; cout << "status |= i3 = " << status << endl;
	status &= ~i1; cout << "status &= ~i1 = " << status << endl;
	if ( (status & i2) )cout << "1. this appears if (status&i2) >0" << endl;	
	status |=i2;
	if ( (status & i2) )cout << "2. this appears if (status&i2) >0" << endl;	
	
	unsigned long ul_; 
	cout << "size of unsigned long: " << sizeof(ul_) << endl; 
	
	
	cout<<dec;
	
	return 0;
}
