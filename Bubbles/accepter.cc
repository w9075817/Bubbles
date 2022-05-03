#include "accepter.hpp"
#include "receiver.hpp"
#include "util.hpp"
#include <iostream>
#include <sstream>
#include <thread>

Accepter::Accepter(Queue<sf::Packet>& q, List<std::shared_ptr<sf::TcpSocket>>& s):
    queue_(q),
    socket_(s)
{}

void Accepter::operator()()
{
    int player = 1;
    unsigned int seed = time(NULL);
    sf::TcpListener listener;
    if (listener.listen(55002) != sf::Socket::Done)
    {
        std::cerr << "Error listening to port\n";
        return;
    }
    std::cout << "Bound to port\n";
    while (true)
    {
        std::shared_ptr<sf::TcpSocket> socket = std::make_shared<sf::TcpSocket>();
        if (listener.accept(*socket) != sf::Socket::Done)
        {
            std::cerr << "cant accept connection\n";
            return;
        }
        socket_.push(socket);
        std::stringstream ss;
        ss << "Accepted a connection from: "
            << socket->getRemoteAddress()
            << ":"
            << socket->getRemotePort()
            << std::endl;
        std::cout << ss.str();
        std::shared_ptr<Receiver> receiver = std::make_shared<Receiver>(socket, queue_);

        sf::Packet seedName;
        seedName << player << seed;
        sf::Packet seedSend = seedName;
        seedSend << player << seed;
        sf::Socket::Status statusSeed = socket->send(seedSend);
        if (statusSeed != sf::Socket::Done)
        {

        }
        player++;


        std::thread(&Receiver::recv_loop, receiver).detach();
    }
}

