#ifndef __PAUSEMENU_SCENE_H__
#define __PAUSEMENU_SCENE_H__

#include "cocos2d.h"
#include "MainMenuScene.h"
#include "LevelLoader.h"

class PauseMenuScene : public cocos2d::CCLayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void resume(Ref *pSender);
	void menuCloseCallback(cocos2d::Ref* pSender);

	void activateMenuScene(Ref * pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(PauseMenuScene);


private:
	// Options
	void addTVGlow(cocos2d::Size const & visibleSize);

	// Load
	std::shared_ptr<GameData> ptr;
};

#endif //__PAUSEMENU_SCENE_H__

