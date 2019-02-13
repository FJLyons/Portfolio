#pragma once
#ifndef MUTANT_H
#define MUTANT_H

#include "SFML\Graphics.hpp"
#include "CollisionManager.h"
#include <time.h>
#include "Bullet.h"
#include "ResourceLoader.h"
#include "Obstacles.h"
#include "Human.h"
#include "GlobalVariables.h"
#include "MutantBullet.h"
class Mutant
{


private:

	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();

	sf::Texture mTexture;
	sf::Texture bulletTexture;
	sf::Sprite mSprite;
	sf::Vector2f mPositon;
	sf::Vector2f velocity;
	bool canShoot;
	sf::Vector2f Direction;
	float shotdelay;
	float shotTimer;
	sf::Clock shotClock;
	sf::Vector2f seekPoint;
	sf::RectangleShape collisionRect;
	sf::RectangleShape FeildofView;
	enum Behaviour { Wander, Flock, Abduct };
	int neighbourRange;
	sf::Vector2f alignment;
	sf::Vector2f cohesion;
	sf::Vector2f seperation;
	sf::Vector2f obsticleseperation;
	float angle;
	int myIndex;
	int humanindex;
	bool abducting;

public:
	Mutant();
	~Mutant();
	Mutant(sf::Vector2f pos);

	void Wandering(std::vector<Mutant*>& mutants, std::vector<Obstacles*>& obstacles, sf::Vector2f playerPos);
	sf::Vector2f ComputeAlignment(std::vector<Mutant*>& mutants);
	sf::Vector2f ComputeCohesion(std::vector<Mutant*>& mutants);
	sf::Vector2f ComputeSeperation(std::vector<Mutant*>& mutants);
	sf::Vector2f ComputeObsticleSeperation(std::vector<Obstacles*>& obstacles);

	void Shoot(sf::Vector2f playerpos, sf::Vector2f playervel);
	void Update(std::vector<Mutant*>& mutants, int indexofCurrentMutant, std::vector<Obstacles*>& obstacles, sf::Vector2f playerPos, sf::Vector2f playerVel);
	void Draw(sf::RenderWindow &window);
	sf::Vector2f getVelocity();
	sf::Vector2f getPosition();
	sf::RectangleShape getRect();
	sf::RectangleShape getFOVRect();
	void setPosition(sf::Vector2f vec);

	int health = 1;

	std::vector<MutantBullet*> bullets;
};

#endif
