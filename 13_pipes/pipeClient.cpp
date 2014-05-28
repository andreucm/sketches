
#include "commConstants.h"

using namespace std;

int main(int argc, char *argv[])
{
	int res, pipeFd, ii, jj, kk, ll; 
	char buffer[PACKETSIZE];
	uniUint clientId;
	uniFloat aa;
	
	if (argc != 2)
	{
		cout << "Please specify an integer as clientId." << endl;
		cout << "*********** Exit program! ************" << endl;
		return -1;
	}
	clientId.i = atoi(argv[1]);//gets clientId from user entry

	if (access(FIFONAME, F_OK) == -1)
	{
		cout <<"Error accessing to " << FIFONAME << ". No pipe created by the Server" << endl;
		return(-1);
	}
	pipeFd = open(FIFONAME, O_WRONLY);//opens fifo for writing
	
	for (ii=0; ii<NREQUESTS; ii++)
	{
		jj=0;//reset packet byte index
		buffer[jj++]='a';//sets service Id
		for (ll=0; ll<UINTSIZE; ll++) buffer[jj++] = clientId.c[ll];//sets client id to the packet
		for (kk=0; kk<NFLOATS; kk++)
		{
			//aa.f = (float)ii * NREQUESTS + ii + (float)kk/(float)NFLOATS; //computes the float number
			aa.f = (float)kk;
			for (ll=0; ll<FLOATSIZE; ll++) buffer[jj++]=aa.c[ll];
		}

		res = write (pipeFd, buffer, PACKETSIZE);
		if (res!=PACKETSIZE)
		{
			cout << "Error on packet sending. Expected Bytes = " << PACKETSIZE << "; Sent Bytes = " << res << endl;
		}
		//sleep(1);
	}

	jj=0;
	buffer[jj++]='q';//at the end, sends a "quit" service, to force ending server process
	for (ll=0; ll<UINTSIZE; ll++) buffer[jj++] = clientId.c[ll];//sets client id to the packet
	write (pipeFd,buffer,1+UINTSIZE); 
	sleep(1);
	//close(pipeFd);
	
	return 0;
}
