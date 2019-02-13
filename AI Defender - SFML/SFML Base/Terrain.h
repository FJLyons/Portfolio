#pragma once

// SFML Headers
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h> 

#include "GlobalVariables.h"

class Terrain
{
public:
	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();

	Terrain();
	~Terrain();

	void init();
	void update();
	void draw(sf::RenderWindow &window);
	sf::Vertex* getPoints();
	//sf::Vector2f pointsOnMap[100]

private:
	sf::Vertex* centerPoints;
	sf::Vertex* leftBorderPoints;
	sf::Vertex* rightBorderPoints;

	sf::Vertex* pointsOnMap;

	void generateTerrainCenter();
	void generateTerrainSides();
	void combineTerrain();
};

