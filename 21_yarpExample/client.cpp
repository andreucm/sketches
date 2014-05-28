
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
	unsigned int ii, numRequests, idx;
	BufferedPort<Bottle> requestPort, replyPort;
	int i1, i2, result;
	
	//user arguments
	if (argc != 2)
	{
		cout << "client: Needs 1 input parameter: <num requests> " << endl;
		cout << "***** Exit program! *****" << endl;
		return -1;
	}
	numRequests = atoi(argv[1]);

	//port initialization
	requestPort.open("/client/request");
	replyPort.open("/client/reply");	
	
	//port connections
	Network::connect(requestPort.getName(), "/server/request", "tcp");
	Network::connect("/server/reply", replyPort.getName(), "tcp");
	
	//main loop (processing & publishing)
	for(ii=1;ii<numRequests;ii++)
	{
		//prepare request parameters
		i1 = ii;
		i2 = i1*2;
		
		//send request
		Bottle & requestBottle = requestPort.prepare(); 
		requestBottle.clear();
		requestBottle.addInt(i1);
		requestBottle.addInt(i2);
		requestPort.write();
		cout << "Send request: (" << i1 << "," << i2 << ")" << endl;
		
		//wait for reply
		Bottle * replyBottle = replyPort.read(true);
		
		//gets result
		idx = 0;
		result = replyBottle->get(idx++).asInt();
		cout << "Get reply: " << result << endl;
		
		//relax request rate (not necessary since client server are synchronized)
		usleep(500000);
	}
	
	//port disconnections
	Network::disconnect(requestPort.getName(), "/server/request", "tcp");
	Network::disconnect("/server/reply", replyPort.getName(), "tcp");
	
	//closes port
	requestPort.close();
	replyPort.close();

	//exit
	return 1;
}
