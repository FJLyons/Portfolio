#ifndef __BREAKOUT_SCENE_H__
#define __BREAKOUT_SCENE_H__

#include "MainMenuScene.h"
#include "LevelLoader.h"
#include "GameOverScene.h"
#include "PauseMenuScene.h"
#include "Paddle.h"
#include "Ball.h"

class BreakoutScene : public cocos2d::CCLayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void activatePauseScene(Ref * pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(BreakoutScene);


	void update(float dt); 
	void destroy(cocos2d::CCLayerColor *layer);

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event * event);

	void createBackdrop(cocos2d::CCLayerColor *layer);

	void createRows(cocos2d::CCLayerColor *layer);
	void createFirstBlocks(cocos2d::CCLayerColor *layer, int i);
	void createSecondBlocks(cocos2d::CCLayerColor *layer, int i);
	void createThirdBlocks(cocos2d::CCLayerColor *layer, int i);


	void gameOver(); // Game Over

private:
	cocos2d::Size visibleSize;
	
	cocos2d::Touch * touched;

	bool hold = false;
	float delayHold = 0.0f;
	int callsHold = 0;

	float delayBlocks = 0.0f;
	int callsBlocks = 0;

	Paddle paddle;
	Ball ball;

	std::vector<MyTile> firstRow;
	std::vector<MyTile> secondRow;
	std::vector<MyTile> thirdRow;

	int size = 24;

	bool newBlockFirst = false;
	bool newBlockSecond = false;
	bool newBlockThird = false;

	// Options
	void addTVGlow(cocos2d::Size const & visibleSize);
	// Load
	std::shared_ptr<GameData> ptr;

	cocos2d::LabelTTF *enemyLivesLabel;
	cocos2d::LabelTTF *enemyLivesValueLabel;

	cocos2d::LabelTTF *playerLivesLabel;
	cocos2d::LabelTTF *playerLivesValueLabel;

	cocos2d::LabelTTF *timeLabel;
	cocos2d::LabelTTF *timeValueLabel;

	cocos2d::LabelTTF *damageLabel;
	cocos2d::LabelTTF *damageValueLabel;


	// Enemy
	cocos2d::Sprite *enemyImage;
	cocos2d::Vector<cocos2d::Sprite *> enemyPaddleImage;
	cocos2d::Vector<cocos2d::SpriteFrame*> enemyPaddleFrames;

	void createEnemy(cocos2d::CCLayerColor *layer);
	void AI();
	void enemyBorder();



	float timer = 0.0f;
	int seconds = 0;


	void ballCollision(cocos2d::CCLayerColor *layer);
	int damage = 1;
};

#endif //_BREAKOUT_SCENE_H__

