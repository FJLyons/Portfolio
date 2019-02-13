#include "InterceptorMissile.h"

InterceptorMissile::InterceptorMissile(sf::Vector2f pos)
{

	ttl = 10;
	mPositon = pos;
	mTexture = ResourceLoader::instance()->getinterceptorTexture();
	mSprite.setTexture(mTexture);
	mSprite.setPosition(mPositon);
	mSprite.setOrigin(sf::Vector2f(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2));
	mVelocity = sf::Vector2f(5, 5);
	alive = true;

	collisionRect.setOrigin(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
	collisionRect.setSize(sf::Vector2f(mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height));
	collisionRect.setOutlineColor(sf::Color::Red);
	collisionRect.setFillColor(sf::Color::Transparent);
	collisionRect.setOutlineThickness(2);
	collisionRect.setPosition(mPositon);
}

bool InterceptorMissile::Update(sf::RectangleShape  player)
{
	ttl -= deltaClock.getElapsedTime().asSeconds();
	sf::Time dt = deltaClock.restart();


	if (ttl <= 0)
	{
		alive = false;
	}

	

	sf::Vector2f Direction = player.getPosition() - mPositon;
	float length = sqrt((Direction.x * Direction.x) + (Direction.y * Direction.y));
	Direction = Direction / length;
	mPositon += sf::Vector2f(Direction.x * mVelocity.x, Direction.y * mVelocity.y);
	mSprite.setPosition(mPositon);

	float  angle = atan2(player.getPosition().y - mPositon.y, player.getPosition().x - mPositon.x);
	angle = angle * (180 / 3.14);
	mSprite.setRotation(angle);
	collisionRect.setPosition(mPositon);

	if (CollisionManager::instance()->RectangleCollision(collisionRect,player) == true)
	{
		alive = false;
	}
	return alive;
}

void InterceptorMissile::Draw(sf::RenderWindow &window)
{
	window.draw(mSprite);
	if (myGlobalOptions->drawCollisionBox)
	{
		window.draw(collisionRect);
	}
}
