#include "Paddle.h"

USING_NS_CC;

Paddle::Paddle()
{
	visibleSize = Director::getInstance()->getVisibleSize();
}

Paddle::~Paddle()
{

}

//void Paddle::createPaddle(cocos2d::CCLayerColor *layer)
//{
//	playerPaddleImage = cocos2d::Sprite::create("BreakoutScene/Player.png");
//	playerPaddleImage->setPosition(Point((visibleSize.width / 2),
//		visibleSize.height / 10 + size));
//
//	playerPaddleImage->setScale(Director::getInstance()->getContentScaleFactor());
//
//	layer->addChild(playerPaddleImage);
//}

void Paddle::movePaddleLeft()
{
	auto left = ((visibleSize.width / 2) - (size * 5.5f));
	if (playerImage->getPosition().x > left)
	{
		auto action = MoveBy::create(0.1f, Point(-size, 0));
		playerImage->runAction(action);
	}
}

void Paddle::movePaddleRight()
{
	auto right = ((visibleSize.width / 2) + (size * 5.5f));
	if (playerImage->getPosition().x < right)
	{
		auto action = MoveBy::create(0.1f, Point(size, 0));
		playerImage->runAction(action);
	}
}

void Paddle::border()
{
	auto left = ((visibleSize.width / 2) - (size * 5.5f));
	auto right = ((visibleSize.width / 2) + (size * 5.5f));

	// Left
	if (playerImage->getPosition().x < left)
	{
		playerImage->setPosition(cocos2d::Vec2(left, playerImage->getPosition().y));
	}

	// Right
	if (playerImage->getPosition().x > right)
	{
		playerImage->setPosition(cocos2d::Vec2(right, playerImage->getPosition().y));
	}
}

Vec2 Paddle::getPaddlePosition()
{
	return playerImage->getPosition();
}

void Paddle::setPaddlePosition(cocos2d::Vec2 pos)
{
	playerImage->setPosition(pos);
}

cocos2d::Sprite *Paddle::getPaddle()
{
	return playerImage;
}


void Paddle::createPaddle(cocos2d::CCLayerColor *layer)
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	// load the Sprite Sheet
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile(ptr->m_paddlePlistFile);
	int frames = 5;

	for (int i = 0; i < frames; i++)
	{
		// Get a SpriteFrame using a name from the spritesheet .plist file.
		playerPaddleFrames.pushBack(spritecache->getSpriteFrameByName("Paddle" + std::to_string(i) + ".png"));
	}

	// Create the animation out of the frames.
	Animation* animation = Animation::createWithSpriteFrames(playerPaddleFrames, 0.05f);
	Animate* animate = Animate::create(animation);

	playerImage = Sprite::createWithSpriteFrame(playerPaddleFrames.at(0));
	playerImage->setScale(Director::getInstance()->getContentScaleFactor());
	playerImage->setPosition(Point((visibleSize.width / 2), visibleSize.height / 10));

	// Run and repeat the animation.
	playerImage->runAction(RepeatForever::create(animate));

	layer->addChild(playerImage);
	playerPaddleImage.pushBack(playerImage);
}