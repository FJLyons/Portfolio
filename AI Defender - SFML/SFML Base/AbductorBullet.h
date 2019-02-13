#ifndef ABDUCTORBULLET_H
#define ABDUCTORBULLET_H

#include "SFML\Graphics.hpp"
#include "ResourceLoader.h"
#include "GlobalVariables.h"
#include "CollisionManager.h"

class AbductorBullet
{

private:
	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();

private:

	sf::Vector2f mPositon;
	sf::Vector2f mVelocity;
	sf::Texture mTexture;
	sf::Sprite mSprite;
	sf::Vector2f mDirection;
	sf::RectangleShape collisionRect;
	bool mfacingRight;
	bool alive;
	float ttl = 3;
	sf::Clock deltaClock;

public:



	AbductorBullet();
	AbductorBullet(sf::Vector2f pos, sf::Vector2f playerpos);
	bool Update();
	void Draw(sf::RenderWindow &window);

	sf::Sprite getSprite();
	sf::RectangleShape getRect();
};

#endif