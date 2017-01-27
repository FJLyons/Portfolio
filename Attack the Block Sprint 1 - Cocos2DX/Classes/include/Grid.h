#ifndef __GRID_H__
#define __GRID_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Tile.h"
#include "Player.h"
#include "LevelLoader.h"

class Grid
{
public:
	Grid();
	~Grid();

	void createGrid(cocos2d::CCLayerColor *layer);
	void createRows(cocos2d::CCLayerColor *layer);

	void createFirstBlocks(cocos2d::CCLayerColor *layer, int i);
	void createSecondBlocks(cocos2d::CCLayerColor *layer, int i);
	void createThirdBlocks(cocos2d::CCLayerColor *layer, int i);

	void update();
	void destroy(cocos2d::CCLayerColor *layer);

	void punishment(cocos2d::CCLayerColor *layer);
	void clearLine(cocos2d::CCLayerColor *layer, cocos2d::Color3B colour);


	void bulletCollision(Player *player, cocos2d::CCLayerColor *layer);

	void fillGrid(cocos2d::CCLayerColor *layer, int gridCol, int rot, int typ);

	void drawFull(cocos2d::CCLayerColor *layer, int row, int col, cocos2d::Color3B);

private:
	cocos2d::Size visibleSize;

	MyTile enemyTiles[10][10];
	MyTile playerTiles[10][10];

	cocos2d::Sprite *fullPlayerTile[10][10];

	std::vector<MyTile> firstRow;
	std::vector<MyTile> secondRow;
	std::vector<MyTile> thirdRow;

	int size = 24;
	int randomBlock = 0;
	int randomRotation = 1;

	bool newBlockFirst = false;
	bool newBlockSecond = false;
	bool newBlockThird = false;

	int minRow[10];
	int offsetCol = 0;
	int gap = 0;

	std::vector<int> occupiedRowOfCols;

	cocos2d::Vec2 blockOnePos;
	cocos2d::Vec2 blockTwoPos;
	cocos2d::Vec2 blockThreePos;
	cocos2d::Vec2 blockFourPos;

	std::shared_ptr<GameData> ptr;

};

#endif //__GRID_H__



