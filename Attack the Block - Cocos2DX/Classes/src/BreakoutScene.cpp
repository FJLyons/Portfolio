#include "BreakoutScene.h"

USING_NS_CC;

Scene* BreakoutScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = BreakoutScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool BreakoutScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)); // Set Colour to white
	visibleSize = Director::getInstance()->getVisibleSize();

	auto menuItem = MenuItemImage::create("BreakoutScene/Pause Up.png", "BreakoutScene/Pause Down.png", CC_CALLBACK_1(BreakoutScene::activatePauseScene, this));
	menuItem->setPosition(Point(visibleSize.width / 12, visibleSize.height - (visibleSize.height / 8)));
	menuItem->setScale(Director::getInstance()->getContentScaleFactor());

	auto menu = Menu::create(menuItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	// Data Load
	ptr = GameData::sharedGameData();
	// Options
	if (ptr->m_defaultTVGlow == true){ addTVGlow(visibleSize); }
	ptr->isTetris = false;

	ptr->m_breakoutEnemyLives = 100;
	ptr->m_breakoutPlayerLives = 100;

	// Art
	createBackdrop(this);

	// Labels
	enemyLivesLabel = LabelTTF::create("E LIFE: ", "OptionsScene/kenvector_future.ttf", 20);
	enemyLivesLabel->setPosition(Point(visibleSize.width / 4, visibleSize.height - (visibleSize.height / 32) - 10));
	enemyLivesLabel->setAnchorPoint(Point(0.0f, 0.0f));
	enemyLivesLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(enemyLivesLabel);

	enemyLivesValueLabel = LabelTTF::create("100", "OptionsScene/kenvector_future.ttf", 20);
	enemyLivesValueLabel->setPosition(Point(visibleSize.width / 3, visibleSize.height - (visibleSize.height / 32) - 10));
	enemyLivesValueLabel->setAnchorPoint(Point(0.0f, 0.0f));
	enemyLivesValueLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(enemyLivesValueLabel);

	playerLivesLabel = LabelTTF::create("P LIFE: ", "OptionsScene/kenvector_future.ttf", 20);
	playerLivesLabel->setPosition(Point(visibleSize.width / 4, (visibleSize.height / 32) - 20));
	playerLivesLabel->setAnchorPoint(Point(0.0f, 0.0f));
	playerLivesLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(playerLivesLabel);

	playerLivesValueLabel = LabelTTF::create("100", "OptionsScene/kenvector_future.ttf", 20);
	playerLivesValueLabel->setPosition(Point(visibleSize.width / 3, (visibleSize.height / 32) - 20));
	playerLivesValueLabel->setAnchorPoint(Point(0.0f, 0.0f));
	playerLivesValueLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(playerLivesValueLabel);

	timeLabel = LabelTTF::create("TIME LEFT: ", "OptionsScene/kenvector_future.ttf", 20);
	timeLabel->setPosition(Point(visibleSize.width - (visibleSize.width / 3), (visibleSize.height / 32) - 20));
	timeLabel->setAnchorPoint(Point(0.0f, 0.0f));
	timeLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(timeLabel);

	timeValueLabel = LabelTTF::create("0", "OptionsScene/kenvector_future.ttf", 20);
	timeValueLabel->setPosition(Point(visibleSize.width - (visibleSize.width / 5), (visibleSize.height / 32) - 20));
	timeValueLabel->setAnchorPoint(Point(0.0f, 0.0f));
	timeValueLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(timeValueLabel);

	damageLabel = LabelTTF::create("DAMAGE: ", "OptionsScene/kenvector_future.ttf", 20);
	damageLabel->setPosition(Point(visibleSize.width - (visibleSize.width / 3), visibleSize.height - (visibleSize.height / 32) - 10));
	damageLabel->setAnchorPoint(Point(0.0f, 0.0f));
	damageLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(damageLabel);

	damageValueLabel = LabelTTF::create("1", "OptionsScene/kenvector_future.ttf", 20);
	damageValueLabel->setPosition(Point(visibleSize.width - (visibleSize.width / 5), visibleSize.height - (visibleSize.height / 32) - 10));
	damageValueLabel->setAnchorPoint(Point(0.0f, 0.0f));
	damageValueLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(damageValueLabel);

	// Import classes
	paddle.createPaddle(this);
	ball.createBall(this);
	createRows(this);

	createEnemy(this);

	// Touch Functionality
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(BreakoutScene::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(BreakoutScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// Update Call
	this->scheduleUpdate();

	return true;
}

void BreakoutScene::update(float dt)
{
	delayHold += dt;
	delayBlocks += dt;

	// Touch
	if (hold == true)
	{
		if (callsHold <= 1)
		{
			if (touched->getLocation().x >= visibleSize.width / 2){
				paddle.movePaddleRight();
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/move.wav");
			}
			else if (touched->getLocation().x < visibleSize.width / 2){
				paddle.movePaddleLeft();
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/move.wav");
			}
		}

		if (delayHold >= 0.1f)
		{
			callsHold = 0;
			delayHold = 0;
		}
		callsHold++;
	}

	// Border
	paddle.border();
	enemyBorder();
	ball.border();
	ball.moveBall();

	// Grid rows
	if (callsBlocks <= 1)
	{
		for (int i = 0; i < firstRow.size(); i++) // First Row
		{
			firstRow.at(i).updateFirstRow();
		}

		for (int i = 0; i < secondRow.size(); i++) // Second Row
		{
			secondRow.at(i).updateSecondRow();
		}

		for (int i = 0; i < thirdRow.size(); i++) // Second Row
		{
			thirdRow.at(i).updateThirdRow();
		}
	}

	// Block Movement
	if (delayBlocks >= 0.2f)
	{
		callsBlocks = 0;
		delayBlocks = 0;
	}
	callsBlocks++;

	// Remove Blocks
	destroy(this);
	ballCollision(this);

	//Time Limit
	timer += 1;

	if (timer >= 60)
	{
		timer -= 60;
		seconds += 1;
	}

	if (seconds >= ptr->m_tetrisTimeLimit)
	{
		ptr->m_breakoutScore = ptr->m_breakoutPlayerLives - ptr->m_breakoutEnemyLives;
		gameOver();
	}

	else if (ptr->m_breakoutPlayerLives <= 0)
	{
		ptr->m_breakoutScore = ptr->m_breakoutPlayerLives - ptr->m_breakoutEnemyLives;
		gameOver();
	}

	else if (ptr->m_breakoutEnemyLives <= 0)
	{
		ptr->m_breakoutScore = ptr->m_breakoutPlayerLives - ptr->m_breakoutEnemyLives;
		gameOver();
	}

	//AI
	AI();


	std::string timeLeft = std::to_string(120 - seconds);
	timeValueLabel->setString(timeLeft);

	//// Score
	std::string playerLives = std::to_string(ptr->m_breakoutPlayerLives);
	playerLivesValueLabel->setString(playerLives);

	std::string enemyLives = std::to_string(ptr->m_breakoutEnemyLives);
	enemyLivesValueLabel->setString(enemyLives);

	std::string damage = std::to_string(ptr->m_damageLives);
	damageValueLabel->setString(damage);
}

bool BreakoutScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event)
{
	CCLOG("Began x = %f, y = %f", touch->getLocation().x, touch->getLocation().y);

	hold = true;
	touched = touch;
	delayHold = 0;
	callsHold = 0;

	return true;
}

void BreakoutScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event * event)
{
	CCLOG("Ended x = %f, y = %f", touch->getLocation().x, touch->getLocation().y);

	hold = false;
}


