
#include "offLinePublisher.h"

CoffLinePublisher::CoffLinePublisher(const double firstTS, const string configFileName, bool vb)
{
	userFirstTimeStamp = firstTS;
	openFiles(configFileName);
	initializeTimeStamps();
	verbose = vb;
}

CoffLinePublisher::~CoffLinePublisher()
{
	unsigned int ii;

	for (ii=0; ii<fileSet.size(); ii++)	fileSet.at(ii)->close();
	fileSet.clear();

	for (ii=0; ii<portSet.size(); ii++) portSet.at(ii)->close();
	portSet.clear();

	if (verbose) cout << "End of CoffLinePublisher DESTRUCTOR " << endl;
}

void CoffLinePublisher::initializeNow()
{
	firstNow = timeStamp();
	cout << "   First Log File Time Stamp: "; printTimeStamp(logFileFirstTimeStamp, &cout); cout << endl;
	cout << "   Starting Off-line Loop Time: "; printTimeStamp(firstNow, &cout); cout << endl;
}

int CoffLinePublisher::publishNextData()
{
	unsigned int numC, ii;
	double tsi, now, dT;
	
	//reads data from the file indicated by the index iiNext
	fileSet.at(iiNext)->get(); //just extract a space char after the time stamp already read
	fileSet.at(iiNext)->getline(buf, MAX_CHARS_PER_LINE);//reads the data row up to the end
	numC = fileSet.at(iiNext)->gcount();
	if (verbose)
	{
		cout << endl << "Read Data: ";
		printTimeStamp(timeStamps.at(iiNext), &cout);
		cout << ": " << buf << endl;
	}
	
	//publishData thorugh YARP ports
	Bottle & bot = portSet.at(iiNext)->prepare(); 
	bot.clear();
	bot.addDouble(timeStamps.at(iiNext)); //add time stamp at the first field of the bottle
	fillBottle(iiNext, buf, bot);
	if (verbose)
	{
		cout << "Published Data: ";
		cout << bot.toString() << endl;
	}
	portSet.at(iiNext)->write(); 
	
	//clear buffer 
	clearBuffer(numC);

	//reads the next ts for the current active file
	(*fileSet.at(iiNext)) >> tsi;
	timeStamps.at(iiNext) = tsi; 
	
	//checks end of file
	if ( fileSet.at(iiNext)->eof() ) 
	{
		cout << "End of File for file id: " << iiNext << endl;
		fileStatus.at(iiNext) = END_OF_FILE;
		timeStamps.at(iiNext) = timeStamp();
	}
	
	//checks if all files are EOF
	bool all_eof = true;
	for (ii=1; ii<fileStatus.size(); ii++)
	{
		if ( fileStatus.at(ii) == ACTIVE_FILE ) 
		{
			all_eof = false;
			break;
		}
	}
	if (all_eof) return ALL_FILES_ENDED;
	
	//decides the next file
	tsi = timeStamps.at(0);
	iiNext=0;
	for (ii=1; ii<fileSet.size(); ii++)
	{
		if ( ( fileStatus.at(ii) == ACTIVE_FILE ) && ( timeStamps.at(ii) < tsi ) )
		{
			tsi = timeStamps.at(ii);
			iiNext=ii;
		}
	}
	
	//sleeps up to the next time stamp
	now = timeStamp();
	dT = (timeStamps.at(iiNext)-logFileFirstTimeStamp) - (now-firstNow);
	if ( dT<0 ) dT = 0;
	dT = dT*(double)1e6;
	usleep((int)dT);
	
	return RUNNING;
}

