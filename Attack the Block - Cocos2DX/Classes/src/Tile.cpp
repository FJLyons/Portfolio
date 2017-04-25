#include "Tile.h"

USING_NS_CC;

MyTile::MyTile()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	scale = Director::getInstance()->getContentScaleFactor() * 0.5f;
}

MyTile::~MyTile()
{

}

void MyTile::createEnemyTile(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point)
{
	enemyTilePosition = point;
	auto enemyTile = cocos2d::Sprite::create("Grid/Grid Enemy.png");
	enemyTile->setPosition(Point(point));
	enemyTile->setScale(Director::getInstance()->getContentScaleFactor());

	layer->addChild(enemyTile);

	enemyTilePosition = point;
}

cocos2d::Vec2 MyTile::getEnemyTilePosition()
{
	return enemyTilePosition;
}

void MyTile::createPlayerTile(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point)
{
	playerTilePosition = point;
	auto playerTile = cocos2d::Sprite::create("Grid/Grid Player.png");
	playerTile->setPosition(Point(point));
	playerTile->setScale(Director::getInstance()->getContentScaleFactor());

	layer->addChild(playerTile);

	playerTilePosition = point;
}

cocos2d::Vec2 MyTile::getPlayerTilePosition()
{
	return playerTilePosition;
}

float MyTile::getType()
{
	return blockType;
}

void MyTile::setType(float type)
{
	blockType = type;
}

cocos2d::Sprite *MyTile::getBlock()
{
	cocos2d::Sprite *block;

	if (blockType == 0) { block = BLUE; }
	else if (blockType == 1) { block = GREEN; }
	else if (blockType == 2) { block = ORANGE; }
	else if (blockType == 3) { block = PINK; }
	else if (blockType == 4) { block = RED; }
	else if (blockType == 5) { block = TURQ; }
	else if (blockType == 6) { block = YELLOW; }


	else if (blockType == -1) { block = BREAKOUT; }

	return block;
}

float MyTile::getRotation()
{
	return rotation;
}

void MyTile::setRotation(float rot)
{
	rotation = rot;

	if (blockType == 0) { BLUE->setRotation(90 * rotation); }
	else if (blockType == 1) { GREEN->setRotation(90 * rotation); }
	else if (blockType == 2) { ORANGE->setRotation(90 * rotation); }
	else if (blockType == 3) { PINK->setRotation(90 * rotation); }
	else if (blockType == 4) { RED->setRotation(90 * rotation); }
	else if (blockType == 5) { TURQ->setRotation(90 * rotation); }
	else if (blockType == 6) { YELLOW->setRotation(90 * rotation); }
}

cocos2d::Vec2 MyTile::getBlockPosition()
{
	auto position = Vec2(0, 0);

	if (blockType == 0) { position = BLUE->getPosition(); }
	else if (blockType == 1) { position = GREEN->getPosition(); }
	else if (blockType == 2) { position = ORANGE->getPosition(); }
	else if (blockType == 3) { position = PINK->getPosition(); }
	else if (blockType == 4) { position = RED->getPosition(); }
	else if (blockType == 5) { position = TURQ->getPosition(); }
	else if (blockType == 6) { position = YELLOW->getPosition(); }

	// Game 2
	else if (blockType == -1) { position = BREAKOUT->getPosition(); }

	return position;
}

bool MyTile::getFilled()
{
	return filled;
}

void MyTile::setFilled(bool fill)
{
	filled = fill;
}


void MyTile::createBreakoutBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point)
{
	BREAKOUT = cocos2d::Sprite::create("BreakoutScene/Brick.png");
	BREAKOUT->setPosition(Point(point));
	BREAKOUT->setScale(scale * 2);
	layer->addChild(BREAKOUT);
}


