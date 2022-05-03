#include "UDPreceiver.h"
#define BUFF_SIZE 1024

UDPreceiver::UDPreceiver(sf::UdpSocket& s) : socket(s)
{
}

void UDPreceiver::receive()
{
	while (true)
	{
		sf::IpAddress recipient;
		unsigned short remote_port;
		char data[BUFF_SIZE];
		std::size_t received;
		std::memset(data, 0, BUFF_SIZE);
		if (socket.receive(data, BUFF_SIZE, received, recipient, remote_port) != sf::Socket::Done)
		{
			std::cerr << "Problems receiving" << std::endl;
			exit(0);
		}
		else
		{
			std::string s = std::string(data);
			sf::Socket::Status status = socket.send(s.c_str(), s.length(), recipient, remote_port);
			if (status != sf::Socket::Done)
			{
				std::cerr << "Problems sending" << std::endl;
				exit(0);
			}
		}

	}
}