void BreakoutScene::menuCloseCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void BreakoutScene::activatePauseScene(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	auto scene = PauseMenuScene::createScene();
	Director::getInstance()->pushScene(scene);
}

void BreakoutScene::gameOver()
{
	auto scene = GameOver::createScene();
	Director::getInstance()->replaceScene(scene);
}

void BreakoutScene::addTVGlow(cocos2d::Size const & visibleSize)
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	// TV Effect
	auto TV = cocos2d::Sprite::create(ptr->m_TVGlow);
	TV->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	TV->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(TV);
}
void BreakoutScene::createBackdrop(cocos2d::CCLayerColor *layer)
{
	// Art
	auto backdrop = cocos2d::Sprite::create("BreakoutScene/Backdrop.png");
	backdrop->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	backdrop->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(backdrop);
}

void BreakoutScene::createRows(cocos2d::CCLayerColor *layer)
{
	firstRow.resize(16);
	for (int i = 0; i < firstRow.size(); i++) // First Row
	{
		createFirstBlocks(layer, i);
	}

	secondRow.resize(16);
	for (int i = 0; i < secondRow.size(); i++) // Second Row
	{
		createSecondBlocks(layer, i);
	}

	thirdRow.resize(16);
	for (int i = 0; i < thirdRow.size(); i++) // Second Row
	{
		createThirdBlocks(layer, i);
	}
}

