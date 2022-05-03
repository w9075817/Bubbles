#include "server.h"
#include "accepter.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include "queue.hpp"
#include "list.hpp"
#include "receiver.hpp"
#include "util.hpp"
#include "UDPreceiver.h"

void Server::CreateServer()
{
    List<std::shared_ptr<sf::TcpSocket>> sockets;
    sf::UdpSocket socket;
    unsigned int local_port = 55000;
    if (socket.bind(local_port) != sf::Socket::Done)
    {
        std::cerr << "Failed to bind to port";
        exit;
    }

    std::thread(Accepter(queue, sockets)).detach();
    std::shared_ptr<UDPreceiver> reciever = std::make_shared<UDPreceiver>(socket);
    std::thread(&UDPreceiver::receive, reciever).detach();
    sf::IpAddress recipient;
    unsigned short remote_port;

    while (1)
    {
        auto s = queue.pop();
        sf::Packet packet1 =s;
        //sf::TcpSocket* otherPacket = s2;
        //std::cout << "main read: \"" << s.angle << "\"\n";
        auto send = [&packet1](std::shared_ptr<sf::TcpSocket> socket)
        {
            sf::TcpSocket* current = socket.get();
            short c = current->getLocalPort();
            sf::Socket::Status status;
            //if (current != otherPacket)
            {
                status = socket->send(packet1);
                if (status != sf::Socket::Done)
                {
                    std::cerr << "Error sending to client\n";
                }
            }
            return;
        };
        sockets.for_each(send);
    }
}
