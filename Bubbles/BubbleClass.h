#pragma once
#include <SFML/Network.hpp>

class BubbleClass
{
public:
	BubbleClass(int color, sf::Vector2f position);

	int colour;
	sf::Vector2f pos;
	int type;
	bool isDestroyed;
};

