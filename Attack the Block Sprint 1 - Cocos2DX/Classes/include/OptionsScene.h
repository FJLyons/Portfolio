#ifndef __OPTIONS_SCENE_H__
#define __OPTIONS_SCENE_H__

#include "cocos2d.h"
#include "MainMenuScene.h"
#include "LevelLoader.h"

using namespace rapidjson;

class OptionsScene : public cocos2d::CCLayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void activateMenuScene(Ref * pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(OptionsScene);

	void update(float);

	void swapMusic(Ref *pSender);
	void swapTV(Ref *pSender);
	void swapTutorial(Ref *pSender);

private:
	// Options
	void addTVGlow(cocos2d::Size const & visibleSize);
	// Load
	std::shared_ptr<GameData> ptr;

	cocos2d::LabelTTF *musicLabel;
	cocos2d::LabelTTF *tutLabel;
	cocos2d::LabelTTF *tvLabel;
};

#endif //__OPTIONS_SCENE_H__

