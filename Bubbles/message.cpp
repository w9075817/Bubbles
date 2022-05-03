#include "message.h"

sf::Packet& operator << (sf::Packet& p, emsg_type e)
{
	sf::Uint8 t = static_cast<sf::Uint8>(e);
	return p << t;
}

sf::Packet& operator >> (sf::Packet& p, emsg_type& e)
{
	sf::Uint8 t;
	p >> t;
	e = static_cast<emsg_type>(t);
	return p;
}

sf::Packet& operator << (sf::Packet& p, const str_msg m)
{
	return (p << m.type << m.s);
}

sf::Packet& operator >> (sf::Packet& p, str_msg& m)
{
	return (p >> m.s);
}

sf::Packet& operator << (sf::Packet& p, const int_str_msg m)
{
	return (p << m.type << m.i << m.s);
}

sf::Packet& operator >> (sf::Packet& p, int_str_msg& m)
{
	return (p >> m.i >> m.s);
}

sf::Packet& operator << (sf::Packet& p, const move_msg m)
{
	return (p  << m.cannon_angle << m.isCannonReady << m.isMoving << m.velocityX << m.score << m.velocityY << m.ballcolour << m.isShooting);
}

sf::Packet& operator >> (sf::Packet& p, move_msg& m)
{
	return (p >> m.cannon_angle >> m.isCannonReady >> m.isMoving >> m.velocityX >> m.score >> m.velocityY >> m.ballcolour >> m.isShooting);
}