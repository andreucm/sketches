#include "asterx1_gps.h"

CasteRx1::CasteRx1(const string & hwPort)
{
	portName = hwPort;
}

CasteRx1::~CasteRx1()
{
	blockList.clear();
	if(serialFD>0) closeDevice();
	cout << "CasteRx1::~CasteRx1(). End of CasteRx1 destructor" << endl;	//bye bye message
}

int CasteRx1::openDevice()
{
	int retValue;
	
	retValue = openSerialComms(portName);
	if ( retValue == SERIALCOMMERROR )
	{
		status = SERIALCOMMERROR; 
		cout << "CasteRx1::open(). ERROR opening serial communications" << endl;
	}
	else
	{
		serialFD = retValue;
	      //FD_ZERO(&set);
		//FD_SET(serialFD, &set);
		status = BASICSUCCESS;
		cout << "CasteRx1::open(). SUCCESS opening serial communications" << endl;
	}
	
	return status;	
}

int CasteRx1::closeDevice()
{
	close(serialFD);
	serialFD = -1;
	cout << "CasteRx1::close(). Closing serial communications" << endl;
	status = BASICSUCCESS; 
	return status;
}

int CasteRx1::startAcquisition()
{
	int retValue=0; 
	sbfBlockManager *blockMng;
	
	//enables commands as input at USB1
	retValue += sendCommand("setDataInOut,USB1,CMD,none\n");

	//stops data transmission if it was running
	retValue += sendCommand("setSBFOutput,all,USB1,none\n");
	
	//enables septentrio binary format (SBF) as output at USB1
	retValue += sendCommand("setDataInOut,USB1,CMD,+SBF\n");
	
	//GPS signal processing options for urban mobile robots
	retValue += sendCommand("setMultipathMitigation,on\n");//enables processing for multipath removing (pag.26)
	retValue += sendCommand("setReceiverDynamics,Moderate\n");//Mobile robots moves at moderate speeds
	retValue += sendCommand("setPVTMode,Rover,StandAlone\n");//rover mode and enables standalone
	
	//data configuration. Add blocks of interest to "group 1"
	retValue += sendCommand("setSBFGroups,Group1,PVTCartesian\n");
	blockMng = new sbfBlockManager; blockMng->blockId = PVTCartesianBlockId; blockMng->received = false;
	blockList.push_back(*blockMng);
	
	retValue += sendCommand("setSBFGroups,Group1,+PosCovCartesian\n");
	blockMng = new sbfBlockManager; blockMng->blockId = PosCovCartesianBlockId; blockMng->received = false;
	blockList.push_back(*blockMng);

	retValue += sendCommand("setSBFGroups,Group1,+DOP\n");
	blockMng = new sbfBlockManager; blockMng->blockId = DOPBlockId; blockMng->received = false;
	blockList.push_back(*blockMng);
	
	retValue += sendCommand("setSBFGroups,Group1,+PVTGeodetic\n");
	blockMng = new sbfBlockManager; blockMng->blockId = PVTGeodeticBlockId; blockMng->received = false;
	blockList.push_back(*blockMng);
	
	//requests "Group1" data to be output to USB1 at "onChange" rate , 1 sec or 500mse rate
	//retValue += sendCommand("setSBFOutput,Stream1,USB1,Group1,onChange\n");
	//retValue += sendCommand("setSBFOutput,Stream1,USB1,Group1,sec2\n");
	retValue += sendCommand("setSBFOutput,Stream1,USB1,Group1,msec500\n");
	
	if ( retValue == BASICSUCCESS*11 ) //success on all commands
	{
		status = BASICSUCCESS;
		cout << "CasteRx1::start(). SUCCESS on device configuration. The device is now transmitting data ..." << endl;
	}
	else //otherwise
	{
		status = DEVICECONFIGERROR;
		cout << "CasteRx1::start(). ERROR on device configuration for data transmission" << endl;
	}

	return status;
}

int CasteRx1::stopAcquisition()
{
	int retValue=0; 
	
	blockList.clear();
	
	retValue += sendCommand("setSBFOutput,Stream1,USB1,none\n");//stops data transmission from device
	retValue += sendCommand("setDataInOut,USB1,CMD,none\n");//enables commands as input at USB1		
	if ( retValue == BASICSUCCESS*2 )
	{
		status = BASICSUCCESS;
		cout << "CasteRx1::stop(). SUCCESS on stopping the device data transmission" << endl;
	}
	else 
	{
		status = STOPPINGERROR;
		cout << "CasteRx1::stop(). ERROR on stopping the device data transmission" << endl;
	}
	
	return status;
}

