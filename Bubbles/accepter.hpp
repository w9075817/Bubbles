#ifndef ACCEPTER_HPP
#define ACCEPTER_HPP

#pragma once

#include "queue.hpp"
#include "list.hpp"
#include <SFML/Network.hpp>
#include "message.h"
#include "util.hpp"

class Accepter {
    private:
        Queue<sf::Packet>& queue_;
        List<std::shared_ptr<sf::TcpSocket>>& socket_;

    public:
        Accepter(Queue<sf::Packet>& q, List<std::shared_ptr<sf::TcpSocket>>& s);
        void operator()();
};


#endif
