#include "GameOverScene.h"

USING_NS_CC;

Scene* GameOver::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOver::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameOver::init()
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

	auto gameOver = MenuItemImage::create("GameOverScene/Game Over.png", "GameOverScene/Game Over.png");
	gameOver->setPosition(Point(visibleSize.width / 2, visibleSize.height - (visibleSize.height / 16)));
	gameOver->setScale(Director::getInstance()->getContentScaleFactor());

	auto win = MenuItemImage::create("GameOverScene/Win.png", "GameOverScene/Win.png");
	win->setPosition(Point(visibleSize.width / 2, visibleSize.height - (visibleSize.height / 3) + (win->getContentSize().height) * Director::getInstance()->getContentScaleFactor()));
	win->setScale(Director::getInstance()->getContentScaleFactor());

	auto draw = MenuItemImage::create("GameOverScene/Draw.png", "GameOverScene/Draw.png");
	draw->setPosition(Point(visibleSize.width / 2, visibleSize.height - (visibleSize.height / 3)));
	draw->setScale(Director::getInstance()->getContentScaleFactor());

	auto lose = MenuItemImage::create("GameOverScene/Lose.png", "GameOverScene/Lose.png");
	lose->setPosition(Point(visibleSize.width / 2, visibleSize.height - (visibleSize.height / 3) - (lose->getContentSize().height) * Director::getInstance()->getContentScaleFactor()));
	lose->setScale(Director::getInstance()->getContentScaleFactor());

	if (ptr->isTetris == true && ptr->m_tetrisScore <= 50 || ptr->isTetris == false && ptr->m_breakoutScore < 0)
	{
		win->setOpacity(100);
		draw->setOpacity(100);
		lose->setOpacity(255);
	}

	else if (ptr->isTetris == true && ptr->m_tetrisScore > 50 && ptr->m_tetrisScore <= 100 || ptr->isTetris == false && ptr->m_breakoutScore == 0)
	{
		win->setOpacity(100);
		draw->setOpacity(255);
		lose->setOpacity(100);
	}

	else if (ptr->isTetris == true && ptr->m_tetrisScore > 100 || ptr->isTetris == false && ptr->m_breakoutScore > 0)
	{
		win->setOpacity(255);
		draw->setOpacity(100);
		lose->setOpacity(100);
	}

	auto mainMenu = MenuItemImage::create("GameOverScene/Main Menu Up.png", "GameOverScene/Main Menu Down.png", CC_CALLBACK_1(GameOver::activateMenuScene, this));
	mainMenu->setPosition(Point(visibleSize.width / 2, visibleSize.height / 3));
	mainMenu->setScale(Director::getInstance()->getContentScaleFactor());

	auto playTetris = MenuItemImage::create("GameOverScene/Tetris VS. Up.png", "GameOverScene/Tetris VS. Down.png", CC_CALLBACK_1(GameOver::activateTetrisScene, this));
	playTetris->setPosition(Point(visibleSize.width / 8, visibleSize.height / 3));
	playTetris->setScale(Director::getInstance()->getContentScaleFactor());

	auto playBreakout = MenuItemImage::create("GameOverScene/Breakout VS. Up.png", "GameOverScene/Breakout VS. Down.png", CC_CALLBACK_1(GameOver::activateBreakoutScene, this));
	playBreakout->setPosition(Point(visibleSize.width - (visibleSize.width / 8), visibleSize.height / 3));
	playBreakout->setScale(Director::getInstance()->getContentScaleFactor());

	auto exit = MenuItemImage::create("GameOverScene/Exit Up.png", "GameOverScene/Exit Down.png", CC_CALLBACK_1(GameOver::menuCloseCallback, this));
	exit->setPosition(Point(visibleSize.width / 2, visibleSize.height / 8));
	exit->setScale(Director::getInstance()->getContentScaleFactor());

	auto menu = Menu::create(gameOver, win, lose, draw, mainMenu, playTetris, playBreakout, exit, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	// TV Effect
	addTVGlow(visibleSize);

	// Display Score
	//Tetris
	scoreLabelTetris = LabelTTF::create("TETRIS SCORE: ", "OptionsScene/kenvector_future.ttf", 24);
	scoreLabelTetris->setPosition(Point(visibleSize.width / 16, visibleSize.height - (visibleSize.height / 3)));
	scoreLabelTetris->setAnchorPoint(Point(0.0f, 0.0f));
	scoreLabelTetris->setColor(cocos2d::Color3B::BLACK);
	this->addChild(scoreLabelTetris);

	scoreValueLabelTetris = LabelTTF::create("0", "OptionsScene/kenvector_future.ttf", 48);
	scoreValueLabelTetris->setPosition(Point(visibleSize.width / 16, visibleSize.height - (visibleSize.height / 3) - 48));
	scoreValueLabelTetris->setAnchorPoint(Point(0.0f, 0.0f));
	scoreValueLabelTetris->setColor(cocos2d::Color3B::BLACK);
	this->addChild(scoreValueLabelTetris);

	std::string scoreValueTetris = std::to_string(ptr->m_tetrisScore);
	scoreValueLabelTetris->setString(scoreValueTetris);

	// Breakout
	scoreLabelBreakout = LabelTTF::create("BREAKOUT SCORE: ", "OptionsScene/kenvector_future.ttf", 24);
	scoreLabelBreakout->setPosition(Point(visibleSize.width - (visibleSize.width / 4.2f), visibleSize.height - (visibleSize.height / 3)));
	scoreLabelBreakout->setAnchorPoint(Point(0.0f, 0.0f));
	scoreLabelBreakout->setColor(cocos2d::Color3B::BLACK);
	this->addChild(scoreLabelBreakout);

	scoreValueLabelBreakout = LabelTTF::create("0", "OptionsScene/kenvector_future.ttf", 48);
	scoreValueLabelBreakout->setPosition(Point(visibleSize.width - (visibleSize.width / 4.2f), visibleSize.height - (visibleSize.height / 3) - 48));
	scoreValueLabelBreakout->setAnchorPoint(Point(0.0f, 0.0f));
	scoreValueLabelBreakout->setColor(cocos2d::Color3B::BLACK);
	this->addChild(scoreValueLabelBreakout);

	std::string scoreValueBreakout = std::to_string(ptr->m_breakoutScore);
	scoreValueLabelBreakout->setString(scoreValueBreakout);

	// Set Highscore
	if (ptr->m_tetrisScore > ptr->m_tetrisHighScore)
	{
		ptr->m_tetrisHighScore = ptr->m_tetrisScore;
	}

	if (ptr->m_breakoutScore > ptr->m_breakoutHighScore)
	{
		ptr->m_breakoutHighScore = ptr->m_breakoutScore;
	}

	// Reset Scores
	ptr->m_tetrisScore = 0;
	ptr->m_breakoutScore = 0;

	// Update Call
	this->scheduleUpdate();

	return true;
}

void GameOver::update(float dt)
{

}

void GameOver::menuCloseCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameOver::activateMenuScene(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(scene);
}

void GameOver::activateTetrisScene(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	auto scene = TetrisScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void GameOver::activateBreakoutScene(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/select.wav");
	auto scene = BreakoutScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void GameOver::addTVGlow(cocos2d::Size const & visibleSize)
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	// TV Effect
	auto TV = cocos2d::Sprite::create(ptr->m_TVGlow);
	TV->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	TV->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(TV);
}

