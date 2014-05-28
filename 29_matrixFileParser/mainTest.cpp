#include "counterMatrix.h"
#include <sstream>
#include <cstdlib>

int main(int argc, char *argv[])
{
	ostringstream fileName;
	CcounterMatrix myMatrix;

	if (argc!=3)
	{
		cout << "Calling isntructions: ./counterMatrix [file name] [step size]" << endl;
		cout << "EXIT PROGRAM !" << endl;
		return -1;
	}
	
	cout << endl << "************ Main test of counterMatrix *******************" << endl << endl;
	fileName << argv[1];
	myMatrix.setMatrixStep(atof(argv[2]));
	myMatrix.getFileData(fileName.str());
	myMatrix.fillMatrix();
	myMatrix.printMatrix(IMAGE);
	
	return 1;
}