void BreakoutScene::createFirstBlocks(cocos2d::CCLayerColor *layer, int i)
{
	auto point = Vec2(visibleSize.width / 10 + ((size * 6) * i), (visibleSize.height / 2) + (size * 4));

	if (newBlockFirst == true)
	{
		point = Vec2(firstRow[i - 1].getBlockPosition().x + (size * 4), (visibleSize.height / 2) + (size * 4));
		newBlockFirst = false;
	}

	firstRow[i].createBreakoutBlock(layer, point);
	firstRow[i].setType(-1);
}

void BreakoutScene::createSecondBlocks(cocos2d::CCLayerColor *layer, int i)
{
	auto point = Vec2(visibleSize.width - (visibleSize.width / 10) - ((size * 6) * i), (visibleSize.height / 2));

	if (newBlockSecond == true)
	{
		point = Vec2(secondRow[i - 1].getBlockPosition().x - (size * 4), (visibleSize.height / 2));
		newBlockSecond = false;
	}
	secondRow[i].createBreakoutBlock(layer, point);
	secondRow[i].setType(-1);
}

void BreakoutScene::createThirdBlocks(cocos2d::CCLayerColor *layer, int i)
{
	auto point = Vec2(visibleSize.width / 10 + ((size * 6) * i), (visibleSize.height / 2) - (size * 4));

	if (newBlockThird == true)
	{
		point = Vec2(thirdRow[i - 1].getBlockPosition().x + (size * 4), (visibleSize.height / 2) - (size * 4));
		newBlockThird = false;
	}
	thirdRow[i].createBreakoutBlock(layer, point);
	thirdRow[i].setType(-1);
}

void BreakoutScene::destroy(cocos2d::CCLayerColor *layer)
{
	for (int i = 0; i < firstRow.size(); i++) // First Row
	{
		if (firstRow[i].getBlockPosition().x < -size)
		{
			firstRow[i].removeImage(layer);
			firstRow.erase(firstRow.begin() + i);
			firstRow.resize(10);
			createFirstBlocks(layer, 9);
			newBlockFirst = true;
		}
	}

	for (int i = 0; i < secondRow.size(); i++) // First Row
	{
		if (secondRow[i].getBlockPosition().x > visibleSize.width + size)
		{
			secondRow[i].removeImage(layer);
			secondRow.erase(secondRow.begin() + i);
			secondRow.resize(10);
			createSecondBlocks(layer, 9);
			newBlockSecond = true;
		}
	}

	for (int i = 0; i < thirdRow.size(); i++) // First Row
	{
		if (thirdRow[i].getBlockPosition().x < -size)
		{
			thirdRow[i].removeImage(layer);
			thirdRow.erase(thirdRow.begin() + i);
			thirdRow.resize(10);
			createThirdBlocks(layer, 9);
			newBlockThird = true;
		}
	}
}


void BreakoutScene::createEnemy(cocos2d::CCLayerColor *layer)
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	// load the Sprite Sheet
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile(ptr->m_enemyPlistFile);
	int frames = 5;

	for (int i = 0; i < frames; i++)
	{
		// Get a SpriteFrame using a name from the spritesheet .plist file.
		enemyPaddleFrames.pushBack(spritecache->getSpriteFrameByName("Enemy Paddle" + std::to_string(i) + ".png"));
	}

	// Create the animation out of the frames.
	Animation* animation = Animation::createWithSpriteFrames(enemyPaddleFrames, 0.05f);
	Animate* animate = Animate::create(animation);

	enemyImage = Sprite::createWithSpriteFrame(enemyPaddleFrames.at(0));
	enemyImage->setScale(Director::getInstance()->getContentScaleFactor());
	enemyImage->setRotation(180);
	enemyImage->setPosition(Point((visibleSize.width / 2), visibleSize.height - (visibleSize.height / 10)));


	// Run and repeat the animation.
	enemyImage->runAction(RepeatForever::create(animate));

	layer->addChild(enemyImage);
	enemyPaddleImage.pushBack(enemyImage);
}