void CasteRx1::readDataFromDevice()
{
	unsigned char buffer[200];
	int nRead;
	unsigned short int crcValue; //crc value from device and crc computed
	unsigned short int blockId; //data block id from receiver
	unsigned short int blockLength; //size of the SBF block in bytes
	timeval tmStmp;
	list<sbfBlockManager>::iterator iiBlock;	
	bool allReceived = false;
	int a1=0;
	int a2=0;
	int a3=0;
	int a4=0;
	
	//reset received field of block managers
	for (iiBlock=blockList.begin();iiBlock!=blockList.end();iiBlock++){ iiBlock->received = false; }
	
	//acquisition up to all requested blocks are received
	while (!allReceived)
	{
		//Sets Time Stamp (this is the system time stamp, not the GPS time!)
		gettimeofday(&tmStmp, NULL); 
		timeStamp = (double)(tmStmp.tv_sec + tmStmp.tv_usec/1e6);	
		
		status = synchronizeHeader(serialFD,(const char *)"$@");
		//unsigned char headChars[2];
		//headChars[0]=0xe7;
		//headChars[1]=0x3f;
		//status = synchronizeHeader(serialFD,(const unsigned char *)&headChars[0]);
	//cout << "readDataFromDevice(): SYNCHRONIZED" << endl;
		if ( status != BASICSUCCESS ) //if !BASICSUCCESS, status will be either COMMTIMEOUT or SERIALCOMMERROR
		{
			cout << "CasteRx1::readDataFromDevice(). ERROR. when synchronizing with header. Expected header = $@" << endl;
			return; 
		}		
	//cout << "readDataFromDevice(): "<< __LINE__ << endl;
		//starts reading header
		blockingRead(serialFD, &buffer[0], 2);//reads transmitted crc value
		crcValue = getUI2(&buffer[0]); //decodes crc value
		blockingRead(serialFD, &buffer[0], 2);//reads block id field
		blockId = (buffer[0] & 0xff)|((buffer[1] & 0x1f)<<8);//decodes blockId
		blockingRead(serialFD, &buffer[0], 2); //reads block length
		blockLength = getUI2(&buffer[0]);//gets block length in bytes
		if ( (blockLength%4!=0) || (blockLength < 9) || (blockLength > 200) )
		{
			cout << "CasteRx1::readDataFromDevice(). ERROR. SBF block Length not correct: " << blockLength << " bytes" << endl;
			status = SBFPROTOCOLERROR;
			return;
		}
	cout << "readDataFromDevice(): "<< __LINE__ << "; blockLength = " << blockLength << endl;
		//reads the rest of the block and loads it to the buffer
		nRead = blockingRead(serialFD, buffer, (int)(blockLength-8));
		if ( nRead != (int)(blockLength-8) )
		{
			cout<<"CasteRx1::readDataFromDevice(). ERROR. Not Enough received bytes (read:"<<nRead<<"; expected:"<<(int)(blockLength-8)<<")"<<endl;
			status = SBFPROTOCOLERROR;
			return;
		}

		//Computes & checks CRC
		//TO DO !!!!!!!!!
		/*
		crcComputed=computeCrc(buffer, blockLength-4);
		if (crcComputed!=crcValue){
			cout << "CasteRx1::process(). Error. CRC error" << endl;
			return -7;}
		*/
	//cout << "readDataFromDevice(): "<< __LINE__ << endl;	
		//decodes SBF block
		switch(blockId)
		{
			case PVTCartesianBlockId: a1++;
				status = decodePVTCartesian(buffer);
				markBlockAsReceived(PVTCartesianBlockId);
				break;

			case PVTGeodeticBlockId:a2++;
				status = decodePVTGeodetic(buffer);
				markBlockAsReceived(PVTGeodeticBlockId);
				break;
				
			case PosCovCartesianBlockId:a3++;
				status = decodePosCovCartesian(buffer);
				markBlockAsReceived(PosCovCartesianBlockId);
				break;

			case DOPBlockId:a4++;
				status = decodeDOP(buffer);
				markBlockAsReceived(DOPBlockId);
				break;

			default:
				cout << "CasteRx1::readDataFromDevice(). ERROR. Unknown SBF blockId: " << blockId << endl;
				status = SBFPROTOCOLERROR;	
				break;
		}
	//cout << "readDataFromDevice(): "<< __LINE__ << endl;		
		//check resulting status of the decoding function
		if ( status != BASICSUCCESS )
		{
			cout << "CasteRx1::readDataFromDevice(). ERROR on decoding SBF block. BlockId = " << blockId << ". Status = " << status << endl;
			return;
		}
		else//checks recursively if all blocks have been received
		{		
			allReceived = true;
			for (iiBlock=blockList.begin();iiBlock!=blockList.end();iiBlock++){ allReceived = iiBlock->received & allReceived; }
		}
// 	cout << "readDataFromDevice(): "<< "a1=" << a1 << "; a2=" << a2 << "; a3=" << a3 << "; a4=" << a4 << endl;		
	}

	//conversion to map coordinates
	if ( ( status == BASICSUCCESS ) && ( pvtError == 0 ) ) { fromWgsToMap(); }
	//cout << "readDataFromDevice(): "<< __LINE__ << endl;
	//if we are here, all things have g0one fine!
	return;
}

