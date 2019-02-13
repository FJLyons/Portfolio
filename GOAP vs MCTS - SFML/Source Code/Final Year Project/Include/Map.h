#pragma once

// SFML Headers
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "Manager_Resource.h"

class Map
{
private:
	// Avoid unwanted construction
	Map(const Map&) = delete;
	// Avoid unwanted assignment
	Map& operator=(const Map&) = delete;

public:
	// Create Map
	Map();
	// Destructor
	~Map();

	// Initialise Map
	void Init();
	// Update Map
	void Update();
	// Draw Map
	// @param window - SFML window
	void Draw(sf::RenderWindow &window);

private:
	// Map body - for drawing
	sf::RectangleShape m_rectangleShape;
};

