#ifndef __MYTILE_H__
#define __MYTILE_H__

#include "cocos2d.h"

class MyTile
{
public:
	MyTile();
	~MyTile();

	void createEnemyTile(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point);
	void createPlayerTile(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point);
	
	void createBlueBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point);
	void createGreenBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point);
	void createOrangeBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point);
	void createPinkBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point);
	void createRedBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point);
	void createTurqBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point);
	void createYellowBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point);

	float getType();
	void setType(float);

	float getRotation();
	void setRotation(float);

	bool getFilled();
	void setFilled(bool);

	cocos2d::Vec2 getBlockPosition();

	cocos2d::Vec2 getEnemyTilePosition();
	cocos2d::Vec2 getPlayerTilePosition();

	void updateFirstRow();
	void updateSecondRow();
	void updateThirdRow();

	void removeImage(cocos2d::CCLayerColor *layer);

	cocos2d::Sprite *getBlock();

	void createBreakoutBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point);

private:
	cocos2d::Size visibleSize;

	int size = 24;
	float scale;

	float blockType = 0;

	cocos2d::Sprite *BLUE;
	cocos2d::Sprite *GREEN;
	cocos2d::Sprite *ORANGE;
	cocos2d::Sprite *PINK;
	cocos2d::Sprite *RED;
	cocos2d::Sprite *TURQ;
	cocos2d::Sprite *YELLOW;


	cocos2d::Sprite *BREAKOUT;

	float rotation;
	bool filled = false;

	cocos2d::Vec2 playerTilePosition;
	cocos2d::Vec2 enemyTilePosition;
};

#endif //__MYTILE_H__
