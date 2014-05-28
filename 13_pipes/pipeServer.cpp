
#include "commConstants.h"

using namespace std;

int main()
{
	bool run=1;
	int res, pipeFd, ii, jj, kk; 
	char serviceId, buffer[PACKETSIZE];
	unsigned int clientId;
	float value;
	uniFloat aa;
	uniUint uu;
	
	//checks if fifo already exist
	if (access(FIFONAME, F_OK) == -1)
	{
		res = mkfifo(FIFONAME, 0777);
		if (res != 0)
		{
			cout << "Error creating fifo: " <<  FIFONAME << "; ErrorId: " << res << endl;
			return(-1);
		}
	}
	
	//opens fifo
	pipeFd = open(FIFONAME, O_RDONLY);//opens fifo for reading with blocking. Blocks here up to some writer arrives
	if (pipeFd == -1)
	{
		cout << "Error opening fifo: " <<  FIFONAME << "; ErrorId: " << res << endl;;
		return(-1);
	}
	else 	cout << FIFONAME << " fifo opened successfully" << endl;
	
	//checks PACKETSIZE and warns user if it exceeds PIPE_BUF
	if (PACKETSIZE > PIPE_BUF)
	{
		cout << "WARNING: PACKETSIZE > PIPE_BUF ( defined at limits.h )" << endl
			<< "This not ensure atomicity in fifo writing" << endl
			<< "Possible malfunctions in multi-client situations" << endl;
	}
	
	while(run)
	{
		res = read(pipeFd, buffer, 1);//blocking here up to receiving some data
		serviceId = buffer[0];//gets the first char indicating service Id
		switch(serviceId)//service switch		
		{
			case 'a':
				res = read(pipeFd, buffer, PACKETSIZE-1);//read the rest of the packet
				if ( res!=PACKETSIZE-1 )
				{
					cout << "Error on packet reception. Expected Bytes = " << PACKETSIZE <<"; Received Bytes = " << res << endl;
				}
				else
				{
					ii=0;//reset packet byte index
					for (jj=0; jj<UINTSIZE; jj++) uu.c[jj] = buffer[ii++];//gets an unsigned int from the packet
					clientId = uu.i;
					for (kk=0; kk<NFLOATS; kk++)//receives floats 
					{
						for (jj=0; jj<FLOATSIZE; jj++) aa.c[jj] = buffer[ii++]; //gets a float from buffer
						value = aa.f;
						if ( value != (float)kk ) //checks the received value
						{
							cout << "Error. Value = " << value << endl;
						}
					}
				}
				break;
			case 'q':
				run=0; //force quit
				break;
			default:
				cout << "unknown service request" << endl;
				break;
		}
	}
	
	close(pipeFd);
	return 0;
}
