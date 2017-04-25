//Frankie Lyons
//C00177077
//SAT Assignment

#include "Quadtree.h"

Quadtree::Quadtree(float x, float y, float width, float height, int level, int maxLevel) :
m_x(x),
m_y(y),
m_width(width),
m_height(height),
m_level(level),
m_maxLevel(maxLevel),
topLeft(nullptr),
topRight(nullptr),
bottomLeft(nullptr),
bottomRight(nullptr),
m_maxObjects(5)
{
	// draws a square grid
	shape.setPosition(x, y);
	shape.setSize(sf::Vector2f(width, height));
	shape.setFillColor(sf::Color(0, 0, 0, 0));
	shape.setOutlineThickness(1.0f);
	shape.setOutlineColor(sf::Color(255, 255, 255));
}

void Quadtree::Update(sf::RenderWindow* window, Objects ** object)
{
	m_objects = object;

	if (topRight != nullptr)
	{
		topRight->Update(window, object);
		topLeft->Update(window, object);
		bottomRight->Update(window, object);
		bottomLeft->Update(window, object);
	}

	window->draw(shape);

	if (m_objects != nullptr)
	{
		quad = true; // collision = true
	}

	else
	{
		quad = false;
	}
}

void Quadtree::Clear()
{
	if (topLeft != nullptr)
	{
		topRight->Clear();
		topLeft->Clear();
		bottomRight->Clear();
		bottomLeft->Clear();
	}


	if (topLeft != nullptr)
	{
		delete topRight;
		delete topLeft;
		delete bottomRight;
		delete bottomLeft;
	}

	m_objects = nullptr;
	topRight = nullptr;
	topLeft = nullptr;
	bottomRight = nullptr;
	bottomLeft = nullptr;
}

void Quadtree::AddObject(Objects** object, int numShapes) 
{ // returns if objects are contined within

	if (numShapes > m_maxObjects && m_level <= m_maxLevel)
	{
		split(object, numShapes);
	}

	else if (numShapes == 0)
	{
		m_objects = nullptr;
	}

	else
	{
		m_objects = object;
		m_numberOfShapes = numShapes;
	}
}

void Quadtree::split(Objects** object, int maxShapes)
{
	int TLNum = 0;
	int TRNum = 0;
	int BLNum = 0;
	int BRNum = 0;

	//maintains a list of indices of which shapes are in each new quads bounds
	std::vector<int> TLList, TRList, BLList, BRList;

	float topLeftX = m_x;
	float topLeftY = m_y;

	float topRightX = m_x + m_width / 2.0f;
	float topRightY = m_y;

	float bottomLeftX = m_x;
	float bottomLeftY = m_y + m_height / 2.0f;

	float bottomRightX = m_x + m_width / 2.0f;
	float bottomRightY = m_y + m_height / 2.0f;

	float width = m_width / 2.0f;
	float height = m_height / 2.0f;

	topLeft = new Quadtree(topLeftX, topLeftY, width, height, m_level + 1, m_maxLevel);
	topRight = new Quadtree(topRightX, topRightY, width, height, m_level + 1, m_maxLevel);
	bottomLeft = new Quadtree(bottomLeftX, bottomLeftY, width, height, m_level + 1, m_maxLevel);
	bottomRight = new Quadtree(bottomRightX, bottomRightY, width, height, m_level + 1, m_maxLevel);

	for (int i = 0; i < maxShapes; i++)
	{
		if (object[i]->getPosition().x > topLeftX - object[i]->getSize() && object[i]->getPosition().x < topLeftX + width + object[i]->getSize() &&
			object[i]->getPosition().y > topLeftY - object[i]->getSize() && object[i]->getPosition().y < topLeftY + height + object[i]->getSize())
		{
			TLNum++;
			TLList.push_back(i);
		}

		if (object[i]->getPosition().x > topRightX - object[i]->getSize() && object[i]->getPosition().x < topRightX + width + object[i]->getSize() &&
			object[i]->getPosition().y > topRightY - object[i]->getSize() && object[i]->getPosition().y < topRightY + height + object[i]->getSize())
		{
			TRNum++;
			TRList.push_back(i);
		}

		if (object[i]->getPosition().x > bottomLeftX - object[i]->getSize() && object[i]->getPosition().x < bottomLeftX + width + object[i]->getSize() &&
			object[i]->getPosition().y > bottomLeftY - object[i]->getSize() && object[i]->getPosition().y < bottomLeftY + height + object[i]->getSize())
		{
			BLNum++;
			BLList.push_back(i);
		}

		if (object[i]->getPosition().x > bottomRightX - object[i]->getSize() && object[i]->getPosition().x < bottomRightX + width + object[i]->getSize() &&
			object[i]->getPosition().y > bottomRightY - object[i]->getSize() && object[i]->getPosition().y < bottomRightY + height + object[i]->getSize())
		{
			BRNum++;
			BRList.push_back(i);
		}
	}

	topLeft->AddObject(createShapeArray(TLNum, TLList, object), TLNum);
	topRight->AddObject(createShapeArray(TRNum, TRList, object), TRNum);
	bottomLeft->AddObject(createShapeArray(BLNum, BLList, object), BLNum);
	bottomRight->AddObject(createShapeArray(BRNum, BRList, object), BRNum);
}

Objects** Quadtree::createShapeArray(int numIndices, std::vector<int> indices, Objects** objects)
{
	const int numShapes = numIndices;

	Objects** object = new Objects* [numShapes];

	for (int i = 0; i < numShapes; i++)
	{
		object[i] = objects[indices[i]];
	}

	return object;
}