#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "LevelLoader.h"

class MainMenu : public cocos2d::CCLayerColor
{
public:

    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	void activateTetrisScene(Ref * pSender);
	void activateBreakoutScene(Ref * pSender);
	void activateHighscoreScene(Ref * pSender);
	void activateOptionsScene(Ref * pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);

private:
	// Options
	void addTVGlow(cocos2d::Size const & visibleSize);
	void addMusic();

	// Load
	std::shared_ptr<GameData> ptr;

};

#endif //__MAINMENU_SCENE_H__
