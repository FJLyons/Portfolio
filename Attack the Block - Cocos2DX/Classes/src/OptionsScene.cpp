#include "OptionsScene.h"

USING_NS_CC;

Scene* OptionsScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = OptionsScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool OptionsScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)); // Set Colour to white

	Size visibleSize = Director::getInstance()->getVisibleSize();

	// Data Load
	ptr = GameData::sharedGameData();
	// Options
	if (ptr->m_defaultTVGlow == true) { addTVGlow(visibleSize); }


	auto music = MenuItemImage::create("OptionsScene/Music Up.png", "OptionsScene/Music Down.png", CC_CALLBACK_1(OptionsScene::swapMusic, this));
	music->setPosition(Point(visibleSize.width / 3, visibleSize.height / 3));
	music->setScale(Director::getInstance()->getContentScaleFactor());


	auto tutorial = MenuItemImage::create("OptionsScene/Tutorial Up.png", "OptionsScene/Tutorial Down.png", CC_CALLBACK_1(OptionsScene::swapTutorial, this));
	tutorial->setPosition(Point(visibleSize.width / 3, visibleSize.height / 2));
	tutorial->setScale(Director::getInstance()->getContentScaleFactor());


	auto TV = MenuItemImage::create("OptionsScene/TV Up.png", "OptionsScene/TV Down.png", CC_CALLBACK_1(OptionsScene::swapTV, this));
	TV->setPosition(Point(visibleSize.width / 3, visibleSize.height - (visibleSize.height / 3)));
	TV->setScale(Director::getInstance()->getContentScaleFactor());


	auto menuItem = MenuItemImage::create("OptionsScene/Return Up.png", "OptionsScene/Return Down.png", CC_CALLBACK_1(OptionsScene::activateMenuScene, this));
	menuItem->setPosition(Point(visibleSize.width / 2, visibleSize.height / 8));
	menuItem->setScale(Director::getInstance()->getContentScaleFactor());

	auto menu = Menu::create(menuItem, TV, tutorial, music, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	if (ptr->m_defaultMusic == true) { musicLabel = LabelTTF::create("ON", "OptionsScene/kenvector_future.ttf", 32); }
	else{ musicLabel = LabelTTF::create("OFF", "OptionsScene/kenvector_future.ttf", 32); }
	musicLabel->setPosition(Point(visibleSize.width - (visibleSize.width / 3), visibleSize.height / 3));
	musicLabel->setAnchorPoint(Point(0.0f, 0.0f));
	musicLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(musicLabel);

	if (ptr->m_defaultTutorial == true) { tutLabel = LabelTTF::create("ON", "OptionsScene/kenvector_future.ttf", 32); }
	else{ tutLabel = LabelTTF::create("OFF", "OptionsScene/kenvector_future.ttf", 32); }
	tutLabel->setPosition(Point(visibleSize.width - (visibleSize.width / 3), visibleSize.height / 2));
	tutLabel->setAnchorPoint(Point(0.0f, 0.0f));
	tutLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(tutLabel);

	if (ptr->m_defaultTVGlow == true) { tvLabel = LabelTTF::create("ON", "OptionsScene/kenvector_future.ttf", 32); }
	else{ tvLabel = LabelTTF::create("OFF", "OptionsScene/kenvector_future.ttf", 32); }
	tvLabel->setPosition(Point(visibleSize.width - (visibleSize.width / 3), visibleSize.height - (visibleSize.height / 3)));
	tvLabel->setAnchorPoint(Point(0.0f, 0.0f));
	tvLabel->setColor(cocos2d::Color3B::BLACK);
	this->addChild(tvLabel);

	// Update Call
	this->scheduleUpdate();

	return true;
}

void OptionsScene::update(float dt)
{
	if (ptr->m_defaultTVGlow == true) { tvLabel->setString("ON"); }
	else{ tvLabel->setString("OFF"); }

	if (ptr->m_defaultTutorial == true) { tutLabel->setString("ON");  }
	else{ tutLabel->setString("OFF"); }

	if (ptr->m_defaultMusic == true) { musicLabel->setString("ON"); }
	else{ musicLabel->setString("OFF"); CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(); }
}

void OptionsScene::menuCloseCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void OptionsScene::activateMenuScene(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(scene);
}

void OptionsScene::swapMusic(Ref *pSender) {
	ptr->m_defaultMusic = !ptr->m_defaultMusic;
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
}

void OptionsScene::swapTV(Ref *pSender){
	ptr->m_defaultTVGlow = !ptr->m_defaultTVGlow;
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
}

void OptionsScene::swapTutorial(Ref *pSender) {
	ptr->m_defaultTutorial = !ptr->m_defaultTutorial;
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
}

void OptionsScene::addTVGlow(cocos2d::Size const & visibleSize)
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	// TV Effect
	auto TV = cocos2d::Sprite::create(ptr->m_TVGlow);
	TV->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	TV->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(TV);
}