void CasteRx1::printData(ostream & ost)
{
	streamsize nn;

	ost << status << " ";
	nn=ost.precision(12); ost << timeStamp << " "; ost.precision(nn);
	ost << wnc << " " << tow << " " << numSatellites << " " << PDOP << " ";
	nn=ost.precision(12); ost << lat << " " << lon << " " << alt << " "; ost.precision(nn);
	ost << xWgs << " " << yWgs << " " << zWgs << " ";
	ost << xMap << " " << yMap << " " << zMap << " ";
	ost << vxWgs << " " << vyWgs << " " << vzWgs << " ";
	ost << vxMap << " " << vyMap << " " << vzMap << " ";
	ost << cxxWgs << " " << cyyWgs << " " << czzWgs << " " << cxyWgs << " ";
	ost << cxxMap << " " << cyyMap << " " << czzMap << " " << cxyMap << " ";
	ost << endl;
}

void CasteRx1::setMapOrigin(double mapLat0, double mapLon0, double mapAlt0, double mapAlpha0)
{
	lat0 = mapLat0*M_PI/180.0;
	lon0 = mapLon0*M_PI/180.0;	
	alt0 = mapAlt0;
	alpha0 = mapAlpha0*M_PI/180.0;
	
	computeTM();
}

void CasteRx1::printTM()
{
	streamsize nn;
	
	cout << endl << "MAP ORIGIN (GEODETIC) ----------------------------------" << endl;
	nn=cout.precision(15);
	cout << "lat0 = " << lat0 << endl << "lon0 = " << lon0 << endl << "alt0 = " << alt0 << endl << "alpha0 = " << alpha0 << endl;
	cout.precision(nn);

	cout << endl << "MATRIX TRANSFORM FROM ECEF to MAP-----------------------" << endl;
	cout 	<< TM[0][0] << '\t' << TM[0][1] << '\t' << TM[0][2] << '\t' << TM[0][3] << endl
	<< TM[1][0] << '\t' << TM[1][1] << '\t' << TM[1][2] << '\t' << TM[1][3] << endl 
	<< TM[2][0] << '\t' << TM[2][1] << '\t' << TM[2][2] << '\t' << TM[2][3] << endl 
	<< TM[3][0] << '\t' << '\t' << TM[3][1] << '\t' << '\t' << TM[3][2] << '\t' << '\t' << TM[3][3] << endl;
	cout << "--------------------------------------------------------" << endl << endl;
}

int CasteRx1::getStatus(){ return status; }
double CasteRx1::getTimeStamp(){ return timeStamp; }
unsigned short int CasteRx1::getWnc(){ return wnc; }
unsigned int CasteRx1::getTow(){ return tow; }
unsigned int CasteRx1::getNumSatellites(){ return numSatellites; }
float CasteRx1::getPDOP(){ return PDOP; }
float CasteRx1::getTDOP(){ return TDOP; }
float CasteRx1::getHDOP(){ return HDOP; }
float CasteRx1::getVDOP(){ return VDOP; }
float CasteRx1::getUndulation(){ return undulation; }
unsigned short int CasteRx1::getPVTerror(){ return pvtError;}

