#ifndef INTERCEPTORMISSILE_H
#define INTERCEPTORMISSILE_H

#include "Bullet.h"
#include "GlobalVariables.h"
#include "CollisionManager.h"
#include "ResourceLoader.h"

class InterceptorMissile : public Bullet
{

private:
	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();

private:


public:

	 InterceptorMissile(sf::Vector2f pos);
	 bool Update(sf::RectangleShape  player);
	 void Draw(sf::RenderWindow &window);
};

#endif