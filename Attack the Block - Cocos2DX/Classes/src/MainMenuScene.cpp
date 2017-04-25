#include "MainMenuScene.h"
#include "TetrisScene.h"
#include "BreakoutScene.h"
#include "HighscoreScene.h"
#include "OptionsScene.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}

	CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)); // Set Colour to white

	Size visibleSize = Director::getInstance()->getVisibleSize();


	// Data Load
	ptr = GameData::sharedGameData();

	ptr->m_tetrisScore = 0;
	ptr->m_breakoutScore = 0;

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/select.wav");


	// Options
	if (ptr->m_defaultTVGlow == true) { addTVGlow(visibleSize); }
	if (ptr->m_defaultMusic == true) { addMusic(); }

	auto menuTitle = MenuItemImage::create("MainMenuScene/ATB.png", "MainMenuScene/ATB.png");

	auto tetrisButton = MenuItemImage::create("MainMenuScene/Tetris VS. Up.png", "MainMenuScene/Tetris VS. Down.png",
		CC_CALLBACK_1(MainMenu::activateTetrisScene, this));

	auto breakoutButton = MenuItemImage::create("MainMenuScene/Breakout VS. Up.png", "MainMenuScene/Breakout VS. Down.png",
		CC_CALLBACK_1(MainMenu::activateBreakoutScene, this));

	auto highscoreButton = MenuItemImage::create("MainMenuScene/High Score Up.png", "MainMenuScene/High Score Down.png",
		CC_CALLBACK_1(MainMenu::activateHighscoreScene, this)); 

	auto optionsButton = MenuItemImage::create("MainMenuScene/Options Up.png", "MainMenuScene/Options Down.png",
		CC_CALLBACK_1(MainMenu::activateOptionsScene, this));

	auto exitButton = MenuItemImage::create("MainMenuScene/Exit Up.png", "MainMenuScene/Exit Down.png",
		CC_CALLBACK_1(MainMenu::menuCloseCallback, this));

	menuTitle->setScale(Director::getInstance()->getContentScaleFactor());
	tetrisButton->setScale(Director::getInstance()->getContentScaleFactor());
	breakoutButton->setScale(Director::getInstance()->getContentScaleFactor());
	highscoreButton->setScale(Director::getInstance()->getContentScaleFactor());
	optionsButton->setScale(Director::getInstance()->getContentScaleFactor());
	exitButton->setScale(Director::getInstance()->getContentScaleFactor());

	auto menu = Menu::create(menuTitle, tetrisButton, breakoutButton, highscoreButton, optionsButton, exitButton, NULL);

	// Returns visible size of OpenGL window in points.
	menu->alignItemsVerticallyWithPadding(visibleSize.height / 12);
	this->addChild(menu);

	return true;
}

void MainMenu::menuCloseCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MainMenu::activateTetrisScene(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	auto scene = TetrisScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void MainMenu::activateBreakoutScene(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	auto scene = BreakoutScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void MainMenu::activateHighscoreScene(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	auto scene = HighScoreScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void MainMenu::activateOptionsScene(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	auto scene = OptionsScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void MainMenu::addTVGlow(cocos2d::Size const & visibleSize)
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	// TV Effect
	auto TV = cocos2d::Sprite::create(ptr->m_TVGlow);
	TV->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	TV->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(TV);
}

void MainMenu::addMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Audio/background.mp3", ptr->m_defaultMusic);
}

