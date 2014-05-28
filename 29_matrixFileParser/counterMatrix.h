#ifndef counterMatrix_h
#define counterMatrix_h

#include <vector> 
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

const double DEFAULT_MATRIX_STEP = 0.2;
const unsigned int IMAGE = 0;
const unsigned int VALUES = 1;

struct cellPos
{
	double xx;
	double yy;
};

class CcounterMatrix
{
	protected:
		double xMax, xMin, yMax, yMin;
		double matrixStep;
		vector<cellPos> fileData;
		vector< vector<unsigned int> > matrix;
	
	public:
		CcounterMatrix();
		~CcounterMatrix();
		
		void setMatrixStep(double ms);
		void getFileData(string fileName);
		void fillMatrix();
		void printMatrix(unsigned int type);
};
#endif
