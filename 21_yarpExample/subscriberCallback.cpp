
#include <iostream>
#include <string>
#include <cstdlib>
#include <yarp/os/Bottle.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Time.h>
#include <yarp/os/Network.h>

using namespace yarp::os;
using namespace std;

class cbBufferedPort : public BufferedPort<Bottle>
{
	protected:
		string s1;
		int i1, i2;
		double d1, d2;
		
	public:
		virtual void onRead(Bottle& bot) //callback function
		{
			int ii = 0;
			i1 = bot.get(ii++).asInt();
			d1 = bot.get(ii++).asDouble();
			s1 = bot.get(ii++).asString();
			d2 = bot.get(ii++).asDouble();
			i2 = bot.get(ii++).asInt();			
			
			printData();
		}
		void printData()
		{
			cout 	<< endl 
				<< "i1 = " << i1 << endl
				<< "d1 = " << d1 << endl
				<< "s1 = " << s1 << endl
				<< "d2 = " << d2 << endl
				<< "i2 = " << i2 << endl;
		}
};

int main(int argc, char *argv[])
{	
	// Initialize YARP - some OSes need network and time service initialization
	Network yarp;
	
	//program variables
	unsigned int ii;
	cbBufferedPort myPort;
	
	//port initialization
	myPort.open("/subscriber/input");
	myPort.useCallback();
	
	//port connection (subscrption!)
	Network::connect("/publisher/output", myPort.getName(), "tcp");
	
	while(1)
	{
		cout << "I'm looping faster than data reception!" << endl;
		usleep(100000);
	}

	//port disconnection (subscrption!)
	Network::disconnect("/publisher/output", myPort.getName(), "tcp");

	//closes port
	myPort.close();
	
	//exit
	return 1;
}
