#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "LevelLoader.h"

class Ball
{
public:
	Ball();
	~Ball();

	void createBall(cocos2d::CCLayerColor *layer);
	void moveBall();

	cocos2d::Vec2 getBallPosition();
	void setBallPosition(cocos2d::Vec2);

	cocos2d::Vec2 getBallVelocity();
	void setBallVelocity(cocos2d::Vec2);

	void border();

	cocos2d::Sprite *getBall();


	cocos2d::Vec2 velocity;
	
private:
	cocos2d::Size visibleSize;

	cocos2d::Sprite *ballImage;


	std::shared_ptr<GameData> ptr;


	int size = 24;
};

#endif //__BALL_H__
