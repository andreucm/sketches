
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
	unsigned int ii;
	BufferedPort<Bottle> myPort;
	string s1;
	int i1, i2;
	double d1, d2;
        double mat[16];
	
	//port initialization
	myPort.open("/subscriber/input");
	
	//port connection (subscription!)
// 	Network::connect("/publisher/output", myPort.getName(), "tcp");
        Network::connect("/cameratf/out", myPort.getName(), "tcp");

        //extend cout precision
        std::streamsize nn = cout.precision(20);
        
        //main loop 
	while(1)
	{
		//blocking reception
		Bottle * myBottle = myPort.read(true);

		//Unpacking Bottle content
		ii = 0;
                
// 		i1 = myBottle->get(ii++).asInt();
// 		d1 = myBottle->get(ii++).asDouble();
// 		s1 = myBottle->get(ii++).asString();
// 		d2 = myBottle->get(ii++).asDouble();
// 		i2 = myBottle->get(ii++).asInt();
// 
// 		//Printing data
// 		cout 	<< endl 
// 			<< "i1 = " << i1 << endl
// 			<< "d1 = " << d1 << endl
// 			<< "s1 = " << s1 << endl
// 			<< "d2 = " << d2 << endl
// 			<< "i2 = " << i2 << endl;

                //Printing data
//                 cout    << endl 
//                         << "i1 = " << i1 << endl
//                         << "d1 = " << d1 << endl
//                         << "s1 = " << s1 << endl
//                         << "d2 = " << d2 << endl
//                         << "i2 = " << i2 << endl;

                for (ii=0; ii<16; ii++) mat[ii] = myBottle->get(ii).asDouble(); 
                d1 = myBottle->get(ii++).asDouble();
                d2 = myBottle->get(ii++).asDouble();
                for (ii=0; ii<16; ii++) cout << mat[ii] << " " ;
                cout << endl << "d1 = " << d1 << "; d2 = " << d2 << endl;
	}
        
        //recover cout precision
	cout.precision(nn);                        
	
	//port disconnection (subscrption!)
	Network::disconnect("/publisher/output", myPort.getName(), "tcp");

	//closes port
	myPort.close();
	
	//exit
	return 1;
}
