//Frankie Lyons
//C00177077
//SAT Assignment

#pragma once

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>
#include "Objects.h"

class SATCollisions
{
public:
	SATCollisions();
	~SATCollisions();

	float dotProduct(sf::Vector2f, sf::Vector2f);
	sf::Vector2f projectAxis(Objects*, sf::Vector2f);
	bool gapDetection(Objects*, Objects*); // algorithem for finding smallest point value and biggest point value for both shapes
	void SATResolve(Objects* , Objects*, sf::Vector2f, float);

private:
	// Find axis
	//sf::Vector2f axis;
	//sf::Vector2f normal;

	//// SAT
	//sf::Vector2f vectorOffset;
	//sf::Vector2f normalAxis;

	//sf::Vector2f translationAxis;

	//// Response
	//float mtd;
	//sf::Vector2f mtv;

	
};

