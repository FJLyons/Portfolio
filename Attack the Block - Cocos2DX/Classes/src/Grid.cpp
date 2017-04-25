#include "Grid.h"

USING_NS_CC;

Grid::Grid()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	srand(time(0));

	for (int i = 0; i < 10; i++)
	{
		minRow[i] = 0;
	}

	// Data Load
	ptr = GameData::sharedGameData();
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/collision.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/clear.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/punish.wav");
}

Grid::~Grid()
{

}

void Grid::createGrid(cocos2d::CCLayerColor *layer)
{
	// Enemy Grid
	Vec2 enemyTopLeft = Vec2((visibleSize.width / 2) - (5 * size), visibleSize.height - (visibleSize.height / 10)); // Fills Down
	for (int i = 0; i < 10; i++) // col
	{
		for (int j = 0; j < 10; j++) // row
		{
			enemyTiles[i][j].createEnemyTile(layer, Vec2(enemyTopLeft.x + (i * size), enemyTopLeft.y - (j * size)));
		}
	}

	// Player Grid
	Vec2 playerTopLeft = Vec2((visibleSize.width / 2) - (5 * size), (visibleSize.height / 2) - visibleSize.height / 10); // Fills Down
	for (int i = 0; i < 10; i++) // col
	{
		for (int j = 0; j < 10; j++) // row
		{
			playerTiles[i][j].createPlayerTile(layer, Vec2(playerTopLeft.x + (i * size), playerTopLeft.y - (j * size)));
		}
	}
}

void Grid::createRows(cocos2d::CCLayerColor *layer)
{
	auto blockLane = cocos2d::Sprite::create("Grid/Grid Blank.png");
	blockLane->setPosition(Point(visibleSize.width / 2 - size / 2, visibleSize.height / 2)); // Center Point of row picture
	blockLane->setScale(Director::getInstance()->getContentScaleFactor());
	layer->addChild(blockLane);

	firstRow.resize(16);
	for (int i = 0; i < firstRow.size(); i++) // First Row
	{
		createFirstBlocks(layer, i);
	}

	secondRow.resize(16);
	for (int i = 0; i < secondRow.size(); i++) // Second Row
	{
		createSecondBlocks(layer, i);
	}

	thirdRow.resize(16);
	for (int i = 0; i < thirdRow.size(); i++) // Second Row
	{
		createThirdBlocks(layer, i);
	}
}

void Grid::update()
{
	for (int i = 0; i < firstRow.size(); i++) // First Row
	{
		firstRow.at(i).updateFirstRow();
	}

	for (int i = 0; i < secondRow.size(); i++) // Second Row
	{
		secondRow.at(i).updateSecondRow();
	}

	for (int i = 0; i < thirdRow.size(); i++) // Second Row
	{
		thirdRow.at(i).updateThirdRow();
	}
}

void Grid::destroy(cocos2d::CCLayerColor *layer)
{
	for (int i = 0; i < firstRow.size(); i++) // First Row
	{
		if (firstRow[i].getBlockPosition().x <  - size)
		{
			firstRow[i].removeImage(layer);
			firstRow.erase(firstRow.begin() + i);
			firstRow.resize(16);
			createFirstBlocks(layer, 15);
			newBlockFirst = true;
		}
	}

	for (int i = 0; i < secondRow.size(); i++) // First Row
	{
		if (secondRow[i].getBlockPosition().x > visibleSize.width + size)
		{
			secondRow[i].removeImage(layer);
			secondRow.erase(secondRow.begin() + i);
			secondRow.resize(16);
			createSecondBlocks(layer, 15);
			newBlockSecond = true;
		}
	}

	for (int i = 0; i < thirdRow.size(); i++) // First Row
	{
		if (thirdRow[i].getBlockPosition().x < -size)
		{
			thirdRow[i].removeImage(layer);
			thirdRow.erase(thirdRow.begin() + i);
			thirdRow.resize(16);
			createThirdBlocks(layer, 15);
			newBlockThird = true;
		}
	}
}

void Grid::punishment(cocos2d::CCLayerColor *layer)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/punish.wav");
	ptr->m_punish = true;
	for (int col = 0; col < 10; col++) 
	{
		for (int row = 0; row < 10; row++) 
		{
			playerTiles[col][row].setFilled(false);		

			layer->removeChild(fullPlayerTile[col][row]);
		}
	}
}

