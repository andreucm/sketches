
#include <iostream>
#include "stack.h"

using namespace std;


int main(int argc, char** argv)
{
	Stack<int> intStack;
	Stack<string> strStack;
	intStack.push(25);
	strStack.push("Hello");	
	intStack.print(0);
	strStack.print(0);

	return 0;
}