#include "Terrain.h"



Terrain::Terrain()
{
	
	init();
}


Terrain::~Terrain()
{
}

void Terrain::init()
{
	centerPoints = new sf::Vertex[MAP_WIDTH_POINTS];
	leftBorderPoints = new sf::Vertex[HALF_SCREEN_WIDTH_POINTS];
	rightBorderPoints = new sf::Vertex[HALF_SCREEN_WIDTH_POINTS];

	pointsOnMap = new sf::Vertex[FULL_WIDTH_POINTS];

	generateTerrainCenter();
	generateTerrainSides();
	combineTerrain();
}

void Terrain::update()
{

}

void Terrain::draw(sf::RenderWindow &window)
{
	window.draw(pointsOnMap, FULL_WIDTH_POINTS, sf::LinesStrip);
}

sf::Vertex* Terrain::getPoints()
{
	return  pointsOnMap;
}

void Terrain::generateTerrainCenter()
{
	int previousY = 720;
	bool goingUp = true;
	int maxCount = rand() % 32;
	int buffer = 64;

	for (int i = 0; i < MAP_WIDTH_POINTS; i++)
	{
		if (goingUp == true)
		{
			centerPoints[i] = sf::Vertex(sf::Vector2f(i * 10, previousY - rand() % 10));
			previousY = centerPoints[i].position.y;
			maxCount--;

			if (centerPoints[i].position.y <= 600) // Start - 120
			{
				maxCount = 0;
			}

			if (i >= MAP_WIDTH_POINTS - buffer)
			{
				if (centerPoints[i].position.y <= 720 - buffer) // Limit to start point
				{
					maxCount = 0;
				}
			}
		}

		if (goingUp == false)
		{
			centerPoints[i] = sf::Vertex(sf::Vector2f(i * 10, previousY + rand() % 10));
			previousY = centerPoints[i].position.y;
			maxCount--;

			if (centerPoints[i].position.y >= 840) // Start + 120
			{
				maxCount = 0;
			}

			if (i >= MAP_WIDTH_POINTS - buffer)
			{
				if (centerPoints[i].position.y >= 720 + buffer) // Limit to start point
				{
					maxCount = 0;
				}
			}
		}

		if (maxCount == 0)
		{
			maxCount = rand() % 32;
			goingUp = !goingUp;
		}

		if (i == MAP_WIDTH_POINTS)
		{
			centerPoints[i].position.y = centerPoints[0].position.y;
		}
	}

	centerPoints[MAP_WIDTH_POINTS - 1].position.y = centerPoints[0].position.y;
}

void Terrain::generateTerrainSides()
{
	for (int i = 0; i < HALF_SCREEN_WIDTH_POINTS; i++)
	{
		leftBorderPoints[i] = sf::Vertex(sf::Vector2f(
			-960 + (i * 10) + 10, 
			centerPoints[MAP_WIDTH_POINTS - HALF_SCREEN_WIDTH_POINTS + i].position.y)
		);

		rightBorderPoints[i] = sf::Vertex(sf::Vector2f(
			(MAP_WIDTH_PIXEL) + (i * 10) - 10,
			centerPoints[i].position.y)
		);
	}
}

void Terrain::combineTerrain()
{
	for (int i = 0; i < FULL_WIDTH_POINTS; i++)
	{
		if (i < HALF_SCREEN_WIDTH_POINTS)
		{
			pointsOnMap[i] = sf::Vertex(sf::Vector2f(
				leftBorderPoints[i].position.x,
				leftBorderPoints[i].position.y)
			);
		}

		else if (i >= HALF_SCREEN_WIDTH_POINTS && i < FULL_WIDTH_POINTS - HALF_SCREEN_WIDTH_POINTS)
		{
			pointsOnMap[i] = sf::Vertex(sf::Vector2f(
				centerPoints[i - HALF_SCREEN_WIDTH_POINTS].position.x,
				centerPoints[i - HALF_SCREEN_WIDTH_POINTS].position.y)
			);
		}

		else if (i >= FULL_WIDTH_POINTS - HALF_SCREEN_WIDTH_POINTS)
		{
			pointsOnMap[i] = sf::Vertex(sf::Vector2f(
				rightBorderPoints[i - MAP_WIDTH_POINTS - HALF_SCREEN_WIDTH_POINTS].position.x,
				rightBorderPoints[i - MAP_WIDTH_POINTS - HALF_SCREEN_WIDTH_POINTS].position.y)
			);
		}
	}

	// Clear spaces
	delete centerPoints;
	delete leftBorderPoints;
	delete rightBorderPoints;
}

