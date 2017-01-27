#ifndef __PADDLE_H__
#define __PADDLE_H__

#include "cocos2d.h"
#include "LevelLoader.h"

class Paddle
{
public:
	Paddle();
	~Paddle();

	void createPaddle(cocos2d::CCLayerColor *layer);
	void movePaddleLeft();
	void movePaddleRight();

	cocos2d::Vec2 getPaddlePosition();
	void setPaddlePosition(cocos2d::Vec2);


	cocos2d::Sprite *getPaddle();

	void border();

	
private:
	cocos2d::Size visibleSize;

	cocos2d::Sprite *playerImage;

	cocos2d::Vector<cocos2d::Sprite *> playerPaddleImage;
	cocos2d::Vector<cocos2d::SpriteFrame*> playerPaddleFrames;

	int size = 32;
};

#endif //__PADDLE_H__