void Grid::createFirstBlocks(cocos2d::CCLayerColor *layer, int i)
{
	randomBlock = rand() % 7;
	randomRotation = rand() % 3 + 1;

	auto point = Vec2(visibleSize.width / 16 + ((size * 6) * i), (visibleSize.height / 2) + (size * 1.5f));

	if (newBlockFirst == true)
	{
		point = Vec2(firstRow[i - 1].getBlockPosition().x + (size * 3), (visibleSize.height / 2) + (size * 1.5f));
		newBlockFirst = false;
	}

	if (randomBlock == 0) { firstRow[i].createBlueBlock(layer, point); firstRow[i].setType(0); firstRow[i].setRotation(randomRotation); }
	else if (randomBlock == 1) { firstRow[i].createGreenBlock(layer, point); firstRow[i].setType(1); firstRow[i].setRotation(randomRotation); }
	else if (randomBlock == 2) { firstRow[i].createOrangeBlock(layer, point); firstRow[i].setType(2); firstRow[i].setRotation(randomRotation); }
	else if (randomBlock == 3) { firstRow[i].createPinkBlock(layer, point); firstRow[i].setType(3); firstRow[i].setRotation(randomRotation); }
	else if (randomBlock == 4) { firstRow[i].createRedBlock(layer, point); firstRow[i].setType(4); firstRow[i].setRotation(randomRotation); }
	else if (randomBlock == 5) { firstRow[i].createTurqBlock(layer, point); firstRow[i].setType(5); firstRow[i].setRotation(randomRotation); }
	else if (randomBlock == 6) { firstRow[i].createYellowBlock(layer, point); firstRow[i].setType(6); firstRow[i].setRotation(randomRotation); }
}

void Grid::createSecondBlocks(cocos2d::CCLayerColor *layer, int i)
{
	randomBlock = rand() % 7;
	randomRotation = rand() % 3 + 1;

	auto point = Vec2(visibleSize.width - (visibleSize.width / 16) - ((size * 4) * i), (visibleSize.height / 2));

	if (newBlockSecond == true)
	{
		point = Vec2(secondRow[i - 1].getBlockPosition().x - (size * 2.5f), (visibleSize.height / 2));
		newBlockSecond = false;
	}

	if (randomBlock == 0) { secondRow[i].createBlueBlock(layer, point); secondRow[i].setType(0); secondRow[i].setRotation(randomRotation); }
	else if (randomBlock == 1) { secondRow[i].createGreenBlock(layer, point); secondRow[i].setType(1); secondRow[i].setRotation(randomRotation); }
	else if (randomBlock == 2) { secondRow[i].createOrangeBlock(layer, point); secondRow[i].setType(2); secondRow[i].setRotation(randomRotation); }
	else if (randomBlock == 3) { secondRow[i].createPinkBlock(layer, point); secondRow[i].setType(3); secondRow[i].setRotation(randomRotation); }
	else if (randomBlock == 4) { secondRow[i].createRedBlock(layer, point); secondRow[i].setType(4); secondRow[i].setRotation(randomRotation); }
	else if (randomBlock == 5) { secondRow[i].createTurqBlock(layer, point); secondRow[i].setType(5); secondRow[i].setRotation(randomRotation); }
	else if (randomBlock == 6) { secondRow[i].createYellowBlock(layer, point); secondRow[i].setType(6); secondRow[i].setRotation(randomRotation); }
}

void Grid::createThirdBlocks(cocos2d::CCLayerColor *layer, int i)
{
	randomBlock = rand() % 7;
	randomRotation = rand() % 3 + 1;

	auto point = Vec2(visibleSize.width / 16 + ((size * 6) * i), (visibleSize.height / 2) - (size * 1.5f));

	if (newBlockThird == true)
	{
		point = Vec2(thirdRow[i - 1].getBlockPosition().x + (size * 3), (visibleSize.height / 2) - (size * 1.5f));
		newBlockThird = false;
	}

	if (randomBlock == 0) { thirdRow[i].createBlueBlock(layer, point); thirdRow[i].setType(0); thirdRow[i].setRotation(randomRotation); }
	else if (randomBlock == 1) { thirdRow[i].createGreenBlock(layer, point); thirdRow[i].setType(1); thirdRow[i].setRotation(randomRotation); }
	else if (randomBlock == 2) { thirdRow[i].createOrangeBlock(layer, point); thirdRow[i].setType(2); thirdRow[i].setRotation(randomRotation); }
	else if (randomBlock == 3) { thirdRow[i].createPinkBlock(layer, point); thirdRow[i].setType(3); thirdRow[i].setRotation(randomRotation); }
	else if (randomBlock == 4) { thirdRow[i].createRedBlock(layer, point); thirdRow[i].setType(4); thirdRow[i].setRotation(randomRotation); }
	else if (randomBlock == 5) { thirdRow[i].createTurqBlock(layer, point); thirdRow[i].setType(5); thirdRow[i].setRotation(randomRotation); }
	else if (randomBlock == 6) { thirdRow[i].createYellowBlock(layer, point); thirdRow[i].setType(6); thirdRow[i].setRotation(randomRotation); }
}

