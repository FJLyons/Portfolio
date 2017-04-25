#ifndef __HIGHSCORE_SCENE_H__
#define __HIGHSCORE_SCENE_H__

#include "cocos2d.h"
#include "MainMenuScene.h"
#include "LevelLoader.h"

class HighScoreScene : public cocos2d::CCLayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void activateMenuScene(Ref * pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(HighScoreScene);

	void update(float dt);

private:
	// Options
	void addTVGlow(cocos2d::Size const & visibleSize);

	// Load
	std::shared_ptr<GameData> ptr;

	cocos2d::LabelTTF *scoreLabelTetris;
	cocos2d::LabelTTF *scoreValueLabelTetris;

	cocos2d::LabelTTF *scoreLabelBreakout;
	cocos2d::LabelTTF *scoreValueLabelBreakout;
};

#endif //__HIGHSCORE_SCENE_H__

