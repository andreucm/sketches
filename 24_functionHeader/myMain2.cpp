#include <iostream>
#include "myFunctions.h"

int main()
{
	//declare variables
	double param1, param2;
	double result1, result2, result;
	
	//initialize parameters
	param1 = 2;
	param2 = 3;
	
	//compute
	result1 = mySum(param1,param2);
	result2 = myProduct(param1,param2);
	result = myProduct(result1,result2);
	
	//display
	std::cout << "Result is:" << std::endl;
	std::cout << "   result = " << result << std::endl;
	
	return 0;
}
