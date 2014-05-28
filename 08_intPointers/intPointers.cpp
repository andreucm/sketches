//Compile command < g++ -o hello hello.cpp >

#include <iostream>

int main()
{
	int *aa;
	int *bb;
	int *cc;
	
	aa = new int();
	bb = new int();
	cc = new int();

	*aa=4;
	*bb=7;
	*cc=((*aa)+(*bb));

	std::cout << "cc = " << (*cc) << std::endl;

	delete (aa);
	delete (bb);
	delete (cc);

	return 1;
} 