double CasteRx1::getLat(bool units)
{ 
	if ( units == inRADS ) return lat; 
	else return lat*180.0/M_PI;
}

double CasteRx1::getLon(bool units)
{
	if ( units == inRADS ) return lon; 
	else return lon*180.0/M_PI;
}

double CasteRx1::getAlt(){ return alt; }
double CasteRx1::getXWgs(){ return xWgs; }
double CasteRx1::getYWgs(){ return yWgs; }
double CasteRx1::getZWgs(){ return zWgs; }
double CasteRx1::getXMap(){ return xMap; }
double CasteRx1::getYMap(){ return yMap; }
double CasteRx1::getZMap(){ return zMap; }
double CasteRx1::getVxWgs(){ return vxWgs; }
double CasteRx1::getVyWgs(){ return vyWgs; }
double CasteRx1::getVzWgs(){ return vzWgs; }
double CasteRx1::getVxMap(){ return vxMap; }
double CasteRx1::getVyMap(){ return vyMap; }
double CasteRx1::getVzMap(){ return vzMap; }
double CasteRx1::getCxxWgs(){ return cxxWgs; }
double CasteRx1::getCyyWgs(){ return cyyWgs; }
double CasteRx1::getCzzWgs(){ return czzWgs; }
double CasteRx1::getCxyWgs(){ return cxyWgs; }
double CasteRx1::getCxxMap(){ return cxxMap; }
double CasteRx1::getCyyMap(){ return cyyMap; }
double CasteRx1::getCzzMap(){ return czzMap; }
double CasteRx1::getCxyMap(){ return cxyMap; }



/******************************* PROTECTED MEMBERS ************************************************/

int CasteRx1::openSerialComms(const string & hwPort)
{
	int retValue;
	
	//opens serial port given by hwPort name. read() will not block
	//retValue = open(hwPort.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
        retValue = open(hwPort.c_str(), O_RDWR | O_NONBLOCK); 
	if ( retValue < 0 )
	{
		cout << "CasteRx1::openSerialComms(). ERROR opening " << hwPort << endl;
		return SERIALCOMMERROR;
	}
	else
	{
		cout << "CasteRx1::openSerialComms(). SUCCESS opening " << hwPort << endl;
		return retValue;
	}
}

int CasteRx1::configSerialComms(int commFd)
{
	int retValue;
	termios ttyset; //termios variable to configure serial port
	
	tcgetattr(commFd, &ttyset);
/*
	ttyset.c_cflag = ( B115200 | CLOCAL | CREAD | CS8 );
	ttyset.c_iflag = ( IGNBRK ); //Ignores break condition on input
	ttyset.c_oflag = 0x0;
	ttyset.c_lflag = 0x0; //input mode non-canonical, no echo
	//ttyset.c_lflag = ICANON; //input mode canonical, no echo
	ttyset.c_cc[VTIME] = 0; //if open in O_NONBLOCK, this would not have effect
	ttyset.c_cc[VMIN] = 0; //if open in O_NONBLOCK, this would not have effect
*/
        ttyset.c_cflag|=(CLOCAL | CREAD);
        ttyset.c_cflag&=~CRTSCTS;
        ttyset.c_lflag&=~(ICANON | ECHO | ECHOE | ISIG);
        ttyset.c_iflag&=~(IXON | IXOFF | IXANY);
        ttyset.c_iflag&=~(INLCR | IGNCR | ICRNL);
        ttyset.c_oflag&=~OPOST;
        ttyset.c_cc[VMIN]=0;
        ttyset.c_cc[VTIME]=100;


	tcflush(commFd, TCIFLUSH);
	retValue = tcsetattr(commFd, TCSANOW, &ttyset); //Sets configuration immediately.
	if ( retValue < 0 )
	{
		cout << "CasteRx1::configSerialComms(). ERROR configuring Serial Port " << endl << endl;
		return SERIALCOMMERROR;
	}
	else
	{
		cout << "CasteRx1::configSerialComms(). SUCCESS configuring Serial Port" << endl << endl;
		return BASICSUCCESS;
	}
}

