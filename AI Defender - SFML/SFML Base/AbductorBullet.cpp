#include "AbductorBullet.h"

AbductorBullet::AbductorBullet()
{

}


AbductorBullet::AbductorBullet(sf::Vector2f pos, sf::Vector2f playerpos)
{
	mPositon = pos;
	mTexture = ResourceLoader::instance()->getenemyBulletTexture();
	mSprite.setTexture(mTexture);
	mSprite.setPosition(mPositon);
	mSprite.setOrigin(sf::Vector2f(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2));

	mDirection = playerpos - mPositon;
	

	collisionRect.setOrigin(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
	collisionRect.setSize(sf::Vector2f(mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height));
	collisionRect.setOutlineColor(sf::Color::Red);
	collisionRect.setFillColor(sf::Color::Transparent);
	collisionRect.setOutlineThickness(1);
	collisionRect.setPosition(mPositon);

	alive = true;

	mVelocity = sf::Vector2f(20, 20);

}

bool AbductorBullet::Update()
{
	ttl -= deltaClock.getElapsedTime().asSeconds();
	sf::Time dt = deltaClock.restart();


	if (ttl <= 0)
	{
		alive = false;
	}
	
	mDirection = CollisionManager::instance()->NormaliseVector(mDirection);



	mPositon.x += mVelocity.x * mDirection.x;
	mPositon.y += mVelocity.y * mDirection.y;

	
	mSprite.setPosition(mPositon);
	collisionRect.setPosition(mPositon);
	return alive;
}
sf::Sprite AbductorBullet::getSprite()
{
	return mSprite;
}

void AbductorBullet::Draw(sf::RenderWindow &window)
{
	window.draw(mSprite);
	if (myGlobalOptions->drawCollisionBox)
	{
		window.draw(collisionRect);
	}
}


sf::RectangleShape AbductorBullet::getRect()
{
	return collisionRect;
}
