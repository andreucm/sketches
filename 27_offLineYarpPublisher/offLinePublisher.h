#ifndef offLinePublisher_H
#define offLinePublisher_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Time.h>
#include <yarp/os/Network.h>

using namespace yarp::os;
using namespace std;

static const unsigned int MAX_CHARS_PER_LINE = 1000;
static const unsigned int TIME_STAMP_DIGITS = 15;
static const unsigned int ACTIVE_FILE = 1;
static const unsigned int END_OF_FILE = 2;
static const int ALL_FILES_ENDED = -1;
static const int RUNNING = 1;

class CoffLinePublisher
{
	public:
		CoffLinePublisher(const double firstTS, const string configFileName, bool vb = true);
		~CoffLinePublisher();
		void initializeNow();
		int publishNextData();
		
	protected:
		bool verbose; 
		double userFirstTimeStamp;//first time stamp set by user
		double logFileFirstTimeStamp;//actual first time stamp of the first data row that will be published
		double firstNow;//starting moment of the off-line reading process loop
		unsigned int iiNext; //indicates which is the next file index to be read 
		vector<ifstream*> fileSet; //set of files with log data
		vector<double> timeStamps; //corresponding next time stamp for each file stream
		vector<unsigned int> fileStatus; //indicates if a given file has reached EOF
		vector<BufferedPort <Bottle>* > portSet;
		char buf[MAX_CHARS_PER_LINE]; //buffer for read file operations
		
		void openFiles(const string configFileName);
		void initializeTimeStamps();
		void fillBottle(const unsigned int bottleLayout, const char *buffer, Bottle & bot);
		void clearBuffer(const unsigned int nn);
		double timeStamp();
		void printTimeStamp(const double tS, ostream *ost);
};

#endif