int CasteRx1::blockingRead(int commFd, unsigned char *buf, int nn, double secondsTimeOut)
{
	bool localError = false;
	int selectResult;
	int bytesRead = 0;
	fd_set set;
      struct timeval timeout;
	int count, ii;

	//loop to read nn bytes in a blocking way with a timeout.
	while( ( bytesRead < nn ) && (!localError) )
	{
		
		if (read(commFd, &buf[bytesRead], 1) == 1)
		{
			bytesRead++;
		}
		else
		{
			FD_ZERO(&set);
			FD_SET(commFd, &set);
			//timeout.tv_sec = floor(secondsTimeOut);
			//timeout.tv_usec = (int)( (secondsTimeOut - floor(secondsTimeOut))*1e6 );
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;
			selectResult = select(commFd+1, &set, NULL, NULL, &timeout);

			if(FD_ISSET(commFd,&set))
			{
				switch (selectResult)//select returns 0 if timeout, 1 if input available, -1 if error.
				{
					case 0: 
						localError = true;  
						bytesRead = COMMTIMEOUT;
						cout << "CasteRx1::blockingRead(): Time Out Comm Error on fd=" << commFd << endl;
						break;
					case 1:
						if ( read(commFd, &buf[bytesRead], 1) == 1)
						{
							//cout << hex << (int)buf[bytesRead] << " " ;
							bytesRead++;
						}
						/*count = read(commFd, &buf[bytesRead], nn-bytesRead);
						if ( count > 0 )
						{
								for (ii=0;ii<count;ii++) cout << hex << (int)buf[bytesRead+ii] << " " ;
								bytesRead = bytesRead + count;
						}*/
						else
						{
							localError = true;  
							bytesRead = SERIALCOMMERROR;
							cout << "CasteRx1::blockingRead(): read() error on fd=" << commFd << endl;
						}
						break;
					default: //-1 and others
						localError = true;  
						bytesRead = SERIALCOMMERROR;
						cout << "CasteRx1::blockingRead(): Serial Comm Error on fd=" << commFd << endl;
						break;
				}
			}
		}
	}
	//cout << "blockingRead(): bytesRead=" << bytesRead << "; localError = " << localError << "; buf = " << buf << endl;
	//cout << dec << endl;
	return bytesRead;
}

/**
  closes serial communication
*/
int CasteRx1::closeSerialComms(int commFd)
{
	close(commFd);
	return BASICSUCCESS;
}

unsigned short int CasteRx1::getUI1(unsigned char *buf)
{
	//return ( (buf[0] & 0xff) | (0x0 << 8) );
	return (buf[0] & 0x00ff);
}

unsigned short int CasteRx1::getUI2(unsigned char *buf)
{
	return ( (buf[0] & 0xff) | ((buf[1]&0xff)<<8) );	
}

unsigned int CasteRx1::getUI4(unsigned char *buf)
{
	return ( (buf[0]&0xff) | ((buf[1]&0xff)<<8) | ((buf[2]&0xff)<<16) | ((buf[3]&0xff)<<24) );	
}

float CasteRx1::getFloat(unsigned char *buf)
{
	unsigned int uintL;
	uniFloat uniF;
	
	uintL=(buf[0]&0xff)|((buf[1]&0xff)<<8)|((buf[2]&0xff)<<16)|((buf[3]&0xff)<<24);
	uniF.i = uintL;
	
	return uniF.flt;	
}

double CasteRx1::getDouble(unsigned char *buf)
{
	unsigned int uintL;
	unsigned long long int uintH;
	uniDouble uniD;
	
	uintL=(buf[0]&0xff)|((buf[1]&0xff)<<8)|((buf[2]&0xff)<<16)|((buf[3]&0xff)<<24);
	uintH=(buf[4]&0xff)|((buf[5]&0xff)<<8)|((buf[6]&0xff)<<16)|((buf[7]&0xff)<<24);
	//uniD.i = uintL|((uintH&0xffff)<<32);
	uniD.i = uintL|uintH<<32;
	
	return uniD.dbl;	
}

