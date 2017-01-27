#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "LevelLoader.h"
#include "SimpleAudioEngine.h"
#include "MainMenuScene.h"


class  AppDelegate : private cocos2d::Application
{
private:
	std::shared_ptr<GameData> m_ptr;

public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();


    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
};

#endif //_APP_DELEGATE_H_

