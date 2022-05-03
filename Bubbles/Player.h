#pragma once
#include <SFML/Network.hpp>

class Player
{
public:
	//Change wording!
	int angle;
	bool isCannonReady;
	bool isCannonShooting;
	bool isBallMoving;
	bool isBallMovingLeft;
	float velocityX;
	float velocityY;
	int score;
	int color;
	void SetPosition(int x, int y);
	void SetVelocity(sf::Vector2f newVelocity);
	void AddScore(int scoreAdd);
};