void CoffLinePublisher::openFiles(const string configFileName)
{
	ifstream configFile;
	string dataFileName;
	ifstream *dataFile;
	BufferedPort<Bottle> *bPort;
	
	cout << "Opening files indicated by config file: " << configFileName << endl;
	configFile.open(configFileName.c_str(), ifstream::in); //opens config file
	while (!configFile.eof())
	{
		configFile.getline(buf,MAX_CHARS_PER_LINE, ' '); //reads up to reach MAX_CHARS_PER_LINE,' ' or eof
		//cout << "Read chars: " << configFile.gcount() << "; Opening file: " << buf.str().c_str() << endl;
		if ( configFile.gcount() > 3 )  //"heuristic" condition to avoid last spacing line to be confused as a file
		{
			//cout << "Read chars: " << configFile.gcount() << endl;
			cout << "; Opening file: " << buf << endl;
			dataFile = new ifstream();
			dataFile->open(buf);
			fileSet.push_back(dataFile);//adds data file to the set
			timeStamps.push_back(0);//initializes time stamp vector to 0
			fileStatus.push_back(ACTIVE_FILE); //indicates that the file is active
			clearBuffer(configFile.gcount());
			configFile.getline(buf,MAX_CHARS_PER_LINE, '\n'); //reads port name
			cout << "port name: " << buf << endl;
			bPort = new BufferedPort<Bottle>;
			bPort->open(buf);
			portSet.push_back(bPort);
			clearBuffer(configFile.gcount());
		}
	}	
}

void CoffLinePublisher::initializeTimeStamps()
{
	unsigned int ii;
	double tsi;
	
	//advances file seek position up to the first row data with timestamp>= userFirstTimeStamp
	for (ii=0; ii<fileSet.size(); ii++)
	{
		(*fileSet.at(ii)) >> tsi;
		timeStamps.at(ii) = tsi;
		while ( tsi < userFirstTimeStamp ) 
		{
			fileSet.at(ii)->getline(buf, MAX_CHARS_PER_LINE);
			clearBuffer(fileSet.at(ii)->gcount());//discard line
			(*fileSet.at(ii)) >> tsi;
			timeStamps.at(ii) = tsi; //updates data time stamp for this file
		}
		//cout << timeStamps.at(ii) << endl;
	}	
	
	//initializes logFileFirstTimeStamp and iiNext
	logFileFirstTimeStamp = timeStamps.at(0);
	iiNext=0;
	for (ii=1; ii<timeStamps.size(); ii++)
	{
		if ( timeStamps.at(ii) <= logFileFirstTimeStamp) 
		{
			logFileFirstTimeStamp = timeStamps.at(ii);
			iiNext = ii;
		}
	}
	//cout << iiNext << "; " << logFileFirstTimeStamp << endl;
}

//this method should be replaced by some more elaborated method allowing to parse log files from a given data pattern described as a txt file or xml
void CoffLinePublisher::fillBottle(unsigned int bottleLayout, const char *buffer, Bottle & bot)
{
	stringstream stream_buf; 
	string aux_string;
	int aux_int;
	double aux_double;
	
	stream_buf << buffer;
	if (verbose)cout << "streamBuf (Line " << __LINE__ << "): " << stream_buf.str() << endl;
	switch(bottleLayout)
	{
		case 0:
			stream_buf >> aux_int;
			bot.addInt(aux_int);
			stream_buf >> aux_int;
			bot.addInt(aux_int);
			stream_buf >> aux_double;
			bot.addInt(aux_double);
			break;
		case 1:
			stream_buf >> aux_int;
			bot.addInt(aux_int);
			getline (stream_buf,aux_string);
			bot.addString(aux_string.c_str());
			break;
		default:
			cout << "Warning: Unknown bottle layout!" << endl;
			break;
	}
}

void CoffLinePublisher::clearBuffer(const unsigned int nn)
{
	unsigned int ii;
	for (ii=0; ii<nn; ii++) buf[ii] = '\0';
}

double CoffLinePublisher::timeStamp()
{
	timeval timeStamp;
	gettimeofday(&timeStamp, NULL); 
	return (double)(timeStamp.tv_sec + timeStamp.tv_usec/1e6);	
}

void CoffLinePublisher::printTimeStamp(const double tS, ostream *ost)
{
	streamsize nn, ww;
	ios_base::fmtflags fmtfl;
	char cc; 
	
	fmtfl = ost->flags(ios::left);
	cc = ost->fill('0');
	ww = ost->width(TIME_STAMP_DIGITS);
	nn = ost->precision(TIME_STAMP_DIGITS);
	
	(*ost) << tS;
	
	ost->flags(fmtfl);
	ost->fill(cc);
	ost->width(ww);
	ost->precision(nn);
}
