
#include <iostream>
#include <string>
#include <cstdlib>
#include <yarp/os/Bottle.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Time.h>
#include <yarp/os/Network.h>

using namespace yarp::os;
using namespace std;

int main(int argc, char *argv[])
{
	// Initialize YARP - some OSes need network and time service initialization
	Network yarp;
	
	//program variables
	unsigned int ii, numIterations;
	BufferedPort<Bottle> myPort;
	string s1;
	int i1, i2;
	double d1, d2;
	
	//user arguments
	if (argc != 2)
	{
		cout << "publisher: Needs 1 input parameter: <num iterations> " << endl;
		cout << "***** Exit program! *****" << endl;
		return -1;
	}
	numIterations = atoi(argv[1]);

	//port initialization
	myPort.open("/publisher/output");
	
	//main loop (processing & publishing)
	for(ii=1;ii<numIterations;ii++)
	{
		//do processing
		i1 = ii;
		i2 = i1*2;
		d1 = 1.0/(double)ii;
		d2 = d1*2;
		s1 = "Hello PipeGuards";
		
		//publish data
		Bottle & myBottle = myPort.prepare(); 
		myBottle.clear();
		myBottle.addInt(i1);
		myBottle.addDouble(d1);
		myBottle.addString(s1.c_str());
		myBottle.addDouble(d2);
		myBottle.addInt(i2);
		myPort.write();
		
		//relax output rate
		usleep(500000);
	}
	
	//closes port
	myPort.close();

	//exit
	return 1;
}