void Grid::bulletCollision(Player *player, cocos2d::CCLayerColor *layer)
{
	Rect bulletBody(player->getBullet()->getBoundingBox());

	for (int i = 0; i < firstRow.size(); i++) // First Row
	{
		Rect blockBody(firstRow[i].getBlock()->getBoundingBox());

		if (bulletBody.intersectsRect(blockBody))
		{
			auto getRotation = firstRow[i].getRotation();
			auto getType = firstRow[i].getType();

			firstRow[i].removeImage(layer);
			firstRow.erase(firstRow.begin() + i);
			firstRow.resize(16);
			createFirstBlocks(layer, 15);
			newBlockFirst = true;
			player->setFired(false);
			fillGrid(layer, player->colUnder, getRotation, getType);
		}
	}

	for (int i = 0; i < secondRow.size(); i++) // Second Row
	{
		Rect blockBody(secondRow[i].getBlock()->getBoundingBox());

		if (bulletBody.intersectsRect(blockBody))
		{
			auto getRotation = secondRow[i].getRotation();
			auto getType = secondRow[i].getType();

			secondRow[i].removeImage(layer);
			secondRow.erase(secondRow.begin() + i);
			secondRow.resize(16);
			createSecondBlocks(layer, 15);
			newBlockSecond = true;
			player->setFired(false);
			fillGrid(layer, player->colUnder, getRotation, getType);
		}
	}

	for (int i = 0; i < thirdRow.size(); i++) // Third Row
	{
		Rect blockBody(thirdRow[i].getBlock()->getBoundingBox());

		if (bulletBody.intersectsRect(blockBody))
		{
			auto getRotation = thirdRow[i].getRotation();
			auto getType = thirdRow[i].getType();

			thirdRow[i].removeImage(layer);
			thirdRow.erase(thirdRow.begin() + i);
			thirdRow.resize(16);
			createThirdBlocks(layer, 15);
			newBlockThird = true;
			player->setFired(false);
			fillGrid(layer, player->colUnder, getRotation, getType);
		}
	}
}

