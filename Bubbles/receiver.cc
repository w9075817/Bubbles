#include "receiver.hpp"
#include "util.hpp"
#include <chrono>
#include <cstring>
#include <iostream>
#include <iostream>
#include <sstream>
#include <thread>


Receiver::Receiver(std::shared_ptr<sf::TcpSocket> s,
        Queue<sf::Packet> &q):
    socket_(s),
    queue_(q)
{
}

void Receiver::recv_loop()
{
    char buffer[256];
    {
        std::stringstream ss;
        ss << socket_->getRemoteAddress() << ":" << socket_->getRemotePort() << std::endl;
        std::cout << ss.str();
    }
    std::size_t received;
    sf::Packet packet;
    while(1)
    {

        if (socket_->receive(packet) != sf::Socket::Done)
        {
            std::cout << "Problem receiving from client\n";
                return;
        }
        sf::TcpSocket* tcp = socket_.get();
        queue_.push(packet);
    }
}