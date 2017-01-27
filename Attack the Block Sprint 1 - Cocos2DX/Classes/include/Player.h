#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Tile.h"

class Player
{
public:
	Player();
	~Player();

	void createPlayer(cocos2d::CCLayerColor *layer);
	void createBullet(cocos2d::CCLayerColor *layer);

	void movePlayerLeft();
	void movePlayerRight();

	cocos2d::Vec2 getPlayerPosition();
	void setPlayerPosition(cocos2d::Vec2);

	void border();
	void resetBullet();

	void moveBullet();

	bool getFired();
	void setFired(bool);

	cocos2d::Vec2 getBulletPosition();
	cocos2d::Sprite *getBullet();

	int colUnder = 6;
	
private:
	cocos2d::Size visibleSize;

	cocos2d::Sprite *playerImage;
	cocos2d::Sprite *bulletImage;

	bool fired = false;

	int size = 24;
};

#endif //__PLAYER_H__
