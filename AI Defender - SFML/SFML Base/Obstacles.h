#pragma once
#ifndef OBSTACLES_H
#define OBSTACLES_H

#include "SFML\Graphics.hpp"
#include "ResourceLoader.h"
#include "GlobalVariables.h"

class Obstacles
{
private:
	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();

public:
	Obstacles();
	~Obstacles();

private:
	sf::Texture mTexture;
	sf::Sprite mSprite;

	sf::Vector2f mPositon;

	sf::Vector2f maxVelocity;
	sf::Vector2f velocity;
	sf::Vector2f direction;

	sf::RectangleShape collisionRect;

	sf::Vector2f mScale;

public:
	void Update();
	void Draw(sf::RenderWindow &window);

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f position);

	sf::RectangleShape getRect();

};

#endif
