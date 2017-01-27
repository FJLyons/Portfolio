#ifndef __TETRIS_SCENE_H__
#define __TETRIS_SCENE_H__

#include "cocos2d.h"
#include "MainMenuScene.h"

#include "Grid.h"
#include "Player.h"

#include "LevelLoader.h"

#include "GameOverScene.h"
#include "PauseMenuScene.h"

class TetrisScene : public cocos2d::CCLayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event * event);

	void update(float dt);

	// Options
	void addTVGlow(cocos2d::Size const & visibleSize);
	void addTutorial(cocos2d::Size const & visibleSize);


	void gameOver(); // Game Over

	void activatePauseScene(Ref * pSender); // Return

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(TetrisScene);

private:
	cocos2d::Size visibleSize;

	Grid grid;
	Player player;

	bool hold = false;
	cocos2d::Touch * touched;

	float delayHold = 0.0f;
	int callsHold = 0;

	float delayBlocks = 0.0f;
	int callsBlocks = 0;

	float delayFired = 0.0f;
	int callsFired = 0;

	float timer = 0.0f;
	int seconds = 0;

	int punishTimer = 0;

	// Load
	std::shared_ptr<GameData> ptr;

	cocos2d::LabelTTF *punishLabel;
	cocos2d::LabelTTF *punishTimerLabel;

	cocos2d::LabelTTF *scoreLabel;
	cocos2d::LabelTTF *scoreValueLabel;

	cocos2d::LabelTTF *timeLabel;
	cocos2d::LabelTTF *timeValueLabel;
};

#endif //__TETRIS_SCENE_H__

