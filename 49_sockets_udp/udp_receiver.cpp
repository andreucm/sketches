// UDP receiver
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
	struct sockaddr_in server_address, client_address;
	double2byte pi;

	// Creating socket file descriptor
	if ( (socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		std::cout << "Socket creation failed" << std::endl;
		return -1;
	}

	//clear addresses
	memset(&server_address, 0, sizeof(server_address));
	memset(&client_address, 0, sizeof(client_address));

	// Filling server information
	server_address.sin_family = AF_INET; // IPv4
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	// Bind the socket with the server address
	if ( bind(socket_fd, (const struct sockaddr *)&server_address,
			sizeof(server_address)) < 0 )
	{
		std::cout << "Socket bind failed" << std::endl;
		return -1;
	}

	// block until reception
	int n_bytes;
	socklen_t len;
	n_bytes = recvfrom(	socket_fd, datagram_buffer.data(), datagram_buffer.size(),
						MSG_WAITALL, ( struct sockaddr *) &client_address, &len);

	// print the received message
	std::cout << "Message received: ";
	for (unsigned int ii=0; ii<datagram_buffer.size(); ii++)
		std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)datagram_buffer.at(ii) << " ";
	std::cout << std::dec << std::endl;

	//close the socket and return
	close(socket_fd);
	return 0;
}
