// UDP sender
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h> //memset()
#include <iostream>
#include <vector>
#include <iomanip>

const unsigned short PORT = 8080;
const unsigned int DATAGRAM_BUFFER_SIZE = 18;

union double2byte {
	double d;
	unsigned char b[8];
};

int main()
{
	int socket_fd;
	std::vector<unsigned char> datagram_buffer(DATAGRAM_BUFFER_SIZE);
	struct sockaddr_in server_address;
	double2byte pi;

	// Creating socket file descriptor
	if ( (socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		std::cout << "Socket creation failed" << std::endl;
		return -1;
	}

	// Filling server information
	memset(&server_address, 0, sizeof(server_address)); //clear all
	server_address.sin_family = AF_INET; //IPv4
	server_address.sin_port = htons(PORT);//converts the unsigned short argument from host byte order to network byte order.
	server_address.sin_addr.s_addr = INADDR_ANY; //any internet address

	//fill the message with 10 ints and 1 double
	for (unsigned int ii=0; ii<10; ii++)
		datagram_buffer.at(ii) = ii;
	pi.d = 3.141592;
	for (unsigned int ii=10; ii<datagram_buffer.size(); ii++)
		datagram_buffer.at(ii) = pi.b[ii-10];

	// send the message
	sendto(	socket_fd, datagram_buffer.data(), datagram_buffer.size(),
			MSG_CONFIRM, (const struct sockaddr *) &server_address,
			sizeof(server_address));

	// print the sent message
	std::cout << "Message sent: ";
	for (unsigned int ii=0; ii<datagram_buffer.size(); ii++)
		std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)datagram_buffer.at(ii) << " ";
	std::cout << std::dec << std::endl;

	//close the socket and return
	close(socket_fd);
	return 0;
}
