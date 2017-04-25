#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"
#include "MainMenuScene.h"
#include "TetrisScene.h"
#include "BreakoutScene.h"
#include "LevelLoader.h"

class GameOver : public cocos2d::CCLayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void activateMenuScene(Ref * pSender);
	void activateTetrisScene(Ref * pSender);
	void activateBreakoutScene(Ref * pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(GameOver);

	void update(float dt);


private:
	// Options
	void addTVGlow(cocos2d::Size const & visibleSize);

	//Loading
	std::shared_ptr<GameData> ptr;

	cocos2d::LabelTTF *scoreLabelTetris;
	cocos2d::LabelTTF *scoreValueLabelTetris;

	cocos2d::LabelTTF *scoreLabelBreakout;
	cocos2d::LabelTTF *scoreValueLabelBreakout;
};

#endif //__GAMEOVER_SCENE_H__

