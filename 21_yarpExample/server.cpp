
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
	int i1,i2,result;
	unsigned int idx;
	
	//program variables
	BufferedPort<Bottle> requestPort, replyPort;
	
	//port initialization
	requestPort.open("/server/request");
	replyPort.open("/server/reply");
	
	while(1)
	{
		//blocking reception. Waiting for requests
		Bottle * requestBottle = requestPort.read(true);

		//Unpacking Bottle content
		idx = 0;
		i1 = requestBottle->get(idx++).asInt();
		i2 = requestBottle->get(idx++).asInt();
		
		//process service
		result = i1 + i2;

		//reply
		Bottle & replyBottle = replyPort.prepare(); 
		replyBottle.clear();
		replyBottle.addInt(result);
		replyPort.write();
	}
	
	//closes port
	requestPort.close();
	replyPort.close();
	
	//exit
	return 1;
}
