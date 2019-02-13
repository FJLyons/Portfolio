#include "Human.h"

Human::Human()
{

}

Human::Human(sf::Vertex* points)
{
	targetPoints = points;
	currentPoint = currentPoint = HALF_SCREEN_WIDTH_POINTS + rand() % MAP_WIDTH_POINTS + 1;
	
	targeted = false;
	falling = false;
	mTexture = ResourceLoader::instance()->gethumanTexture();
	mindicatorTexture = ResourceLoader::instance()->getindicatorTexture();
	mPositon = points[currentPoint - 1].position;
	velocity = sf::Vector2f(1, 1);
	abducted = false;
	mTexture.setSmooth(true);

	mSprite.setTexture(mTexture);
	mSprite.setPosition(mPositon);

	mindicatorSprite.setTexture(mindicatorTexture);
	mindicatorSprite.setOrigin(sf::Vector2f(mSprite.getLocalBounds().width + 100, mSprite.getLocalBounds().height / 2));

	mSprite.setOrigin(sf::Vector2f(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2));

	collisionRect.setOrigin(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height);
	collisionRect.setSize(sf::Vector2f(mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height));
	collisionRect.setOutlineColor(sf::Color::Red);
	collisionRect.setFillColor(sf::Color::Transparent);
	collisionRect.setOutlineThickness(2);
	collisionRect.setPosition(mPositon);

}

void Human::Update(sf::Vector2f playerPos)
{
	if (abducted == false && falling == false)
	{
		sf::Vector2f Direction = targetPoints[currentPoint].position - mPositon;
		float length = sqrt((Direction.x * Direction.x) + (Direction.y * Direction.y));
		Direction = Direction / length;

		mPositon += sf::Vector2f(Direction.x * velocity.x, Direction.y * velocity.y);


		if (mPositon.x >= targetPoints[currentPoint].position.x)
		{
			currentPoint++;
		}

		// Teleport right
		if (currentPoint > FULL_WIDTH_POINTS)
		{
			mPositon = targetPoints[HALF_SCREEN_WIDTH_POINTS].position;
			currentPoint = HALF_SCREEN_WIDTH_POINTS + 1;
		}

		// teleport left
		if (currentPoint < 0)
		{
			mPositon = targetPoints[FULL_WIDTH_POINTS - HALF_SCREEN_WIDTH_POINTS].position;
			currentPoint = FULL_WIDTH_POINTS - HALF_SCREEN_WIDTH_POINTS - 1;
		}
		previousYpos = mPositon.y;
	}
	else if (abducted == true && falling == false)
	{
		
		angle = atan2(playerPos.y - mPositon.y, playerPos.x - mPositon.x);
		angle = angle * (180 / 3.14);

		mindicatorSprite.setPosition(playerPos );
		mindicatorSprite.setRotation(angle);
		velocity = sf::Vector2f(0, -2);
		mPositon += velocity;
	}
	else if (abducted == false && falling == true)
	{
  		velocity = sf::Vector2f(0, 10);
		mPositon += velocity;
		if (mPositon.y >= previousYpos)
		{
			falling = false;
			targeted = false;
			velocity = sf::Vector2f(1, 1);
		}
	}
	mSprite.setPosition(sf::Vector2f(mPositon.x, mPositon.y - mSprite.getLocalBounds().height / 2));
	collisionRect.setPosition(mPositon);
}

void Human::Draw(sf::RenderWindow &window, bool mini)
{
	if (mini == false)
	{
		mSprite.setScale(sf::Vector2f(1, 1));
		window.draw(mSprite);
	}

	if (mini == true)
	{
		mSprite.setScale(sf::Vector2f(4, 4));
		window.draw(mSprite);
	}

	if (abducted == true && falling == false)
	{
		window.draw(mindicatorSprite);
	}

}

bool Human::getTargeted()
{
	return targeted;
}

void Human::setTargeted(bool target)
{
	targeted = target;
}

void Human::setAbducted(bool abduct)
{
	abducted = abduct;
}

void Human::setFalling(bool fall)
{
	falling = fall;
}

bool Human::getFalling()
{
	return falling;
}



sf::RectangleShape Human::getRect()
{
	return collisionRect;
}

sf::Vector2f Human::getPosition()
{
	return mPositon - sf::Vector2f(0, 30);
}

int Human::getPoint()
{
	return currentPoint - 1;
}



void Human::setPoint(int index)
{
	mPositon = targetPoints[index].position;
	currentPoint = index + 1;
}