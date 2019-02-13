#include "MutantBullet.h"

MutantBullet::MutantBullet()
{
}

MutantBullet::MutantBullet(sf::Vector2f pos, sf::Vector2f playerpos, sf::Vector2f playerVel)
{
	mPositon = pos;
	mTexture = ResourceLoader::instance()->getenemyBulletTexture();
	mSprite.setTexture(mTexture);
	mSprite.setPosition(mPositon);
	mSprite.setOrigin(sf::Vector2f(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2));

	mDirection = playerpos - mPositon;
	mPlayerDirection = playerVel;

	collisionRect.setOrigin(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
	collisionRect.setSize(sf::Vector2f(mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height));
	collisionRect.setOutlineColor(sf::Color::Red);
	collisionRect.setFillColor(sf::Color::Transparent);
	collisionRect.setOutlineThickness(1);
	collisionRect.setPosition(mPositon);

	alive = true;

	mVelocity = sf::Vector2f(30, 30);
}

bool MutantBullet::Update()
{
	ttl -= deltaClock.getElapsedTime().asSeconds();
	sf::Time dt = deltaClock.restart();


	if (ttl <= 0)
	{
		alive = false;
	}

	mDirection = CollisionManager::instance()->NormaliseVector(mDirection);
	mPlayerDirection = CollisionManager::instance()->NormaliseVector(mPlayerVelocity);
	mDirection.x = mDirection.x + (mPlayerDirection.x*3);
	mDirection.y = mDirection.y + (mPlayerDirection.y*3);

	mPositon.x += mVelocity.x * mDirection.x;
	mPositon.y += mVelocity.y * mDirection.y;


	mSprite.setPosition(mPositon);
	collisionRect.setPosition(mPositon);
	return alive;
}

void MutantBullet::Draw(sf::RenderWindow & window)
{
	window.draw(mSprite);
	if (myGlobalOptions->drawCollisionBox)
	{
		window.draw(collisionRect);
	}
}

sf::Sprite MutantBullet::getSprite()
{
	return sf::Sprite();
}

sf::RectangleShape MutantBullet::getRect()
{
	return sf::RectangleShape();
}