int CasteRx1::sendCommand(const string & cmnd)
{
	int nn;
	unsigned char replyBuffer[200];
	string reply, expectedReply;
	bool headerFound;
	
	//relax if we concatenate commands. (200 ms were suggested by Septentrio support team in a personal communication). 50ms tested and it seems it is not enough!!!
	usleep(200000);
	
	//flushes data at serial HW I/O buffer
	//tcflush(serialFD, TCIOFLUSH); 

	//sends the command string
	write(serialFD, cmnd.c_str(), cmnd.size());

	//synchronize header
	//usleep(50000);
	status = synchronizeHeader(serialFD,(const char *)"$R");
	if ( status != BASICSUCCESS )
	{
		cout << "CasteRx1::sendCommand(). ERROR. when synchronizing with header. Expected header = $R" <<  endl;
		return status; //it will be either COMMTIMEOUT or SERIALCOMMERROR
	}

	//reads response up to receive expected length bytes (2 + cmnd.size()). Expected response : "$R: <cmnd>", but $R has been already read
	nn = blockingRead(serialFD, &replyBuffer[0], cmnd.size() + 2);
	if ( nn < 0 )
	{
		return nn;//if error in blockingRead(), nn is an error code
	}

	//check response
	if ( replyBuffer[0] == '?' ) 
	{
		cout << "CasteRx1::sendCommand(). Error. Unknown command: " << cmnd << endl;
		return DEVICECONFIGERROR;
	}
	replyBuffer[nn] = '\n';
	replyBuffer[nn+1] = '\0';
	reply.append((const char*)replyBuffer);
	expectedReply = ": ";
	expectedReply.append(cmnd);

	if ( reply.compare(0, cmnd.size()+1, expectedReply, 0, cmnd.size()+1) == 0) //expected = received
	{
		//cout << "CasteRx1::sendCommand(). Command Successful: " << cmnd << endl;
		return BASICSUCCESS;
	}
	else //otherwise
	{
		cout << "CasteRx1::sendCommand(). Error. " << endl;
		cout << "      Sent   command: " << cmnd << endl;
		cout << "      Device   Reply: " << reply << endl;
		cout << "      Expected Reply: " << expectedReply << endl;
		return DEVICECONFIGERROR;
	}
}

int CasteRx1::synchronizeHeader(int ffdd, const char *mark, unsigned int markSize, double timeout)
{
	unsigned char readHeader[2]; 
	bool endLoop = false;
	int brResult = 1;
	int retValue;

	//brResult = blockingRead(ffdd,&readHeader[0],2,timeout);
	
	readHeader[0]=0x0;
	readHeader[1]=0x0;
	
	while (!endLoop)
	{
		while ( ( (char)readHeader[0] != mark[0] ) && ( brResult == 1 ) )
		{
			brResult = blockingRead(ffdd,&readHeader[0],1,timeout);
			//cout << "synchronizeHeader()_0: " << readHeader[0] << endl;
		}
		if (brResult == 1 ) //first char occurrence encountered in the previous loop 
		{
			brResult = blockingRead(ffdd,&readHeader[1],1,timeout);
			//cout << "synchronizeHeader()_1: " << readHeader[1] << endl;
			if ( brResult == 1 ) //success
			{
				if ( ((char)readHeader[1] != mark[1]) && ((char)readHeader[1] != mark[0]) )
				{
					readHeader[0] = 0x0; //continue in the loop from the start
				}
				if((char)readHeader[1] == mark[1]) 
				{
					endLoop = true;
					retValue = BASICSUCCESS;
				}
				if((char)readHeader[1] == mark[0])//first char is transmitted more than once concatenated
				{
					readHeader[0] = mark[0]; //continue in the loop but first character is already ok
				}
			}
			else
			{
				endLoop = true;
				retValue = brResult; //some error code returned by blockingRead()
			}
		}
		else 
		{
			endLoop = true;
			retValue = brResult; //some error code returned by blockingRead()
		}
	}
	
	return retValue;
}

int CasteRx1::decodePVTCartesian(unsigned char *sbfMsg)
{
	unsigned short int mode;//PVT computation mode (e.g. standAlone, fixed, differential, rtk,...)
	
	tow = getUI4(&sbfMsg[0]);
	wnc = getUI2(&sbfMsg[4]);
	mode = getUI1(&sbfMsg[6]);
	pvtError = getUI1(&sbfMsg[7]);
	if ( pvtError!=0 ) //GPS error to obtain position (this is not an acquisition error)
	{
		//cout << "CasteRx1::decodePVTCartesian(): PVT_ERROR: " << pvtError << endl;
	}
	else
	{
		xWgs = getDouble(&sbfMsg[8]);
		yWgs = getDouble(&sbfMsg[16]);	
		zWgs = getDouble(&sbfMsg[24]);
		undulation = (double) getFloat(&sbfMsg[32]);
		vxWgs = (double) getFloat(&sbfMsg[36]);
		vyWgs = (double) getFloat(&sbfMsg[40]);
		vzWgs = (double) getFloat(&sbfMsg[44]);
		numSatellites = getUI1(&sbfMsg[66]);
	}

	//cout << "CasteRx1::decodePVTCartesian()" << endl;
	return BASICSUCCESS;
}

