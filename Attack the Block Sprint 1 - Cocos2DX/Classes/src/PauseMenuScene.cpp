#include "PauseMenuScene.h"

USING_NS_CC;

Scene* PauseMenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = PauseMenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool PauseMenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)); // Set Colour to white

	Size visibleSize = Director::getInstance()->getVisibleSize();


	auto menuTitle = MenuItemImage::create("MainMenuScene/ATB.png", "MainMenuScene/ATB.png");
	menuTitle->setScale(Director::getInstance()->getContentScaleFactor());

	auto resumeItem = MenuItemImage::create("PauseMenuScene/Return Up.png", "PauseMenuScene/Return Down.png", CC_CALLBACK_1(PauseMenuScene::resume, this));
	resumeItem->setScale(Director::getInstance()->getContentScaleFactor());

	auto menuItem = MenuItemImage::create("PauseMenuScene/Main Menu Up.png", "PauseMenuScene/Main Menu Down.png", CC_CALLBACK_1(PauseMenuScene::activateMenuScene, this));
	menuItem->setScale(Director::getInstance()->getContentScaleFactor());

	auto menu = Menu::create(menuTitle, resumeItem, menuItem, NULL);

	menu->alignItemsVerticallyWithPadding(visibleSize.height / 4);
	this->addChild(menu);


	// Data Load
	ptr = GameData::sharedGameData();

	// Options
	if (ptr->m_defaultTVGlow == true){ addTVGlow(visibleSize); }

	return true;
}

void PauseMenuScene::resume(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	Director::getInstance()->popScene();
}

void PauseMenuScene::menuCloseCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void PauseMenuScene::activateMenuScene(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(scene);
}

void PauseMenuScene::addTVGlow(cocos2d::Size const & visibleSize)
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	// TV Effect
	auto TV = cocos2d::Sprite::create(ptr->m_TVGlow);
	TV->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	TV->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(TV);
}

