#include "counterMatrix.h"

CcounterMatrix::CcounterMatrix()
{
	matrixStep = DEFAULT_MATRIX_STEP;
}

CcounterMatrix::~CcounterMatrix()
{
	fileData.clear();
	matrix.clear();
}

void CcounterMatrix::setMatrixStep(double ms)
{
	matrixStep = ms;
}

void CcounterMatrix::getFileData(string fileName)
{
	ifstream file;
	cellPos cpFile;
	unsigned int ii;
	
	file.open(fileName.c_str());
	
	while(!file.eof())
	{
		file >> cpFile.xx >> cpFile.yy;//get doubles
		if (file.good())
		{
			//stores & displays data
			fileData.push_back(cpFile);
			//cout << cpFile.xx << " " << cpFile.yy << endl; //print doubles to stdout			
		}
	}
	
	//sets data xy range (min/max)
	xMax = fileData.at(0).xx;
	xMin = fileData.at(0).xx;
	yMax = fileData.at(0).yy;
	yMin = fileData.at(0).yy;

	for (ii=1; ii<fileData.size(); ii++)
	{
		if (fileData.at(ii).xx>xMax) xMax = fileData.at(ii).xx;
		if (fileData.at(ii).xx<xMin) xMin = fileData.at(ii).xx;
		if (fileData.at(ii).yy>yMax) yMax = fileData.at(ii).yy;
		if (fileData.at(ii).yy<yMin) yMin = fileData.at(ii).yy;			
	}

	cout << "getFileData() *****************************" << endl
	     << "File Name: " << fileName << endl
	     << "Num cell positions: " << fileData.size() << endl
	     << "Data ranges:" << endl
	     << "\tX in [" << xMin << "," << xMax << "]" << endl
	     << "\tY in [" << yMin << "," << yMax << "]" << endl;
	file.close();
}

void CcounterMatrix::fillMatrix()
{
	unsigned int ii, jj, kk, nCols, nRows;
	
	//compute dimensions of the matrix
	nRows = ceil( (xMax - xMin)/matrixStep ) + 1;
	nCols = ceil( (yMax - yMin)/matrixStep ) + 1;
	cout << "fillMatrix() ******************************" << endl
	     << "\tnRows: " << nRows << endl
	     << "\tnCols: " << nCols << endl;
	     
	
	//allocates matrix memory space
	matrix.resize(nRows);
	for(ii=0; ii<matrix.size(); ii++)
	{
		matrix.at(ii).resize(nCols);
	}
	cout << "\tMemory allocated to store " << matrix.size() << " x " << matrix.at(0).size() << " elements" << endl;
	
	//fills matrix
	for(kk=0; kk<fileData.size(); kk++)
	{
		ii = round((fileData.at(kk).xx-xMin)/matrixStep);
		jj = round((fileData.at(kk).yy-yMin)/matrixStep);
		//cout << "ii: " << ii << "; jj: " << jj << endl;
		matrix.at(ii).at(jj) = 1; //++ if we want an histogram
	} 
}

void CcounterMatrix::printMatrix(unsigned int type)
{
	unsigned int ii, jj;
	
	cout << "printMatrix() *****************************" << endl;
	for(ii=0; ii<matrix.size(); ii++)
	{
		cout << "\t";
		for(jj=0; jj<matrix.at(ii).size(); jj++)
		{
			switch (type)
			{
				case IMAGE:
					if ( matrix.at(ii).at(jj)==0 ) cout << "  ";
					else cout << "X ";
					break;

				case VALUES:
					cout << matrix.at(ii).at(jj) << " ";
					break;
				default:
					cout << "printMatrix(). Unknown display type (" << type << ")" << endl
					     << "No display performed!" << endl;
					break;
			}
		}
		cout << endl;
	}
}