int CasteRx1::decodePVTGeodetic(unsigned char *sbfMsg)
{
	unsigned short int mode;//PVT computation mode (e.g. standAlone, fixed, differential, rtk,...)
		
	tow = getUI4(&sbfMsg[0]);
	wnc = getUI2(&sbfMsg[4]);
	mode = getUI1(&sbfMsg[6]);
	pvtError = getUI1(&sbfMsg[7]);
	if ( pvtError!=0 ) //GPS error to obtain position (this is not an acquisition error)
	{
		//cout << "CasteRx1::decodePVTGeodetic(): PVT_ERROR: " << pvtError << endl;
	}
	else 
	{
		lat = getDouble(&sbfMsg[8]);
		lon = getDouble(&sbfMsg[16]);
		alt = getDouble(&sbfMsg[24]);
	}
	
	//cout << "CasteRx1::decodePVTGeodetic()" << endl;	
	return BASICSUCCESS;
}

int CasteRx1::decodePosCovCartesian(unsigned char *sbfMsg)
{	
	unsigned short int mode;//PVT computation mode (e.g. standAlone, fixed, differential, rtk,...)

	tow = getUI4(&sbfMsg[0]);
	wnc = getUI2(&sbfMsg[4]);
	mode = getUI1(&sbfMsg[6]);
	pvtError = getUI1(&sbfMsg[7]);
	if ( pvtError!=0 ) //GPS error to obtain position (this is not an acquisition error)
	{
		//cout << "CasteRx1::decodePosCovCartesian(): PVT_ERROR: " << pvtError << endl;
	}
	else 
	{
		cxxWgs = (double) getFloat(&sbfMsg[8]);
		cyyWgs = (double) getFloat(&sbfMsg[12]);
		czzWgs = (double) getFloat(&sbfMsg[16]);
		cxyWgs = (double) getFloat(&sbfMsg[24]);
		cxzWgs = (double) getFloat(&sbfMsg[28]);
		cyzWgs = (double) getFloat(&sbfMsg[36]);	
	}
	
	//cout << "CasteRx1::decodePosCovCartesian()" << endl;	
	return BASICSUCCESS;
}

int CasteRx1::decodeDOP(unsigned char *sbfMsg)
{
	unsigned char nrsv;//total number of satellites for DOP computation

	tow = getUI4(&sbfMsg[0]);
	wnc = getUI2(&sbfMsg[4]);
	nrsv = getUI1(&sbfMsg[6]);

	PDOP = (double)getUI2(&sbfMsg[8])/100.0;
	TDOP = (double)getUI2(&sbfMsg[10])/100.0;
	HDOP = (double)getUI2(&sbfMsg[12])/100.0;
	VDOP = (double)getUI2(&sbfMsg[14])/100.0;		

	//cout << "CasteRx1::decodeDOP()" << endl;
	return BASICSUCCESS;
}

unsigned short int CasteRx1::computeCrc(unsigned char *buf, unsigned short int nBytes)
{
	//to do
}

