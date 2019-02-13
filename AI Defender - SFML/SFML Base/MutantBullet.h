#ifndef MUTANTBULLET_H
#define MUTANTBULLET_H

#include "SFML\Graphics.hpp"
#include "ResourceLoader.h"
#include "GlobalVariables.h"
#include "CollisionManager.h"

class MutantBullet
{

private:
	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();

private:

	sf::Vector2f mPositon;
	sf::Vector2f mVelocity;
	sf::Texture mTexture;
	sf::Sprite mSprite;
	sf::Vector2f mDirection;
	sf::Vector2f mPlayerDirection;
	sf::Vector2f mPlayerVelocity;
	sf::RectangleShape collisionRect;
	bool mfacingRight;
	bool alive;
	float ttl = 3;
	sf::Clock deltaClock;

public:



	MutantBullet();
	MutantBullet(sf::Vector2f pos, sf::Vector2f playerpos, sf::Vector2f playerVel);
	bool Update();
	void Draw(sf::RenderWindow &window);

	sf::Sprite getSprite();
	sf::RectangleShape getRect();
};

#endif