void BreakoutScene::AI()
{
	auto left = ((visibleSize.width / 2) - (32 * 5.5f));
	auto right = ((visibleSize.width / 2) + (32 * 5.5f));

	// Move Left
	if (ball.getBallPosition().x < enemyImage->getPosition().x - (enemyImage->getContentSize().width / 2 * Director::getInstance()->getContentScaleFactor()))
	{
		if (enemyImage->getPosition().x > left)
		{
			enemyImage->setPosition(enemyImage->getPosition().x - 3, enemyImage->getPosition().y);
		}
	}
	
	// Move Right
	else if (ball.getBallPosition().x >= enemyImage->getPosition().x + (enemyImage->getContentSize().width / 2 * Director::getInstance()->getContentScaleFactor()))
	{
		if (enemyImage->getPosition().x < right)
		{
			enemyImage->setPosition(enemyImage->getPosition().x + 3, enemyImage->getPosition().y);
		}
	}
}

void BreakoutScene::enemyBorder()
{
	auto left = ((visibleSize.width / 2) - (32 * 5.5f));
	auto right = ((visibleSize.width / 2) + (32 * 5.5f));
	// Border
	// Left
	if (enemyImage->getPosition().x < left)
	{
		enemyImage->setPosition(cocos2d::Vec2(left, enemyImage->getPosition().y));
	}

	// Right
	if (enemyImage->getPosition().x > right)
	{
		enemyImage->setPosition(cocos2d::Vec2(right, enemyImage->getPosition().y));
	}
}

