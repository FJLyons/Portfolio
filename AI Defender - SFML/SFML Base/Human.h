#pragma once
#ifndef HUMAN_H
#define HUMAN_H

#include "SFML\Graphics.hpp"
#include "ResourceLoader.h"
#include "GlobalVariables.h"

class Human
{

private:
	sf::Texture mTexture;
	sf::Sprite mSprite;

	sf::Texture mindicatorTexture;
	sf::Sprite mindicatorSprite;

	sf::Vector2f mPositon;
	sf::Vector2f velocity;
	sf::Vertex* targetPoints;
	sf::RectangleShape collisionRect;
	sf::Transform transform;
	float angle;
	float previousYpos;
	bool targeted;
	bool abducted;
	bool falling;
	int currentPoint;

public:
	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();

	Human();
	Human(sf::Vertex* points);

	void Update(sf::Vector2f playerPos);

	void Draw(sf::RenderWindow &window, bool mini);
	bool getTargeted();
	void setTargeted(bool target);
	void setAbducted(bool abduct);
	void setFalling(bool fall);
	bool getFalling();
	sf::RectangleShape getRect();
	sf::Vector2f getPosition();
	int getPoint();
	void setPoint(int index);
};

#endif
