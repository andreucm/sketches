
#ifndef asteRx1_h
#define asteRx1_h

//include
#include <iostream>
#include <list>
#include <math.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>

union uniFloat {
  float flt;
  unsigned int i;
  //uint32_t i;
};

union uniDouble {
  double dbl;
  unsigned long long int i;
  //uint64_t i;
};

struct sbfBlockManager {
  unsigned short int blockId;
  bool received;
};

//physical constants
const double earthRadius = 6378137.0;
const double earthExcentricity = 0.081819190842622; 

//acquisition protocol (SBF protocol)
const unsigned short int PVTCartesianBlockId = 4006;
const unsigned short int PVTGeodeticBlockId = 4007;
const unsigned short int PosCovCartesianBlockId = 5905;
const unsigned short int DOPBlockId = 4001;

//status values
const int BASICSUCCESS = 0;
const int PARSINGERROR = -1;
const int DEVICECONFIGERROR = -2;
const int SERIALCOMMERROR = -3;
const int SBFPROTOCOLERROR = -4;
const int STOPPINGERROR = -5;
const int COMMTIMEOUT = -6;

//unit identifiers
const bool inRADS = 0;
const bool inDEGREES = 1;

using namespace std;

/**
  CasteRx1 implements continuous data acquisition from the asteRx1 GPS receiver. 
  Detailed documentation about SBF data transfer protocol can be found in the device manual.
  PLEASE NOTE: This driver only implements a part of the AsteRx1 device functionalities (the most useful part for mobile robot navigation).
  PLEASE NOTE: For extended functionalities , please refer to the manual.
  PLEASE NOTE: Feel free to improve and complete this driver
  Programmer: Andreu Corominas Murtra, acorominas@iri.upc.edu
  Device website: www.septentrio.com
*/
class CasteRx1
{
	public:
		/**
		  Constructor: just catch the name of the serial port
		*/
		CasteRx1(const string & hwPort); 
		
		/**
		  Destructor: closes serial Port in case it was opened
		*/
		virtual ~CasteRx1(); 
				
		/**
		  If success returns BASICSUCCESS. Otherwise returns SERIALCOMMERROR
		*/
		int openDevice(); 
		
		/**
		  Always returns BASICSUCCESS
		*/
		int closeDevice();
		
		/**
		  Configures the device to perform some signal processing basics for mobile robotics.
		  Starts a mode of operation of continuos data flow from the device at a rate of 1Hz.
		  If success returns BASICSUCCESS. Otherwise returns DEVICECONFIGERROR.
		*/
		int startAcquisition(); 
		
		/**
		  Stops mode of operation of continuous data flow
		  If success returns BASICSUCCESS. Otherwise returns STOPPINGERROR.
		*/ 
		int stopAcquisition(); 
		
		/**
		  Reads current data produced by the device. Ends when all data blocks requested have been received. 
		  Sets status and all data fields. 
		  Call getStatus() to know about success or fails during reading.
		*/
		void readDataFromDevice(); 
		
		/**
		  Prints all current data in a single row of the given ostream (cout or file)
		*/
		void printData(ostream & ost);		
		
		/**
		  Sets map origin and computes transformation matrix. 
		  Angles in degrees and altitude in meters.
		  A call to this function is mandatory if local coordinate related position is required.
		*/
		void setMapOrigin(double mapLat0, double mapLon0, double mapAlt0, double mapAlpha0); 
		
		/**
		  Prints the homogeneous transformation matrix used to convert data from ECEF-WGS to local map.
		*/
		void printTM(); 
		
		/**
		  Get data ...
		  TimeStamp is set by the computer running the driver.
		  Tow is the GPS "time of week".
		  PVTerror codes are explained in the device manual, pag XX.
		  Lat and Lon are expressed in radians by default. Pass "inDEGREES" if degree values are required.
		  *Wgs is data with respect to the ECEF-WGS coordinate frame.
		  *Map is data with respect to the local (map) coordinate frame.
		*/
		int getStatus();
		double getTimeStamp();
		unsigned short int getWnc();
		unsigned int getTow();
		unsigned int getNumSatellites();
		float getPDOP();
		float getTDOP();
		float getHDOP();
		float getVDOP();
		float getUndulation();
		unsigned short int getPVTerror();
		double getLat(bool units=inRADS); 
		double getLon(bool units=inRADS); 
		double getAlt();
		double getXWgs();
		double getYWgs();
		double getZWgs();
		double getXMap();
		double getYMap();
		double getZMap();
		double getVxWgs();
		double getVyWgs();
		double getVzWgs();
		double getVxMap();
		double getVyMap();
		double getVzMap();
		double getCxxWgs();
		double getCyyWgs();
		double getCzzWgs();
		double getCxyWgs();
		double getCxxMap();
		double getCyyMap();
		double getCzzMap();
		double getCxyMap();
		
	protected:
		//variables for device operation and configuration
		int serialFD; /**<Serial port file descriptor*/
		//fd_set set; /**<fd set for select()*/
		string portName; /**<serial port name*/
		list<sbfBlockManager> blockList; /**<this list manages which data blocks are requested and has been (or not) received in the current iteration*/
		double lat0, lon0, alt0, alpha0; /**<coordinates of the map origin. Alpha is the orientation of the north vector wrt the map X axis*/
		double TM[4][4]; /**homogeneous transformation matrix to convert from ecef-wgs84 to map coordinates*/
		
