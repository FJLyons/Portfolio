#include "Ball.h"

USING_NS_CC;

Ball::Ball()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	srand(time(0));
}

Ball::~Ball()
{

}

void Ball::createBall(cocos2d::CCLayerColor *layer)
{
	ballImage = cocos2d::Sprite::create("BreakoutScene/Ball.png");
	ballImage->setPosition(Point((visibleSize.width / 2), visibleSize.height / 2));
	ballImage->setScale(Director::getInstance()->getContentScaleFactor() * 0.65f);
	layer->addChild(ballImage);

	srand(time(0));
	velocity.x = rand() % 3 - 2; 

	srand(time(0));
	velocity.y = rand() % 3 - 2;

	if (velocity.x == 0 || velocity.y == 0)
	{
		velocity.x += 1;
		velocity.y += 1;
	}

	velocity *= 2;
}

void Ball::moveBall()
{
	ballImage->setPosition(ballImage->getPosition() + velocity);
}

void Ball::border()
{
	ptr = GameData::sharedGameData();

	auto left = visibleSize.width - (visibleSize.width / 3);
	auto right = visibleSize.width / 3;

	auto top = visibleSize.height - (visibleSize.height / 16);
	auto bottom = visibleSize.height / 16;

	// Left
	if (ballImage->getPosition().x > left)
	{
		//BallImage->setPosition(cocos2d::Vec2(left, BallImage->getPosition().y));
		velocity.x = -velocity.x;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/bounce.wav");
	}

	// Right
	if (ballImage->getPosition().x < right)
	{
		//BallImage->setPosition(cocos2d::Vec2(right, BallImage->getPosition().y));
		velocity.x = -velocity.x;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/bounce.wav");
	}

	// Top
	if (ballImage->getPosition().y > top)
	{
		//BallImage->setPosition(cocos2d::Vec2(BallImage->getPosition().x, top));
		velocity.y = -velocity.y;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/bounce.wav");

		ptr->m_breakoutEnemyLives -= ptr->m_damageLives;
		ptr->m_damageLives = 1;
		ballImage->setPosition(Point((visibleSize.width / 2), visibleSize.height / 2));
		velocity = Vec2(0, 0);

		srand(time(0));
		velocity.x = rand() % 3 - 2;

		srand(time(0));
		velocity.y = rand() % 3 - 2;

		if (velocity.x == 0 || velocity.y == 0)
		{
			velocity.x += 1;
			velocity.y += 1;
		}

		velocity *= 2;
	}

	// Bottom
	if (ballImage->getPosition().y < bottom)
	{
		//BallImage->setPosition(cocos2d::Vec2(BallImage->getPosition().x, bottom));
		velocity.y = -velocity.y;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/bounce.wav");

		ptr->m_breakoutPlayerLives -= ptr->m_damageLives;
		ptr->m_damageLives = 1;
		ballImage->setPosition(Point((visibleSize.width / 2), visibleSize.height / 2));
		velocity = Vec2(0, 0);

		srand(time(0));
		velocity.x = rand() % 3 - 2;

		srand(time(0));
		velocity.y = rand() % 3 - 2;

		if (velocity.x == 0 || velocity.y == 0)
		{
			velocity.x += 1;
			velocity.y += 1;
		}

		velocity *= 2;
	}
}

Vec2 Ball::getBallPosition()
{
	return ballImage->getPosition();
}

void Ball::setBallPosition(cocos2d::Vec2 pos)
{
	ballImage->setPosition(pos);
}

Vec2 Ball::getBallVelocity()
{
	return velocity;
}

void Ball::setBallVelocity(cocos2d::Vec2 vel)
{
	velocity = vel;
}

cocos2d::Sprite *Ball::getBall()
{
	return ballImage;
}