void Grid::fillGrid(cocos2d::CCLayerColor *layer, int colUnder, int rot, int typ)
{
	auto type = typ;
	auto rotation = rot;

	occupiedRowOfCols.clear();

	offsetCol = colUnder;
	std::vector<cocos2d::Vec2> blockPositions;

	cocos2d::Color3B colour;

	ptr->m_tetrisScore += 5;

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/collision.wav");

#pragma region Blue
	if (type == 0)
	{
		colour = cocos2d::Color3B::BLUE;
		if (rotation == 1)
		{
			if (offsetCol >= 7)
			{
				auto difference = 7 - offsetCol;
				offsetCol += difference;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -1);
			blockTwoPos = cocos2d::Vec2(offsetCol, -2);
			blockThreePos = cocos2d::Vec2(offsetCol + 1, -2);
			blockFourPos = cocos2d::Vec2(offsetCol + 2, -2);

			for (int i = 0; i < 3; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}

		else if (rotation == 2)
		{
			if (offsetCol == 9)
			{
				offsetCol -= 1;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -3);
			blockTwoPos = cocos2d::Vec2(offsetCol + 1, -1);
			blockThreePos = cocos2d::Vec2(offsetCol + 1, -2);
			blockFourPos = cocos2d::Vec2(offsetCol + 1, -3);

			for (int i = 0; i < 2; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}

		else if (rotation == 3)
		{
			if (offsetCol >= 7)
			{
				auto difference = 7 - offsetCol;
				offsetCol += difference;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -1);
			blockTwoPos = cocos2d::Vec2(offsetCol + 1, -1);
			blockThreePos = cocos2d::Vec2(offsetCol + 2, -1);
			blockFourPos = cocos2d::Vec2(offsetCol + 2, -2);

			for (int i = 0; i < 3; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}

		else if (rotation == 4)
		{
			if (offsetCol == 9)
			{
				offsetCol -= 1;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -1);
			blockTwoPos = cocos2d::Vec2(offsetCol, -2);
			blockThreePos = cocos2d::Vec2(offsetCol, -3);
			blockFourPos = cocos2d::Vec2(offsetCol + 1, -1);

			for (int i = 0; i < 2; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}
	}
#pragma endregion

#pragma region Green
	else if (type == 1)
	{
		colour = cocos2d::Color3B::GREEN;

		if (rotation == 3){ rotation = 1; }
		if (rotation == 4){ rotation = 2; }

		if (rotation == 1)
		{
			if (offsetCol == 9)
			{
				offsetCol -= 1;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -2);
			blockTwoPos = cocos2d::Vec2(offsetCol, -3);
			blockThreePos = cocos2d::Vec2(offsetCol + 1, -1);
			blockFourPos = cocos2d::Vec2(offsetCol + 1, -2);

			for (int i = 0; i < 2; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}

		else if (rotation == 2)
		{
			if (offsetCol >= 7)
			{
				auto difference = 7 - offsetCol;
				offsetCol += difference;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -1);
			blockTwoPos = cocos2d::Vec2(offsetCol + 1, -1);
			blockThreePos = cocos2d::Vec2(offsetCol + 1, -2);
			blockFourPos = cocos2d::Vec2(offsetCol + 2, -2);

			for (int i = 0; i < 3; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}
	}
#pragma endregion

#pragma region Orange
	else if (type == 2)
	{
		colour = cocos2d::Color3B::ORANGE;
		if (rotation == 1)
		{
			if (offsetCol >= 7)
			{
				auto difference = 7 - offsetCol;
				offsetCol += difference;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -1);
			blockTwoPos = cocos2d::Vec2(offsetCol, -2);
			blockThreePos = cocos2d::Vec2(offsetCol + 1, -1);
			blockFourPos = cocos2d::Vec2(offsetCol + 2, -1);

			for (int i = 0; i < 3; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}

		else if (rotation == 2)
		{
			if (offsetCol == 9)
			{
				offsetCol -= 1;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -1);
			blockTwoPos = cocos2d::Vec2(offsetCol, -2);
			blockThreePos = cocos2d::Vec2(offsetCol, -3);
			blockFourPos = cocos2d::Vec2(offsetCol + 1, -3);

			for (int i = 0; i < 2; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}

		else if (rotation == 3)
		{
			if (offsetCol >= 7)
			{
				auto difference = 7 - offsetCol;
				offsetCol += difference;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -2);
			blockTwoPos = cocos2d::Vec2(offsetCol + 1, -2);
			blockThreePos = cocos2d::Vec2(offsetCol + 2, -1);
			blockFourPos = cocos2d::Vec2(offsetCol + 2, -2);

			for (int i = 0; i < 3; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}

		else if (rotation == 4)
		{
			if (offsetCol == 9)
			{
				offsetCol -= 1;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -1);
			blockTwoPos = cocos2d::Vec2(offsetCol + 1, -1);
			blockThreePos = cocos2d::Vec2(offsetCol + 1, -2);
			blockFourPos = cocos2d::Vec2(offsetCol + 1, -3);

			for (int i = 0; i < 2; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}
	}
#pragma endregion

#pragma region Purple
	else if (type == 3)
	{
		colour = cocos2d::Color3B::MAGENTA;

		if (rotation == 1)
		{
			if (offsetCol == 9)
			{
				offsetCol -= 1;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -1);
			blockTwoPos = cocos2d::Vec2(offsetCol, -2);
			blockThreePos = cocos2d::Vec2(offsetCol, -3);
			blockFourPos = cocos2d::Vec2(offsetCol + 1, -2);

			for (int i = 0; i < 2; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}

		else if (rotation == 2)
		{
			if (offsetCol >= 7)
			{
				auto difference = 7 - offsetCol;
				offsetCol += difference;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -2);
			blockTwoPos = cocos2d::Vec2(offsetCol + 1, -1);
			blockThreePos = cocos2d::Vec2(offsetCol + 1, -2);
			blockFourPos = cocos2d::Vec2(offsetCol + 2, -2);

			for (int i = 0; i < 3; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}

		else if (rotation == 3)
		{
			if (offsetCol == 9)
			{
				offsetCol -= 1;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -2);
			blockTwoPos = cocos2d::Vec2(offsetCol + 1, -1);
			blockThreePos = cocos2d::Vec2(offsetCol + 1, -2);
			blockFourPos = cocos2d::Vec2(offsetCol + 1, -3);

			for (int i = 0; i < 2; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}

		else if (rotation == 4)
		{
			if (offsetCol >= 7)
			{
				auto difference = 7 - offsetCol;
				offsetCol += difference;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -1);
			blockTwoPos = cocos2d::Vec2(offsetCol + 1, -1);
			blockThreePos = cocos2d::Vec2(offsetCol + 1, -2);
			blockFourPos = cocos2d::Vec2(offsetCol + 2, -1);

			for (int i = 0; i < 3; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}
	}
#pragma endregion

#pragma region Red
	else if (type == 4)
	{
		colour = cocos2d::Color3B::RED;

		if (rotation == 3){ rotation = 1; }
		if (rotation == 4){ rotation = 2; }

		if (rotation == 1)
		{
			if (offsetCol == 9)
			{
				offsetCol -= 1;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -1);
			blockTwoPos = cocos2d::Vec2(offsetCol, -2);
			blockThreePos = cocos2d::Vec2(offsetCol + 1, -2);
			blockFourPos = cocos2d::Vec2(offsetCol + 1, -3);

			for (int i = 0; i < 2; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}

		else if (rotation == 2)
		{
			if (offsetCol >= 7)
			{
				auto difference = 7 - offsetCol;
				offsetCol += difference;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -2);
			blockTwoPos = cocos2d::Vec2(offsetCol + 1, -1);
			blockThreePos = cocos2d::Vec2(offsetCol + 1, -2);
			blockFourPos = cocos2d::Vec2(offsetCol + 2, -1);

			for (int i = 0; i < 3; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}
	}
#pragma endregion

#pragma region Turq
	else if (type == 5)
	{
		colour = cocos2d::Color3B(0, 255, 255); //Turq

		if (rotation == 3){ rotation = 1; }
		if (rotation == 4){ rotation = 2; }

		if (rotation == 1)
		{
			blockOnePos = cocos2d::Vec2(offsetCol, -1);
			blockTwoPos = cocos2d::Vec2(offsetCol, -2);
			blockThreePos = cocos2d::Vec2(offsetCol, -3);
			blockFourPos = cocos2d::Vec2(offsetCol, -4);

			for (int i = 0; i < 1; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}

		else if (rotation == 2)
		{
			if (offsetCol >= 6)
			{
				auto difference = 6 - offsetCol;
				offsetCol += difference;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -1);
			blockTwoPos = cocos2d::Vec2(offsetCol + 1, -1);
			blockThreePos = cocos2d::Vec2(offsetCol + 2, -1);
			blockFourPos = cocos2d::Vec2(offsetCol + 3, -1);

			for (int i = 0; i < 4; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}
	}
#pragma endregion

#pragma region Yellow
	else if (type == 6)
	{
		colour = cocos2d::Color3B::YELLOW;
		if (rotation == 2 || rotation == 3 || rotation == 4){ rotation = 1; }

		if (rotation == 1)
		{
			if (offsetCol == 9)
			{
				offsetCol -= 1;
			}

			blockOnePos = cocos2d::Vec2(offsetCol, -1);
			blockTwoPos = cocos2d::Vec2(offsetCol, -2);
			blockThreePos = cocos2d::Vec2(offsetCol + 1, -1);
			blockFourPos = cocos2d::Vec2(offsetCol + 1, -2);

			for (int i = 0; i < 2; i++)
			{
				occupiedRowOfCols.push_back(offsetCol + i);
			}
		}
	}
#pragma endregion

	blockPositions.push_back(blockOnePos);
	blockPositions.push_back(blockTwoPos);
	blockPositions.push_back(blockThreePos);
	blockPositions.push_back(blockFourPos);

	int lowest = 100000;
	bool tileOccupied = false;

	for (int col = 0; col < blockPositions.size(); col++)
	{
		for (int row = 0; row < 10; row++)
		{
			if (playerTiles[occupiedRowOfCols[blockPositions[col].x - offsetCol]][row].getFilled() == true)
			{
				if (lowest > row - blockPositions[col].y - 1)
				{
					lowest = row - blockPositions[col].y - 1;
					tileOccupied = true;
				}
			}
		}
	}

	if (tileOccupied == false)
	{
		lowest = 10;
	}

	if (lowest <= 1)
	{
		punishment(layer);

		for (int col = 0; col < 10; col++)
		{
			for (int row = 0; row < 10; row++)
			{
				if (playerTiles[col][row].getFilled() == true)
				{
					punishment(layer);
				}
			}
		}
	}

	else
	{
		playerTiles[static_cast<int>(blockOnePos.x)][static_cast<int>(blockOnePos.y) + lowest].setFilled(true);
		playerTiles[static_cast<int>(blockTwoPos.x)][static_cast<int>(blockTwoPos.y) + lowest].setFilled(true);
		playerTiles[static_cast<int>(blockThreePos.x)][static_cast<int>(blockThreePos.y) + lowest].setFilled(true);
		playerTiles[static_cast<int>(blockFourPos.x)][static_cast<int>(blockFourPos.y) + lowest].setFilled(true);


		drawFull(layer, blockOnePos.x, blockOnePos.y + lowest, colour);
		drawFull(layer, blockTwoPos.x, blockTwoPos.y + lowest, colour);
		drawFull(layer, blockThreePos.x, blockThreePos.y + lowest, colour);
		drawFull(layer, blockFourPos.x, blockFourPos.y + lowest, colour);

		clearLine(layer, colour);
	}
}

void Grid::drawFull(cocos2d::CCLayerColor *layer, int col, int row, cocos2d::Color3B colour)
{
	if (playerTiles[col][row].getFilled() == true)
	{
		fullPlayerTile[col][row] = cocos2d::Sprite::create("Grid/Grid Full.png");
		fullPlayerTile[col][row]->setPosition(Point(playerTiles[col][row].getPlayerTilePosition()));
		fullPlayerTile[col][row]->setScale(Director::getInstance()->getContentScaleFactor());
		fullPlayerTile[col][row]->setColor(colour);
		fullPlayerTile[col][row]->setOpacity(175);
		layer->addChild(fullPlayerTile[col][row]);
	}
}


void Grid::clearLine(cocos2d::CCLayerColor *layer, cocos2d::Color3B colour)
{
	int clearZero = 0;
	int	clearOne = 0;
	int clearTwo = 0;
	int	clearThree = 0;
	int	clearFour = 0;
	int	clearFive = 0;
	int	clearSix = 0;
	int	clearSeven = 0;
	int	clearEight = 0;
	int	clearNine = 0;

#pragma region Check_tiles_per_row

	for (int col = 0; col < 10; col++)
	{
		if (playerTiles[col][9].getFilled() == true) { clearZero += 1; }
		if (playerTiles[col][8].getFilled() == true) { clearOne += 1; }
		if (playerTiles[col][7].getFilled() == true) { clearTwo += 1; }
		if (playerTiles[col][6].getFilled() == true) { clearThree += 1; }
		if (playerTiles[col][5].getFilled() == true) { clearFour += 1; }
		if (playerTiles[col][4].getFilled() == true) { clearFive += 1; }
		if (playerTiles[col][3].getFilled() == true) { clearSix += 1; }
		if (playerTiles[col][2].getFilled() == true) { clearSeven += 1; }
		if (playerTiles[col][1].getFilled() == true) { clearEight += 1; }
		if (playerTiles[col][0].getFilled() == true) { clearNine += 1; }
	}
#pragma endregion


#pragma region bottom_row

	if (clearZero >= 10)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/clear.wav");
		ptr->m_tetrisScore += 10;

		for (int col = 0; col < 10; col++)
		{
			playerTiles[col][9].setFilled(false);
			layer->removeChild(fullPlayerTile[col][9]);

			for (int row = 0; row < 10; row++)
			{
				if (playerTiles[col][row].getFilled() == true)
				{
					playerTiles[col][row].setFilled(false);
					layer->removeChild(fullPlayerTile[col][row]);

					if (row != 9)
					{
						playerTiles[col][row + 1].setFilled(true);
						drawFull(layer, col, row + 1, colour);
						row += 1;
					}
					else
					{
						playerTiles[col][row].setFilled(true);
						drawFull(layer, col, row, colour);
					}
				}
			}
		}
	}
#pragma endregion

#pragma region second_bottom_row

	if (clearOne >= 10)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/clear.wav");
		ptr->m_tetrisScore += 10;

		for (int col = 0; col < 10; col++)
		{
			playerTiles[col][8].setFilled(false);
			layer->removeChild(fullPlayerTile[col][8]);

			for (int row = 0; row < 9; row++)
			{
				if (playerTiles[col][row].getFilled() == true)
				{
					playerTiles[col][row].setFilled(false);
					layer->removeChild(fullPlayerTile[col][row]);

					if (row != 9)
					{
						playerTiles[col][row + 1].setFilled(true);
						drawFull(layer, col, row + 1, colour);
						row += 1;
					}
					else
					{
						playerTiles[col][row].setFilled(true);
						drawFull(layer, col, row, colour);
					}
				}
			}
		}
	}
#pragma endregion

#pragma region third_bottom_row

	if (clearTwo >= 10)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/clear.wav");
		ptr->m_tetrisScore += 10;

		for (int col = 0; col < 10; col++)
		{
			playerTiles[col][7].setFilled(false);
			layer->removeChild(fullPlayerTile[col][7]);

			for (int row = 0; row < 8; row++)
			{
				if (playerTiles[col][row].getFilled() == true)
				{
					playerTiles[col][row].setFilled(false);
					layer->removeChild(fullPlayerTile[col][row]);

					if (row != 9)
					{
						playerTiles[col][row + 1].setFilled(true);
						drawFull(layer, col, row + 1, colour);
						row += 1;
					}
					else
					{
						playerTiles[col][row].setFilled(true);
						drawFull(layer, col, row, colour);
					}
				}
			}
		}
	}
#pragma endregion

#pragma region fourth_bottom_row

	if (clearThree >= 10)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/clear.wav");
		ptr->m_tetrisScore += 10;

		for (int col = 0; col < 10; col++)
		{
			playerTiles[col][6].setFilled(false);
			layer->removeChild(fullPlayerTile[col][6]);

			for (int row = 0; row < 7; row++)
			{
				if (playerTiles[col][row].getFilled() == true)
				{
					playerTiles[col][row].setFilled(false);
					layer->removeChild(fullPlayerTile[col][row]);

					if (row != 9)
					{
						playerTiles[col][row + 1].setFilled(true);
						drawFull(layer, col, row + 1, colour);
						row += 1;
					}
					else
					{
						playerTiles[col][row].setFilled(true);
						drawFull(layer, col, row, colour);
					}
				}
			}
		}
	}
#pragma endregion

#pragma region fifth_bottom_row

	if (clearFour >= 10)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/clear.wav");
		ptr->m_tetrisScore += 10;

		for (int col = 0; col < 10; col++)
		{
			playerTiles[col][5].setFilled(false);
			layer->removeChild(fullPlayerTile[col][5]);

			for (int row = 0; row < 6; row++)
			{
				if (playerTiles[col][row].getFilled() == true)
				{
					playerTiles[col][row].setFilled(false);
					layer->removeChild(fullPlayerTile[col][row]);

					if (row != 9)
					{
						playerTiles[col][row + 1].setFilled(true);
						drawFull(layer, col, row + 1, colour);
						row += 1;
					}
					else
					{
						playerTiles[col][row].setFilled(true);
						drawFull(layer, col, row, colour);
					}
				}
			}
		}
	}
#pragma endregion

#pragma region fifth_from_top

	if (clearFive >= 10)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/clear.wav");
		ptr->m_tetrisScore += 10;

		for (int col = 0; col < 10; col++)
		{
			playerTiles[col][4].setFilled(false);
			layer->removeChild(fullPlayerTile[col][4]);

			for (int row = 0; row < 5; row++)
			{
				if (playerTiles[col][row].getFilled() == true)
				{
					playerTiles[col][row].setFilled(false);
					layer->removeChild(fullPlayerTile[col][row]);

					if (row != 9)
					{
						playerTiles[col][row + 1].setFilled(true);
						drawFull(layer, col, row + 1, colour);
						row += 1;
					}
					else
					{
						playerTiles[col][row].setFilled(true);
						drawFull(layer, col, row, colour);
					}
				}
			}
		}
	}
#pragma endregion

#pragma region fourth_from_top

	if (clearSix >= 10)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/clear.wav");
		ptr->m_tetrisScore += 10;

		for (int col = 0; col < 10; col++)
		{
			playerTiles[col][3].setFilled(false);
			layer->removeChild(fullPlayerTile[col][3]);

			for (int row = 0; row < 4; row++)
			{
				if (playerTiles[col][row].getFilled() == true)
				{
					playerTiles[col][row].setFilled(false);
					layer->removeChild(fullPlayerTile[col][row]);

					if (row != 9)
					{
						playerTiles[col][row + 1].setFilled(true);
						drawFull(layer, col, row + 1, colour);
						row += 1;
					}
					else
					{
						playerTiles[col][row].setFilled(true);
						drawFull(layer, col, row, colour);
					}
				}
			}
		}
	}
#pragma endregion

#pragma region third_from_top

	if (clearSeven >= 10)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/clear.wav");
		ptr->m_tetrisScore += 10;

		for (int col = 0; col < 10; col++)
		{
			playerTiles[col][2].setFilled(false);
			layer->removeChild(fullPlayerTile[col][2]);

			for (int row = 0; row < 3; row++)
			{
				if (playerTiles[col][row].getFilled() == true)
				{
					playerTiles[col][row].setFilled(false);
					layer->removeChild(fullPlayerTile[col][row]);

					if (row != 9)
					{
						playerTiles[col][row + 1].setFilled(true);
						drawFull(layer, col, row + 1, colour);
						row += 1;
					}
					else
					{
						playerTiles[col][row].setFilled(true);
						drawFull(layer, col, row, colour);
					}
				}
			}
		}
	}
#pragma endregion

#pragma region second_from_top

	if (clearEight >= 10)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/clear.wav");
		ptr->m_tetrisScore += 10;

		for (int col = 0; col < 10; col++)
		{
			playerTiles[col][1].setFilled(false);
			layer->removeChild(fullPlayerTile[col][1]);

			for (int row = 0; row < 2; row++)
			{
				if (playerTiles[col][row].getFilled() == true)
				{
					playerTiles[col][row].setFilled(false);
					layer->removeChild(fullPlayerTile[col][row]);

					if (row != 9)
					{
						playerTiles[col][row + 1].setFilled(true);
						drawFull(layer, col, row + 1, colour);
						row += 1;
					}
					else
					{
						playerTiles[col][row].setFilled(true);
						drawFull(layer, col, row, colour);
					}
				}
			}
		}
	}
#pragma endregion

#pragma region top

	if (clearNine >= 10)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/clear.wav");
		ptr->m_tetrisScore += 10;

		for (int col = 0; col < 10; col++)
		{
			playerTiles[col][0].setFilled(false);
			layer->removeChild(fullPlayerTile[col][0]);

			for (int row = 0; row < 1; row++)
			{
				if (playerTiles[col][row].getFilled() == true)
				{
					playerTiles[col][row].setFilled(false);
					layer->removeChild(fullPlayerTile[col][row]);

					if (row != 9)
					{
						playerTiles[col][row + 1].setFilled(true);
						drawFull(layer, col, row + 1, colour);
						row += 1;
					}
					else
					{
						playerTiles[col][row].setFilled(true);
						drawFull(layer, col, row, colour);
					}
				}
			}
		}
	}
#pragma endregion

}

//void Grid::clearLine(cocos2d::CCLayerColor *layer, cocos2d::Color3B colour)
//{
//	int clearZero = 0;
//	int	clearOne = 0;
//	int clearTwo = 0;
//	int	clearThree = 0;
//	int	clearFour = 0;
//	int	clearFive = 0;
//	int	clearSix = 0;
//	int	clearSeven = 0;
//	int	clearEight = 0;
//	int	clearNine = 0;
//
//#pragma region Check_tiles_per_row
//
//	for (int col = 0; col < 10; col++)
//	{
//		if (playerTiles[col][9].getFilled() == true) { clearZero += 1; }
//		if (playerTiles[col][8].getFilled() == true) { clearOne += 1; }
//		if (playerTiles[col][7].getFilled() == true) { clearTwo += 1; }
//		if (playerTiles[col][6].getFilled() == true) { clearThree += 1; }
//		if (playerTiles[col][5].getFilled() == true) { clearFour += 1; }
//		if (playerTiles[col][4].getFilled() == true) { clearFive += 1; }
//		if (playerTiles[col][3].getFilled() == true) { clearSix += 1; }
//		if (playerTiles[col][2].getFilled() == true) { clearSeven += 1; }
//		if (playerTiles[col][1].getFilled() == true) { clearEight += 1; }
//		if (playerTiles[col][0].getFilled() == true) { clearNine += 1; }
//	}
//#pragma endregion
//
//#pragma region clear
//
//	if (clearZero >= 10)
//	{
//		ptr->m_tetrisScore += 10;
//		for (int col = 0; col < 10; col++)
//		{
//			playerTiles[col][9].setFilled(false);
//			layer->removeChild(fullPlayerTile[col][9]);
//
//			for (int row = 0; row < 10; row++)
//			{
//				if (playerTiles[col][row].getFilled() == true)
//				{
//					playerTiles[col][row].setFilled(false);
//					layer->removeChild(fullPlayerTile[col][row]);
//
//					if (row == 9)
//					{
//						playerTiles[col][row].setFilled(true);
//						drawFull(layer, col, row, colour);
//					}
//					else
//					{
//						playerTiles[col][row + 1].setFilled(true);
//						drawFull(layer, col, row + 1, colour);
//						row += 1;
//					}
//				}
//			}
//		}
//	}
//	if (clearOne >= 10)
//	{
//		ptr->m_tetrisScore += 10;
//		for (int col = 0; col < 10; col++)
//		{
//			playerTiles[col][8].setFilled(false);
//			layer->removeChild(fullPlayerTile[col][8]);
//		}
//	}
//	if (clearTwo >= 10)
//	{
//		ptr->m_tetrisScore += 10;
//		for (int col = 0; col < 10; col++)
//		{
//			playerTiles[col][7].setFilled(false);
//			layer->removeChild(fullPlayerTile[col][7]);
//		}
//	}
//	if (clearThree >= 10)
//	{
//		ptr->m_tetrisScore += 10;
//		for (int col = 0; col < 10; col++)
//		{
//			playerTiles[col][6].setFilled(false);
//			layer->removeChild(fullPlayerTile[col][6]);
//		}
//	}
//	if (clearFour >= 10)
//	{
//		ptr->m_tetrisScore += 10;
//		for (int col = 0; col < 10; col++)
//		{
//			playerTiles[col][5].setFilled(false);
//			layer->removeChild(fullPlayerTile[col][5]);
//		}
//	}
//	if (clearFive >= 10)
//	{
//		ptr->m_tetrisScore += 10;
//		for (int col = 0; col < 10; col++)
//		{
//			playerTiles[col][4].setFilled(false);
//			layer->removeChild(fullPlayerTile[col][4]);
//		}
//	}
//	if (clearSix >= 10)
//	{
//		ptr->m_tetrisScore += 10;
//		for (int col = 0; col < 10; col++)
//		{
//			playerTiles[col][3].setFilled(false);
//			layer->removeChild(fullPlayerTile[col][3]);
//		}
//	}
//	if (clearSeven >= 10)
//	{
//		ptr->m_tetrisScore += 10;
//		for (int col = 0; col < 10; col++)
//		{
//			playerTiles[col][2].setFilled(false);
//			layer->removeChild(fullPlayerTile[col][2]);
//		}
//	}
//	if (clearEight >= 10)
//	{
//		ptr->m_tetrisScore += 10;
//		for (int col = 0; col < 10; col++)
//		{
//			playerTiles[col][1].setFilled(false);
//			layer->removeChild(fullPlayerTile[col][1]);
//		}
//	}
//	if (clearNine >= 10)
//	{
//		ptr->m_tetrisScore += 10;
//		for (int col = 0; col < 10; col++)
//		{
//			playerTiles[col][0].setFilled(false);
//			layer->removeChild(fullPlayerTile[col][0]);
//		}
//	}
//
//
//
//#pragma endregion
//}