		//GPS data to be published
		int status;/**<Status related to the execution of this process*/
		double timeStamp; /**<time stamp set by the computer executing the process (not GPS time)*/
		unsigned short int wnc; /**<gps week number*/		
		unsigned int tow; /**<gps time of week in milliseconds*/
		unsigned int numSatellites; /**<number of visible satellites*/
		double PDOP, TDOP, HDOP, VDOP; /**<Dillution of precsion DOP's*/
		double undulation; /**<local geoid undulation between Datum ellipsoide and reference geoide*/
		unsigned short int pvtError; /**<error codes related to GPS position computation. (see page 50 of Firmware User Manual)*/
		double lat, lon, alt; /**<position in geodetic frame*/
		double xWgs, yWgs, zWgs; /**<position in WGS frame*/
		double xMap, yMap, zMap; /**<position in Map frame, a given reference frame*/
		double vxWgs,vyWgs,vzWgs; /**<velocities referred to WGS*/
		double vxMap, vyMap, vzMap; /**<velocities in Map frame, a given reference frame*/
		double cxxWgs,cyyWgs,czzWgs,cxyWgs,cxzWgs,cyzWgs; /**<covariance parameters of WGS position*/
		double cxxMap, cyyMap, czzMap, cxyMap; /**<covariance parameters in Map frame, a given reference frame*/
		
		/**
		  Opens serial port. 
		  If success returns serial port file descriptor. Otherwise returns SERIALCOMMERROR
		*/
		int openSerialComms(const string & hwPort); 
		
		/**
		  Configures serial port communications at: 115200, No Parity, 8 data bits, 1 stop Bit (8N1). 
		  (Values of masks in /usr/include/bits/termios.h)
		  Returns BASICSUCCESS or error code 
		*/
		int configSerialComms(int commFd);
		
		/**
		  Blocks up to reading nn bytes or to wait for secondsTimeOut. 
		  Return values are:
				nn if success and puts read bytes in the buffer.
				SERIALCOMMERROR if a comm error. 
				COMMTIMEOUT if timeout is reached.
		*/
		int blockingRead(int commFd, unsigned char *buf, int nn, double secondsTimeOut=1.0); 
		
		/**
		  Closes serial communications related to commFd file descriptor
		*/
		int closeSerialComms(int commFd);
		
		/**
		  Returns 1 byte integer in buffer (little-endian reading)
		*/
		unsigned short int getUI1(unsigned char *buf); 

		/**
		  Returns 2 bytes integer in buffer (little-endian reading)
		*/
		unsigned short int getUI2(unsigned char *buf);
		
		/**
		  Returns 4 bytes integer in buffer (little-endian reading)
		*/		
		unsigned int getUI4(unsigned char *buf);
		
		/**
		  Returns 4 byte float in buffer (little-endian reading)
		*/
		float getFloat(unsigned char *buf);
		
		/**
		  Returns 8 byte float in buffer (little-endian reading)
		*/
		double getDouble(unsigned char *buf);

		/**
		  Sends a command to the AsteRx1 device. 
		  If success returns BASICSUCCESS. Otherwise it returns an error code.
		*/
		int sendCommand(const string & cmnd); 

		/**
		  Synchronizes data reading with a given header mark. (markSixe in bytes).
  		  Return values are:
				BASICSUCCESS if success: the mark is found at the ffdd file descriptor input
				SERIALCOMMERROR if a comm error. 
				COMMTIMEOUT if timeout is reached.
		  Operational only for 2 byte header marks. 
		*/
		int synchronizeHeader(int ffdd, const char *mark, unsigned int markSize=2, double timeout=1.0);
		
		/**
		  Decodes PVTCartesian data block. Implementation of the SBF data transfer protocol.
		*/
		int decodePVTCartesian(unsigned char *sbfMsg);
		
		/**
		  Decodes PVTGeodetic data block. Implementation of the SBF data transfer protocol.
		*/		
		int decodePVTGeodetic(unsigned char *sbfMsg);
		
		/**
		  Decodes PosCovCartesian data block. Implementation of the SBF data transfer protocol.
		*/		
		int decodePosCovCartesian(unsigned char *sbfMsg);
		
		/**
		  Decodes DOP data block. Implementation of the SBF data transfer protocol.
		*/		
		int decodeDOP(unsigned char *sbfMsg);
		
		/**
		  computes the crc-ccitt of buf with polynomic 0x1021
		*/
		unsigned short int computeCrc(unsigned char *buf, unsigned short int nBytes);
		
		/**
		  Computes the matrix to transform positions from ECEF-WGS to a local coordinate frame.
		  Local coordinate frame origin should be set by public function setMapOrigin().
		*/
		void computeTM();
		
		/**
		  Transforms ECEF-WGS positions, velocities and covariances to local coordinate frame.
		  Local coordinate frame origin shouold be set by public function setMapOrigin().
		*/			
		void fromWgsToMap(); 
		
		/**
		  Sets to true the received field of the member in blockList that have blockId = id
		*/
		void markBlockAsReceived(unsigned short int id); 
};
#endif