void BreakoutScene::ballCollision(cocos2d::CCLayerColor *layer)
{
	Rect playerBody = paddle.getPaddle()->getBoundingBox();
	Rect enemyBody = enemyImage->getBoundingBox();
	Rect ballBody = ball.getBall()->getBoundingBox();

	auto errorResponse = ball.getBall()->getContentSize().width / 2;
	auto errorArea = errorResponse / 2;

	if (ballBody.intersectsRect(playerBody))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/ball.wav");
		ball.setBallVelocity(ball.getBallVelocity() * 1.1f);

		if (ballBody.getMaxX() >= playerBody.getMinX() - 1 && ballBody.getMaxX() <= playerBody.getMinX() + errorArea)
		{
			auto correction = Vec2(ball.getBallPosition().x - errorResponse, ball.getBallPosition().y);
			ball.setBallPosition(correction);
			ball.velocity.x = -ball.velocity.x;
		}

		else if (ballBody.getMinX() <= playerBody.getMaxX() + 1 && ballBody.getMinX() >= playerBody.getMaxX() - errorArea)
		{
			auto correction = Vec2(ball.getBallPosition().x + errorResponse, ball.getBallPosition().y);
			ball.setBallPosition(correction);
			ball.velocity.x = -ball.velocity.x;
		}

		else
		{
			ball.velocity.y = -ball.velocity.y;
		}
	}

	else if (ballBody.intersectsRect(enemyBody))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/ball.wav");
		ball.setBallVelocity(ball.getBallVelocity() * 1.1f);

		if (ballBody.getMaxX() >= enemyBody.getMinX() - 1 && ballBody.getMaxX() <= enemyBody.getMinX() + errorArea)
		{
			auto correction = Vec2(ball.getBallPosition().x - errorResponse, ball.getBallPosition().y);
			ball.setBallPosition(correction);
			ball.velocity.x = -ball.velocity.x;
		}

		else if (ballBody.getMinX() <= enemyBody.getMaxX() + 1 && ballBody.getMinX() >= enemyBody.getMaxX() + errorArea)
		{
			auto correction = Vec2(ball.getBallPosition().x + errorResponse, ball.getBallPosition().y);
			ball.setBallPosition(correction);
			ball.velocity.x = -ball.velocity.x;
		}

		else
		{
			ball.velocity.y = -ball.velocity.y;
		}
	}

	// blocks
	for (int i = 0; i < firstRow.size(); i++) // First Row
	{
		Rect blockBody(firstRow[i].getBlock()->getBoundingBox());

		if (ballBody.intersectsRect(blockBody))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/ball.wav");
			ptr->m_damageLives += 1;

			firstRow[i].removeImage(layer);
			firstRow.erase(firstRow.begin() + i);
			firstRow.resize(10);
			createFirstBlocks(layer, 9);
			newBlockFirst = true;

			if (ballBody.getMaxX() >= blockBody.getMinX() - 2 && blockBody.getMaxX() <= blockBody.getMinX() + errorArea)
			{
				auto correction = Vec2(ball.getBallPosition().x - errorResponse, ball.getBallPosition().y);
				ball.setBallPosition(correction);
				ball.velocity.x = -ball.velocity.x;
			}

			else if (ballBody.getMinX() <= blockBody.getMaxX() + 2 && blockBody.getMinX() >= blockBody.getMaxX() - errorArea)
			{
				auto correction = Vec2(ball.getBallPosition().x + errorResponse, ball.getBallPosition().y);
				ball.setBallPosition(correction);
				ball.velocity.x = -ball.velocity.x;
			}

			else
			{
				ball.velocity.y = -ball.velocity.y;
			}
		}
	}

	for (int i = 0; i < secondRow.size(); i++) // Second Row
	{
		Rect blockBody(secondRow[i].getBlock()->getBoundingBox());

		if (ballBody.intersectsRect(blockBody))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/ball.wav");
			ptr->m_damageLives += 1;

			secondRow[i].removeImage(layer);
			secondRow.erase(secondRow.begin() + i);
			secondRow.resize(10);
			createSecondBlocks(layer, 9);
			newBlockSecond = true;

			if (ballBody.getMaxX() >= blockBody.getMinX() - 2 && blockBody.getMaxX() <= blockBody.getMinX() + errorArea)
			{
				auto correction = Vec2(ball.getBallPosition().x - errorResponse, ball.getBallPosition().y);
				ball.setBallPosition(correction);
				ball.velocity.x = -ball.velocity.x;
			}

			else if (ballBody.getMinX() <= blockBody.getMaxX() + 2 && blockBody.getMinX() >= blockBody.getMaxX() - errorArea)
			{
				auto correction = Vec2(ball.getBallPosition().x + errorResponse, ball.getBallPosition().y);
				ball.setBallPosition(correction);
				ball.velocity.x = -ball.velocity.x;
			}

			else
			{
				ball.velocity.y = -ball.velocity.y;
			}
		}
	}

	for (int i = 0; i < thirdRow.size(); i++) // Third Row
	{
		Rect blockBody(thirdRow[i].getBlock()->getBoundingBox());

		if (ballBody.intersectsRect(blockBody))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/ball.wav");
			ptr->m_damageLives += 1;
			ball.setBallVelocity(ball.getBallVelocity() * 1.1f);

			thirdRow[i].removeImage(layer);
			thirdRow.erase(thirdRow.begin() + i);
			thirdRow.resize(10);
			createThirdBlocks(layer, 9);
			newBlockThird = true;

			if (ballBody.getMaxX() >= blockBody.getMinX() - 2 && blockBody.getMaxX() <= blockBody.getMinX() + errorArea)
			{
				auto correction = Vec2(ball.getBallPosition().x - errorResponse, ball.getBallPosition().y);
				ball.setBallPosition(correction);
				ball.velocity.x = -ball.velocity.x;
			}

			else if (ballBody.getMinX() <= blockBody.getMaxX() + 2 && blockBody.getMinX() >= blockBody.getMaxX() - errorArea)
			{
				auto correction = Vec2(ball.getBallPosition().x + errorResponse, ball.getBallPosition().y);
				ball.setBallPosition(correction);
				ball.velocity.x = -ball.velocity.x;
			}

			else
			{
				ball.velocity.y = -ball.velocity.y;
			}
		}
	}
}


