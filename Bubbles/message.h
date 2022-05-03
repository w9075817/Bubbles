#ifndef PACKET_HPP
#define PACKET_HPP

#include <SFML/Network.hpp>
#include <iostream>
#include <ctime>

enum class emsg_type : sf::Uint8
{
	str,
	int_str,
	registration, //id username
	move,		  //id angle isShooting isMoving isMovinLeft
	victory		  //username
};

sf::Packet& operator << (sf::Packet& p, emsg_type e);
sf::Packet& operator >> (sf::Packet& p, emsg_type& e);

class msg
{
public:
	emsg_type type;
	msg(emsg_type t) : type(t) {}
};

class str_msg : public msg
{
public:
	str_msg() : msg(emsg_type::str) {}
	std::string s;
};

sf::Packet& operator << (sf::Packet& p, const str_msg m);
sf::Packet& operator >> (sf::Packet& p, str_msg& m);

class int_str_msg : public msg
{
public:
	int_str_msg() : msg(emsg_type::int_str) {}
	sf::Uint32 i;
	std::string s;
};

sf::Packet& operator << (sf::Packet& p, const int_str_msg m);
sf::Packet& operator >> (sf::Packet& p, int_str_msg& m);

class move_msg : public msg
{
public:
	move_msg() : msg(emsg_type::move) {}
	sf::Uint32 id;
		int ballcolour;
		float posX;
		float posY;
		float velocityX;
		float velocityY;
		int score;
		int cannon_angle;
		bool isCannonReady;
		bool isShooting;
		bool isMoving;
		bool isMovingLeft;
};

sf::Packet& operator << (sf::Packet& p, const move_msg m);
sf::Packet& operator >> (sf::Packet& p, move_msg& m);


#endif //MESSAGE_HPP