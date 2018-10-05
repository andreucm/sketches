/**struct.cpp*/

#include <iostream>
#include <string>

typedef struct
{
	int a_;
	double b_;
} StructType;


int main(int argc, char *argv[])
{
	StructType st;
	st.a_ = 2;
	st.b_ = 3.1415;

	return 0;
}