void MyTile::removeImage(cocos2d::CCLayerColor *layer)
{
	if (blockType == 0) { layer->removeChild(BLUE); }
	else if (blockType == 1) { layer->removeChild(GREEN); }
	else if (blockType == 2) { layer->removeChild(ORANGE); }
	else if (blockType == 3) { layer->removeChild(PINK); }
	else if (blockType == 4) { layer->removeChild(RED); }
	else if (blockType == 5) { layer->removeChild(TURQ); }
	else if (blockType == 6) { layer->removeChild(YELLOW); }


	else if (blockType == -1) { layer->removeChild(BREAKOUT); }

}

#pragma region update rows
void MyTile::updateFirstRow()
{
	auto action = MoveBy::create(1, Point(-size / 2, 0));

	if (blockType == 0) { BLUE->runAction(action); }
	else if (blockType == 1) { GREEN->runAction(action); }
	else if (blockType == 2) { ORANGE->runAction(action); }
	else if (blockType == 3) { PINK->runAction(action); }
	else if (blockType == 4) { RED->runAction(action); }
	else if (blockType == 5) { TURQ->runAction(action); }
	else if (blockType == 6) { YELLOW->runAction(action); }

	else if (blockType == -1) { BREAKOUT->runAction(action); }
}

void MyTile::updateSecondRow()
{
	auto action = MoveBy::create(1, Point(size / 2, 0));

	if (blockType == 0) { BLUE->runAction(action); }
	else if (blockType == 1) { GREEN->runAction(action); }
	else if (blockType == 2) { ORANGE->runAction(action); }
	else if (blockType == 3) { PINK->runAction(action); }
	else if (blockType == 4) { RED->runAction(action); }
	else if (blockType == 5) { TURQ->runAction(action); }
	else if (blockType == 6) { YELLOW->runAction(action); }

	else if (blockType == -1) { BREAKOUT->runAction(action); }
}

void MyTile::updateThirdRow()
{
	auto action = MoveBy::create(1, Point(-size / 2, 0));

	if (blockType == 0) { BLUE->runAction(action); }
	else if (blockType == 1) { GREEN->runAction(action); }
	else if (blockType == 2) { ORANGE->runAction(action); }
	else if (blockType == 3) { PINK->runAction(action); }
	else if (blockType == 4) { RED->runAction(action); }
	else if (blockType == 5) { TURQ->runAction(action); }
	else if (blockType == 6) { YELLOW->runAction(action); }

	else if (blockType == -1) { BREAKOUT->runAction(action); }
}
#pragma endregion

#pragma region Create Blocks
void MyTile::createBlueBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point)
{
	BLUE = cocos2d::Sprite::create("Blocks/Blue.png");
	BLUE->setPosition(Point(point));
	BLUE->setScale(scale);
	layer->addChild(BLUE);
}

void MyTile::createGreenBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point)
{
	GREEN = cocos2d::Sprite::create("Blocks/Green.png");
	GREEN->setPosition(Point(point));
	GREEN->setScale(scale);
	layer->addChild(GREEN);
}

void MyTile::createOrangeBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point)
{
	ORANGE = cocos2d::Sprite::create("Blocks/Orange.png");
	ORANGE->setPosition(Point(point));
	ORANGE->setScale(scale);
	layer->addChild(ORANGE);
}

void MyTile::createPinkBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point)
{
	PINK = cocos2d::Sprite::create("Blocks/Pink.png");
	PINK->setPosition(Point(point));
	PINK->setScale(scale);
	layer->addChild(PINK);
}

void MyTile::createRedBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point)
{
	RED = cocos2d::Sprite::create("Blocks/Red.png");
	RED->setPosition(Point(point));
	RED->setScale(scale);
	layer->addChild(RED);
}

void MyTile::createTurqBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point)
{
	TURQ = cocos2d::Sprite::create("Blocks/Turq.png");
	TURQ->setPosition(Point(point));
	TURQ->setScale(scale);
	layer->addChild(TURQ);
}

void MyTile::createYellowBlock(cocos2d::CCLayerColor *layer, cocos2d::Vec2 point)
{
	YELLOW = cocos2d::Sprite::create("Blocks/Yellow.png");
	YELLOW->setPosition(Point(point));
	YELLOW->setScale(scale);
	layer->addChild(YELLOW);
}
#pragma endregion
