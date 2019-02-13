#pragma once
#define SDL_MAIN_HANDLED
#ifdef __APPLE__

#elif defined(_WIN64) || defined(_WIN32)

#endif

#include "SFML/Graphics.hpp"
#include <vector>



class CollisionManager {

public:
	CollisionManager();
	~CollisionManager();
	static CollisionManager* instance();
	bool CircleDetectCollision(sf::CircleShape &s1, sf::CircleShape &s2);
	bool CheckRange(float range, sf::Vector2f object1, sf::Vector2f object2);
	bool RectangleCollision(sf::RectangleShape s1, sf::RectangleShape s2);
	float dotProduct(sf::Vector2f v1, sf::Vector2f v2);
	sf::Vector2f NormaliseVector(sf::Vector2f v1);

private:
	static CollisionManager* _instance;

};
