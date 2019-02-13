#ifndef BULLET_H
#define BULLET_H

#include "SFML\Graphics.hpp"

#include "GlobalVariables.h"

class Bullet
{

private:
	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();

private:

	//sf::Vector2f mPositon;
	//sf::Vector2f mVelocity;
	//sf::Texture mTexture;
	//sf::Sprite mSprite;

	//bool mfacingRight;
	//bool alive;

	//float ttl = 3;

	//sf::Clock deltaClock;
public:

	sf::Vector2f mPositon;
	sf::Vector2f mVelocity;
	sf::Texture mTexture;
	sf::Sprite mSprite;
	sf::RectangleShape collisionRect;
	bool mfacingRight;
	bool alive;
	float ttl = 1;
	sf::Clock deltaClock;

	Bullet();
	Bullet(sf::Vector2f pos, sf::Texture& tex, bool facingRight, sf::Vector2f direction);
	bool Update();
	void Draw(sf::RenderWindow &window);

	sf::Sprite getSprite();
	sf::RectangleShape getRect();
};

#endif