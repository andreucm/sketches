
#include "offLinePublisher.h"

int main()
{
	unsigned int ii; 
	unsigned int retValue = RUNNING;
	
	Network yarp;//initializes yarp netwok
	
	CoffLinePublisher olp(0.0, "../logFiles/fileList.txt", true);
	olp.initializeNow();
	
	Network::connect("/offLine/portB", "/offLine/debug/reader", "tcp");//connect for debugging
	
	while (retValue == RUNNING)
	{
		retValue = olp.publishNextData();
	}

	return 0;
}