void CasteRx1::computeTM()
{
	double kk; //constant
	double xx0,yy0,zz0; //wgs-ecef coordinates of the origin
	double auxTM[4][4];
	
	//sets the rotation part of TM, from ECEF to ENU coordinates
	auxTM[0][0] = -sin(lon0);
	auxTM[0][1] = cos(lon0);
	auxTM[0][2] = 0.0;
	auxTM[1][0] = -sin(lat0)*cos(lon0);
	auxTM[1][1] = -sin(lat0)*sin(lon0);
	auxTM[1][2] = cos(lat0);
	auxTM[2][0] = cos(lat0)*cos(lon0);
	auxTM[2][1] = cos(lat0)*sin(lon0);
	auxTM[2][2] = sin(lat0);

	//computes final alignement of x with respect to the campus baseline as a rotation of alpha0 on z axis
	//Ra=[cos(alpha0)  sin(alpha0) 0;
	//   -sin(alpha0)  cos(alpha0) 0;
	//	0           0          1];
	TM[0][0]=cos(alpha0)*auxTM[0][0] + sin(alpha0)*auxTM[1][0];
	TM[0][1]=cos(alpha0)*auxTM[0][1] + sin(alpha0)*auxTM[1][1];
	TM[0][2]=cos(alpha0)*auxTM[0][2] + sin(alpha0)*auxTM[1][2];
	TM[1][0]=-sin(alpha0)*auxTM[0][0] + cos(alpha0)*auxTM[1][0];
	TM[1][1]=-sin(alpha0)*auxTM[0][1] + cos(alpha0)*auxTM[1][1];
	TM[1][2]=-sin(alpha0)*auxTM[0][2] + cos(alpha0)*auxTM[1][2];
	TM[2][0]=auxTM[2][0]; 
	TM[2][1]=auxTM[2][1];
	TM[2][2]=auxTM[2][2];

	//computes the translation part
	kk = earthRadius/sqrt(1-(pow(earthExcentricity*sin(lat0),2.0)));
	xx0 = (kk + alt0) * cos(lat0) * cos(lon0);
	yy0 = (kk + alt0) * cos(lat0) * sin(lon0);
	zz0 = ((1-earthExcentricity*earthExcentricity)* kk + alt0) * sin(lat0);
	TM[0][3] = - ( TM[0][0]*xx0 + TM[0][1]*yy0 + TM[0][2]*zz0 );
	TM[1][3] = - ( TM[1][0]*xx0 + TM[1][1]*yy0 + TM[1][2]*zz0 );
	TM[2][3] = - ( TM[2][0]*xx0 + TM[2][1]*yy0 + TM[2][2]*zz0 );
	TM[3][0] = 0.0; 
	TM[3][1] = 0.0; 
	TM[3][2] = 0.0;
	TM[3][3] = 1.0;
}

void CasteRx1::fromWgsToMap()
{
	double auxM[3][3];
	int ii,jj;

	//compute map coords
	xMap=TM[0][0]*xWgs+TM[0][1]*yWgs+TM[0][2]*zWgs+TM[0][3];
	yMap=TM[1][0]*xWgs+TM[1][1]*yWgs+TM[1][2]*zWgs+TM[1][3];
	zMap=TM[2][0]*xWgs+TM[2][1]*yWgs+TM[2][2]*zWgs+TM[2][3];

	//compute velocities 
	vxMap=TM[0][0]*vxWgs+TM[0][1]*vyWgs+TM[0][2]*vzWgs;
	vyMap=TM[1][0]*vxWgs+TM[1][1]*vyWgs+TM[1][2]*vzWgs;
	vzMap=TM[2][0]*vxWgs+TM[2][1]*vyWgs+TM[2][2]*vzWgs;

	//compute covariance matrix
	//firts step: auxM=TM*covEcef
	for(ii=0;ii<3;ii++)
	{
		auxM[ii][0]=TM[ii][0]*cxxWgs+TM[ii][1]*cxyWgs+TM[ii][2]*cxzWgs;
		auxM[ii][1]=TM[ii][0]*cxyWgs+TM[ii][1]*cyyWgs+TM[ii][2]*cyzWgs;
		auxM[ii][2]=TM[ii][0]*cxzWgs+TM[ii][1]*cyzWgs+TM[ii][2]*czzWgs;
	}
	//second step: covMap = auxM*TM^T
	cxxMap=auxM[0][0]*TM[0][0]+auxM[0][1]*TM[0][1]+auxM[0][2]*TM[0][2];
	cyyMap=auxM[1][0]*TM[1][0]+auxM[1][1]*TM[1][1]+auxM[1][2]*TM[1][2];
	czzMap=auxM[2][0]*TM[2][0]+auxM[2][1]*TM[2][1]+auxM[2][2]*TM[2][2];
	cxyMap=auxM[0][0]*TM[1][0]+auxM[0][1]*TM[1][1]+auxM[0][2]*TM[1][2];
}

void CasteRx1::markBlockAsReceived(unsigned short int id)
{
	list<sbfBlockManager>::iterator iiBlock;	
	
	for (iiBlock=blockList.begin();iiBlock!=blockList.end();iiBlock++)
	{ 
		if ( iiBlock->blockId == id ) iiBlock->received = true;
	}
}
