#include <iostream>

using namespace std;

int main()
{
	unsigned int ii,jj;
	unsigned int loopLimit = 100;
	
	for(ii=0, jj=0; ii<loopLimit; ii++,jj++)
	{
		cout << "ii: " << ii << "; jj: " << jj << endl;
	}
	
	return 0;
}
