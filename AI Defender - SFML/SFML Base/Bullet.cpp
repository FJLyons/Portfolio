#include "Bullet.h"

Bullet::Bullet()
{

}


Bullet::Bullet(sf::Vector2f pos, sf::Texture& tex, bool facingRight, sf::Vector2f direction)
{
	mPositon = pos;
	mTexture = tex;
	mSprite.setTexture(mTexture);
	mSprite.setPosition(mPositon);
	mSprite.setOrigin(sf::Vector2f(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2));
	mVelocity = sf::Vector2f(50, 0) + direction;
	collisionRect.setOrigin(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
	collisionRect.setSize(sf::Vector2f(mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height));
	collisionRect.setOutlineColor(sf::Color::Red);
	collisionRect.setFillColor(sf::Color::Transparent);
	collisionRect.setOutlineThickness(1);
	collisionRect.setPosition(mPositon);

	alive = true;

	if (facingRight == false)
	{
		mVelocity.x = -mVelocity.x;
	}

}

bool Bullet::Update()
{
	ttl -= deltaClock.getElapsedTime().asSeconds();
	sf::Time dt = deltaClock.restart();


	if (ttl <= 0)
	{
		alive = false;
	}

	mPositon += mVelocity;
	mSprite.setPosition(mPositon);
	collisionRect.setPosition(mPositon);
	return alive;
}
sf::Sprite Bullet::getSprite()
{
	return mSprite;
}

void Bullet::Draw(sf::RenderWindow &window)
{
	window.draw(mSprite);
	if (myGlobalOptions->drawCollisionBox)
	{
		window.draw(collisionRect);
	}
}


sf::RectangleShape Bullet::getRect()
{
	return collisionRect;
}