#include <iostream>
#include <termios.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
//#include <cstdlib>



const unsigned int BUFFER_SIZE = 10;
const std::string portName = "/dev/ttyACM0";

int main()
{
        int retValue, commId;
        unsigned int ii; //byte counter
        unsigned char userChar = ' '; //user input
        unsigned char deviceChar = ' '; //device input  
        unsigned char buffer[BUFFER_SIZE];        
        termios ttySettings; //termios variable to configure serial port        
        termios stdInOldSettings, stdInNewSettings;                
//         timeval timeOut, timeOutLimit;
        fd_set fdSet;
        
        //1. opens serial port. read() will not block
        retValue = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK); 
        if ( retValue < 0 )
        {
                std::cout << "Error opening serial port: " << portName << std::endl;
                return -1;
        }
        else
        {
                std::cout << "Success opening serial port: " << portName << std::endl;
                commId = retValue;
        }
        
        //2. configures serial comm's
        tcgetattr(commId, &ttySettings);//gets current config
        ttySettings.c_cflag = ( B9600 | CLOCAL | CREAD | CS8 ); 
        ttySettings.c_iflag = ( IGNBRK ); //Ignores break condition on input
        ttySettings.c_oflag = 0x0;
        ttySettings.c_lflag = 0x0;
        retValue = tcsetattr(commId, TCSANOW, &ttySettings); //Sets configuration immediately.
        if ( retValue < 0 )
        {
                std::cout << "Error configuring serial communications" << std::endl << std::endl;
                return -1;
        }
        else
        {
                std::cout << "Success configuring serial communications" << std::endl << std::endl;
        }
        
        //3. configure stdin & select
        //Important! ( please see http://www.cplusplus.com/forum/general/5304/ )
        tcgetattr( STDIN_FILENO, &stdInOldSettings );
        stdInNewSettings = stdInOldSettings;
        stdInNewSettings.c_lflag &= (~ICANON & ~ECHO);
        tcsetattr( STDIN_FILENO, TCSANOW, &stdInNewSettings );
        
        //relax to allow devices to be ready
        sleep(1);
        
        //3. main loop. Receives user inputs, sends them to serial device, waits for device answer
        while (userChar != 'e')
        {
//                 //prompts new command
//                 std::cout << "New command: " << std::endl;
// 
//                 //configures fdSet for keyboard entry
//                 FD_ZERO (&fdSet);
//                 FD_SET (STDIN_FILENO, &fdSet);
// 
//                 //waits for an input from keyboard
//                 retValue = select (STDIN_FILENO+1, &fdSet,NULL,NULL,NULL);
// 
//                 //reads user byte
//                 read(STDIN_FILENO,&userChar,1);
//                 
//                 //sends command to device
//                 write(commId,&userChar,1);
//                 std::cout << "Command Sent: " << (unsigned int)userChar << std::endl;                

                //reads answer from device up to '\n' arrives
                ii = 0;//reset byte counter
                do
                {
                        //reconfigure fdSet for device entry
                        FD_ZERO (&fdSet);
                        FD_SET (commId, &fdSet);

                        //blocking read up to an input arrives to some channel at fdSet (stdin)
                        retValue = select (commId+1, &fdSet,NULL,NULL,NULL);
                        
                        //reads ready byte from device
                        read(commId,&deviceChar,1);          
                        
                        //prints counter and char to stdout (debugging)
                        //std::cout << ii << ": " << (unsigned int)deviceChar << std::endl;

                        //stores deviceChar in the buffer
                        buffer[ii] = deviceChar;
                        ii++;
                        
                }while ( ( deviceChar != '\n' ) || ( ii == BUFFER_SIZE-1 ) );
                buffer[ii]='\0';
                
                std::cout << buffer << std::endl;
        }
        
        //closes serial comms and restores old settings 
        close(commId);
        tcsetattr( STDIN_FILENO, TCSANOW, &stdInOldSettings );
        return 0;
}
