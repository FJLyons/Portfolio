//Frankie Lyons
//C00177077
//SAT Assignment

#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Objects.h"
#include "CollisionDetection.h"
#include <iostream>
#include <sstream>

using namespace std;

class Quadtree 
{
public:
	Quadtree();
	Quadtree(float x, float y, float width, float height, int level, int maxLevel);
	void Clear();

	void Update(sf::RenderWindow * window, Objects** object);
	void AddObject(Objects** object, int maxShapes); 

	Objects** createShapeArray(int numIndices, std::vector<int> indices, Objects** objects);

	bool quad;

private:
	// Bounding box for QT
	float m_x;
	float m_y;
	float m_width;
	float m_height;

	int	m_level;
	const int	m_maxLevel;

	int m_numberOfShapes;
	Objects** m_objects;
	const int m_maxObjects;

	Quadtree* topLeft;
	Quadtree* topRight;
	Quadtree* bottomLeft;
	Quadtree* bottomRight;

	sf::RectangleShape shape;

	void split(Objects** objects, int maxShapes);
};

#endif

