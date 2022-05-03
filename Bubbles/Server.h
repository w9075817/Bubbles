#pragma once
#include "queue.hpp"
#include "message.h"

class Server
{
public:
	Server(Queue<sf::Packet>& q) :queue(q){}
	Queue<sf::Packet>& queue;
	void CreateServer();
};