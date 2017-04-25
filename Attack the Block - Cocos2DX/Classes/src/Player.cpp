#include "Player.h"

USING_NS_CC;

Player::Player()
{
	visibleSize = Director::getInstance()->getVisibleSize();
}

Player::~Player()
{

}

void Player::createPlayer(cocos2d::CCLayerColor *layer)
{
	playerImage = cocos2d::Sprite::create("TetrisScene/Player.png");
	playerImage->setPosition(Point((visibleSize.width / 2), visibleSize.height / 10 - size));
	playerImage->setScale(Director::getInstance()->getContentScaleFactor());
	layer->addChild(playerImage);
}

void Player::movePlayerLeft()
{
	if (playerImage->getPosition().x > ((visibleSize.width / 2) - (size * 5)))
	{
		auto action = MoveBy::create(0.2f, Point(-size, 0));
		playerImage->runAction(action);
	}
}

void Player::movePlayerRight()
{
	if (playerImage->getPosition().x < ((visibleSize.width / 2) + (size * 5)))
	{
		auto action = MoveBy::create(0.2f, Point(size, 0));
		playerImage->runAction(action);
	}
}

void Player::border()
{
	// Player
	if (playerImage->getPosition().x >((visibleSize.width / 2) + (size * 4)))
	{
		playerImage->setPosition(cocos2d::Vec2((visibleSize.width / 2) + (size * 4), playerImage->getPosition().y));
	}

	if (playerImage->getPosition().x < ((visibleSize.width / 2) - (size * 5)))
	{
		playerImage->setPosition(cocos2d::Vec2((visibleSize.width / 2) - (size * 5), playerImage->getPosition().y));
	}

	// Bullet
	if (bulletImage->getPosition().y > visibleSize.height - (visibleSize.height / 2.5f) - size)
	{
		bulletImage->setPosition(playerImage->getPosition());
		fired = false;
	}

	if (fired == false)
	{
		bulletImage->setPosition(playerImage->getPosition());
	}
}

Vec2 Player::getPlayerPosition()
{
	return playerImage->getPosition();
}

void Player::setPlayerPosition(cocos2d::Vec2 pos)
{
	playerImage->setPosition(pos);
}

void Player::createBullet(cocos2d::CCLayerColor *layer)
{
	bulletImage = cocos2d::Sprite::create("TetrisScene/Bullet.png");
	bulletImage->setPosition(Point(playerImage->getPosition()));
	bulletImage->setScale(Director::getInstance()->getContentScaleFactor());

	auto bulletBody = PhysicsBody::createBox(bulletImage->getContentSize());
	bulletBody->setDynamic(false);

	layer->addChild(bulletImage, 1);
}

void Player::moveBullet()
{
	auto action = MoveBy::create(0.05f, Point(0, (bulletImage->getContentSize().height / 2) * Director::getInstance()->getContentScaleFactor()));
	bulletImage->runAction(action);
	colUnder = (bulletImage->getPosition().x - ((visibleSize.width / 2) - (size * 5))) / size;
}

bool Player::getFired()
{
	return fired;
}

void Player::setFired(bool shot)
{
	fired = shot;
}

cocos2d::Vec2 Player::getBulletPosition()
{
	return bulletImage->getPosition();
}

cocos2d::Sprite *Player::getBullet()
{
	return bulletImage;
}