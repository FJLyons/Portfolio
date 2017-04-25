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

class CollisionDetection
{
public:
	CollisionDetection();
	~CollisionDetection();

	bool borderCollision(Objects*);

	bool circleCollision(Objects*, Objects*);
	void circleResolve(Objects*, Objects*);
};

