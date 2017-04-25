#include "HighScoreScene.h"

USING_NS_CC;

Scene* HighScoreScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HighScoreScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HighScoreScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)); // Set Colour to white

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto highScore = MenuItemImage::create("HighScoreScene/High Score.png", "HighScoreScene/High Score.png");
	highScore->setPosition(Point(visibleSize.width / 2, visibleSize.height - (visibleSize.height / 16)));
	highScore->setScale(Director::getInstance()->getContentScaleFactor());

	auto menuItem = MenuItemImage::create("HighScoreScene/Return Up.png", "HighScoreScene/Return Down.png", CC_CALLBACK_1(HighScoreScene::activateMenuScene, this));
	menuItem->setPosition(Point(visibleSize.width / 2, visibleSize.height / 8));
	menuItem->setScale(Director::getInstance()->getContentScaleFactor());

	auto menu = Menu::create(highScore, menuItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	// Data Load
	ptr = GameData::sharedGameData();

	// Options
	if (ptr->m_defaultTVGlow == true) { addTVGlow(visibleSize); }

	// Display Score
	//Tetris
	scoreLabelTetris = LabelTTF::create("TETRIS HIGH SCORE: ", "OptionsScene/kenvector_future.ttf", 24);
	scoreLabelTetris->setPosition(Point(visibleSize.width / 16, visibleSize.height - (visibleSize.height / 3)));
	scoreLabelTetris->setAnchorPoint(Point(0.0f, 0.0f));
	scoreLabelTetris->setColor(cocos2d::Color3B::BLACK);
	this->addChild(scoreLabelTetris);

	scoreValueLabelTetris = LabelTTF::create("0", "OptionsScene/kenvector_future.ttf", 48);
	scoreValueLabelTetris->setPosition(Point(visibleSize.width / 16, visibleSize.height - (visibleSize.height / 3) - 48));
	scoreValueLabelTetris->setAnchorPoint(Point(0.0f, 0.0f));
	scoreValueLabelTetris->setColor(cocos2d::Color3B::BLACK);
	this->addChild(scoreValueLabelTetris);

	std::string scoreValueTetris = std::to_string(ptr->m_tetrisHighScore);
	scoreValueLabelTetris->setString(scoreValueTetris);

	// Breakout
	scoreLabelBreakout = LabelTTF::create("BREAKOUT HIGH SCORE: ", "OptionsScene/kenvector_future.ttf", 24);
	scoreLabelBreakout->setPosition(Point(visibleSize.width - (visibleSize.width / 3.4f), visibleSize.height - (visibleSize.height / 3)));
	scoreLabelBreakout->setAnchorPoint(Point(0.0f, 0.0f));
	scoreLabelBreakout->setColor(cocos2d::Color3B::BLACK);
	this->addChild(scoreLabelBreakout);

	scoreValueLabelBreakout = LabelTTF::create("0", "OptionsScene/kenvector_future.ttf", 48);
	scoreValueLabelBreakout->setPosition(Point(visibleSize.width - (visibleSize.width / 3.4f), visibleSize.height - (visibleSize.height / 3) - 48));
	scoreValueLabelBreakout->setAnchorPoint(Point(0.0f, 0.0f));
	scoreValueLabelBreakout->setColor(cocos2d::Color3B::BLACK);
	this->addChild(scoreValueLabelBreakout);

	std::string scoreValueBreakout = std::to_string(ptr->m_breakoutHighScore);
	scoreValueLabelBreakout->setString(scoreValueBreakout);

	// Update Call
	this->scheduleUpdate();

	return true;
}

void HighScoreScene::update(float dt)
{

}
void HighScoreScene::menuCloseCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HighScoreScene::activateMenuScene(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(scene);
}

void HighScoreScene::addTVGlow(cocos2d::Size const & visibleSize)
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	// TV Effect
	auto TV = cocos2d::Sprite::create(ptr->m_TVGlow);
	TV->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	TV->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(TV);
}

