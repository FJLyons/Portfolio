#include "TetrisScene.h"

USING_NS_CC;

Scene* TetrisScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = TetrisScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool TetrisScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)); // Set Colour to white

	visibleSize = Director::getInstance()->getVisibleSize();

	auto menuItem = MenuItemImage::create("TetrisScene/Pause Up.png", "TetrisScene/Pause Down.png", CC_CALLBACK_1(TetrisScene::activatePauseScene, this));
	menuItem->setPosition(Point(visibleSize.width / 12, visibleSize.height - (visibleSize.height / 8)));
	menuItem->setScale(Director::getInstance()->getContentScaleFactor());

	auto menu = Menu::create(menuItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	// Data Load
	ptr = GameData::sharedGameData();
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("Audio/beckground.ogg");

	// Options
	if (ptr->m_defaultTVGlow == true) { addTVGlow(visibleSize); }
	if (ptr->m_defaultTutorial == true) { addTutorial(visibleSize); }
	ptr->isTetris = true;
	ptr->m_punish = false;

	punishLabel = LabelTTF::create("FROZEN FOR: ", "OptionsScene/kenvector_future.ttf", 20);
	punishLabel->setPosition(Point(visibleSize.width / 12, (visibleSize.height / 32) - 20));
	punishLabel->setAnchorPoint(Point(0.0f, 0.0f));
	punishLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(punishLabel);

	punishTimerLabel = LabelTTF::create("0 / 10", "OptionsScene/kenvector_future.ttf", 20);
	punishTimerLabel->setPosition(Point(visibleSize.width / 4, (visibleSize.height / 32) - 20));
	punishTimerLabel->setAnchorPoint(Point(0.0f, 0.0f));
	punishTimerLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(punishTimerLabel);

	scoreLabel = LabelTTF::create("SCORE: ", "OptionsScene/kenvector_future.ttf", 20);
	scoreLabel->setPosition(Point(visibleSize.width / 2.4, (visibleSize.height / 32) - 20));
	scoreLabel->setAnchorPoint(Point(0.0f, 0.0f));
	scoreLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(scoreLabel);

	scoreValueLabel = LabelTTF::create("0", "OptionsScene/kenvector_future.ttf", 20);
	scoreValueLabel->setPosition(Point(visibleSize.width / 1.9, (visibleSize.height / 32) - 20));
	scoreValueLabel->setAnchorPoint(Point(0.0f, 0.0f));
	scoreValueLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(scoreValueLabel);

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

	// Import Classes
	grid.createGrid(this);
	grid.createRows(this);
	player.createPlayer(this);
	player.createBullet(this);

	// Touch Functionality
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(TetrisScene::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(TetrisScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// Update Call
	this->scheduleUpdate();

	return true;
}

void TetrisScene::update(float dt)
{
	delayHold += dt;
	delayBlocks += dt;
	delayFired += dt;

	if (ptr->m_punish == false)
	{
		// Touch
		if (hold == true)
		{
			if (callsHold <= 1)
			{
				if (touched->getLocation().x >= player.getPlayerPosition().x + 14 && touched->getLocation().y < visibleSize.height / 2){
					player.movePlayerRight();
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/move.wav");
				}
				else if (touched->getLocation().x < player.getPlayerPosition().x - 14 && touched->getLocation().y < visibleSize.height / 2){
					player.movePlayerLeft();
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/move.wav");
				}
			}

			if (delayHold >= 0.2f)
			{
				callsHold = 0;
				delayHold = 0;
			}
			callsHold++;
		}

		// Player Bullet
		if (player.getFired() == true)
		{
			if (callsFired <= 1)
			{
				player.moveBullet();
			}

			if (delayFired >= 0.025f)
			{
				callsFired = 0;
				delayFired = 0;
			}
			callsFired++;
		}
	}

	// Always Update //
	// Grid rows
	if (callsBlocks <= 1)
	{
		grid.update();
	}

	// Block Movement
	if (delayBlocks >= 0.2f)
	{
		callsBlocks = 0;
		delayBlocks = 0;
	}
	callsBlocks++;

	// Remove Blocks
	grid.destroy(this);
	grid.bulletCollision(&player, this);

	// Player Position / Bullet reset
	player.border();

	//Time Limit
	timer += 1;
	if (timer >= 60)
	{
		timer -= 60;
		seconds += 1;	
	}
	std::string timeLeft = std::to_string(120 - seconds);
	timeValueLabel->setString(timeLeft);

	if (seconds >= ptr->m_tetrisTimeLimit)
	{
		gameOver();
	}

	// Punishment
	if (ptr->m_punish == true)
	{
		punishTimer += 1;

		if (punishTimer >= (60 * 10))
		{
			punishTimer = 0;
			ptr->m_punish = false;
		}

		std::string timerLeft = std::to_string(punishTimer / 60);
		punishTimerLabel->setString(timerLeft + " / 10");
	}

	// Score
	std::string currentScore = std::to_string(ptr->m_tetrisScore);
	scoreValueLabel->setString(currentScore);
}

bool TetrisScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event)
{
	CCLOG("Began x = %f, y = %f", touch->getLocation().x, touch->getLocation().y);

	hold = true;
	touched = touch;
	delayHold = 0.2f;

	return true;
}

void TetrisScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event * event)
{
	CCLOG("Ended x = %f, y = %f", touch->getLocation().x, touch->getLocation().y);
	
	if (player.getFired() == false && touched->getLocation().y > visibleSize.height / 2)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/shoot.wav");
		player.setFired(true);
	}

	hold = false;
}

void TetrisScene::addTVGlow(cocos2d::Size const & visibleSize)
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	// TV Effect
	auto TV = cocos2d::Sprite::create(ptr->m_TVGlow);
	TV->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	TV->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(TV);
}

void TetrisScene::addTutorial(cocos2d::Size const & visibleSize)
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	auto scale = Director::getInstance()->getContentScaleFactor();

	// Fire Tutorial
	auto fireTut = cocos2d::Sprite::create(ptr->m_tetrisTutorialFire);
	fireTut->setScale(scale);
	fireTut->setPosition(Point((fireTut->getContentSize().width * scale) / 2,
		(visibleSize.height) - (fireTut->getContentSize().height * scale) / 2));
	this->addChild(fireTut);

	// Move Tutorial
	auto moveTut = cocos2d::Sprite::create(ptr->m_tetrisTutorialMove);
	moveTut->setScale(Director::getInstance()->getContentScaleFactor());
	moveTut->setPosition(Point((moveTut->getContentSize().width * scale) / 2,
		(visibleSize.height / 2) - (moveTut->getContentSize().height * scale) / 2));
	this->addChild(moveTut);
}

void TetrisScene::menuCloseCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void TetrisScene::activatePauseScene(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	auto scene = PauseMenuScene::createScene();
	Director::getInstance()->pushScene(scene);
}

void TetrisScene::gameOver()
{
	auto scene = GameOver::createScene();
	Director::getInstance()->replaceScene(scene);
}

