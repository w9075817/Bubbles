#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#pragma once

#include <SFML/Network.hpp>
#include "queue.hpp"
#include "message.h"
#include "util.hpp"

class Receiver {
    public:
        Receiver(std::shared_ptr<sf::TcpSocket> s, Queue<sf::Packet>& queue);
        void recv_loop();
    private:
        std::shared_ptr<sf::TcpSocket> socket_;
        Queue<sf::Packet> &queue_;
};
#endif