//void BreakoutScene::ballCollision(cocos2d::CCLayerColor *layer)
//{
//	Rect playerBody = paddle.getPaddle()->getBoundingBox();
//	Rect enemyBody = enemyImage->getBoundingBox();
//	Rect ballBody = ball.getBall()->getBoundingBox();
//
//	if (ballBody.intersectsRect(playerBody))
//	{
//		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
//		ball.setBallVelocity(ball.getBallVelocity() * 1.1f);
//
//		if (ballBody.getMaxX() >= playerBody.getMinX() - 1 && ballBody.getMaxX() <= playerBody.getMinX() + 2)
//		{
//			ball.velocity.x = -ball.velocity.x;
//		}
//
//		else if (ballBody.getMinX() >= playerBody.getMaxX() - 1 && ballBody.getMinX() <= playerBody.getMaxX() + 2)
//		{
//			ball.velocity.x = -ball.velocity.x;
//		}
//
//		else
//		{
//			ball.velocity.y = -ball.velocity.y;
//		}
//	}
//
//	else if (ballBody.intersectsRect(enemyBody))
//	{
//		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
//		ball.setBallVelocity(ball.getBallVelocity() * 1.1f);
//
//		if (ballBody.getMaxX() >= enemyBody.getMinX() - 1 && ballBody.getMaxX() <= enemyBody.getMinX() + 2)
//		{
//			ball.velocity.x = -ball.velocity.x;
//		}
//
//		else if (ballBody.getMinX() >= enemyBody.getMaxX() - 1 && ballBody.getMinX() <= enemyBody.getMaxX() + 2)
//		{
//			ball.velocity.x = -ball.velocity.x;
//		}
//
//		else
//		{
//			ball.velocity.y = -ball.velocity.y;
//		}
//	}
//
//	// blocks
//	for (int i = 0; i < firstRow.size(); i++) // First Row
//	{
//		Rect blockBody(firstRow[i].getBlock()->getBoundingBox());
//
//		if (ballBody.intersectsRect(blockBody))
//		{
//			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
//			ptr->m_damageLives += 1;
//			ball.setBallVelocity(ball.getBallVelocity() * 1.1f);
//
//			firstRow[i].removeImage(layer);
//			firstRow.erase(firstRow.begin() + i);
//			firstRow.resize(10);
//			createFirstBlocks(layer, 9);
//			newBlockFirst = true;
//
//			if (ballBody.getMaxX() >= blockBody.getMinX() - 2 && blockBody.getMaxX() <= blockBody.getMinX() + 4)
//			{
//				ball.velocity.x = -ball.velocity.x;
//			}
//
//			else if (ballBody.getMinX() >= blockBody.getMaxX() - 2 && blockBody.getMinX() <= blockBody.getMaxX() + 4)
//			{
//				ball.velocity.x = -ball.velocity.x;
//			}
//
//			else
//			{
//				ball.velocity.y = -ball.velocity.y;
//			}
//		}
//	}
//
//	for (int i = 0; i < secondRow.size(); i++) // Second Row
//	{
//		Rect blockBody(secondRow[i].getBlock()->getBoundingBox());
//
//		if (ballBody.intersectsRect(blockBody))
//		{
//			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
//			ptr->m_damageLives += 1;
//			ball.setBallVelocity(ball.getBallVelocity() * 1.1f);
//
//			secondRow[i].removeImage(layer);
//			secondRow.erase(secondRow.begin() + i);
//			secondRow.resize(10);
//			createSecondBlocks(layer, 9);
//			newBlockSecond = true;
//
//			if (ballBody.getMaxX() >= blockBody.getMinX() - 2 && blockBody.getMaxX() <= blockBody.getMinX() + 4)
//			{
//				ball.velocity.x = -ball.velocity.x;
//			}
//
//			else if (ballBody.getMinX() >= blockBody.getMaxX() - 2 && blockBody.getMinX() <= blockBody.getMaxX() + 4)
//			{
//				ball.velocity.x = -ball.velocity.x;
//			}
//
//			else
//			{
//				ball.velocity.y = -ball.velocity.y;
//			}
//		}
//	}
//
//	for (int i = 0; i < thirdRow.size(); i++) // Third Row
//	{
//		Rect blockBody(thirdRow[i].getBlock()->getBoundingBox());
//
//		if (ballBody.intersectsRect(blockBody))
//		{
//			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
//			ptr->m_damageLives += 1;
//			ball.setBallVelocity(ball.getBallVelocity() * 1.1f);
//
//			thirdRow[i].removeImage(layer);
//			thirdRow.erase(thirdRow.begin() + i);
//			thirdRow.resize(10);
//			createThirdBlocks(layer, 9);
//			newBlockThird = true;
//
//			if (ballBody.getMaxX() >= blockBody.getMinX() - 2 && blockBody.getMaxX() <= blockBody.getMinX() + 4)
//			{
//				ball.velocity.x = -ball.velocity.x;
//			}
//
//			else if (ballBody.getMinX() >= blockBody.getMaxX() - 2 && blockBody.getMinX() <= blockBody.getMaxX() + 4)
//			{
//				ball.velocity.x = -ball.velocity.x;
//			}
//
//			else
//			{
//				ball.velocity.y = -ball.velocity.y;
//			}
//		}
//	}
//}

