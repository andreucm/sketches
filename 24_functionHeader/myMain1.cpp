#include <iostream>
#include "myFunctions.h"

int main()
{
	//declare variables
	double param1, param2;
	double result1, result2;
	
	//initialize parameters
	param1 = 2;
	param2 = 3;
	
	//compute
	result1 = mySum(param1,param2);
	result2 = myProduct(param1,param2);
	
	//display
	std::cout << "Results are:" << std::endl;
	std::cout << "   result1 = " << result1 << std::endl;
	std::cout << "   result2 = " << result2 << std::endl;
	
	return 0;
}
