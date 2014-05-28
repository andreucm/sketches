/**helloWorld.cpp*/

#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h> //rand's

using namespace std;

int main()
{
	int a1,a2, mm;
	double x1,x2;
	unsigned int idx;
      vector<bool> myB(8,true);
	bool myB2[8];
      unsigned int st, mask, ii;
	
	//module
	a1 = 12651721;
	mm = 2;
	a2 = a1%mm;
	cout << a1 << " module " << mm << " = " << a2 << endl;
	
	//int, ceil, floor and round
	cout << "3/2 = " << 3/2 << endl;
	cout << "3./2. = " << 3./2. << endl;
	cout << "(int)(3/2) = " << (int)(3/2) << endl;
	cout << "(int)(3./2.) = " << (int)(3./2.) << endl;
      cout << "(int)(3.6/2.) = " << (int)(3.6/2.) << endl;
	cout << "ceil(3/2) = " << ceil(3/2) << endl;
	cout << "ceil(3./2.) = " << ceil(3./2.) << endl;
	cout << "floor(3/2) = " << floor(3/2) << endl;
	cout << "floor(3./2.) = " << floor(3./2.) << endl;
	cout << "(int)(1.9) = " << (int)(1.9) << endl;
	cout << "round(1.9) = " << round(1.9) << endl;
	cout << "round(1.49) = " << round(1.49) << endl;
      cout << "round(-0.5) = " << round(-0.5) << endl;
	cout << "min(12,-3) = " << min(12,-3) << endl;
	cout << "log(M_E) = " << log(M_E) << endl;
	cout << "log10(10) = " << log10(10) << endl;
	cout << "sqrt(2) = " << M_SQRT2 << endl;
	cout << "fabs(-1.345) = " << fabs(-1.345) << endl;
	cout << "exp(1) = " << exp(1) << endl;
	cout << "pow(3,2) = " << pow(3,2) << endl;	
      cout << "copysign ( 10.0,-1.0) = " << copysign( 10.0,-1.0) << endl;
	
	//
	idx = 1;
	cout << "idx = " << idx << endl;
	cout << "!idx = " << !idx << endl;
	cout << "(int)(rand()*255) = " << (int)(rand()*255.0/RAND_MAX) << endl;
	cout << "sizeof(double): " <<  sizeof(x1) << endl;
      
      //bitwise operations
      unsigned short aa, bb;
      
      //setting single bits
      aa = 0x01 | 0x02;
      bb = 0x01 & 0x03;
      std::cout.setf(std::ios::hex, std::ios::basefield);
      std::cout.setf(std::ios::showbase);
      std::cout << "aa: " << aa << std::endl;
      std::cout << "bb: " << bb << std::endl;
      if (aa & 0x01) std::cout << "aa has 0x01" << std::endl;
      if (aa & 0x04) std::cout << "aa has 0x04" << std::endl;
	
      //resetting a single bit
      aa &= (0xffff & ~(0x02) );
      std::cout << "aa: " << aa << std::endl;
      
      //atan 
      double ang = atan2( 0 , -1 );
      std::cout << "ang: " << ang << std::endl;
      
	return 0;
}

