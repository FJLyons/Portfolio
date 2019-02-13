#pragma once
#ifndef ABDUCTOR_H
#define ABDUCTOR_H

#include "SFML\Graphics.hpp"
#include "CollisionManager.h"
#include <time.h>
#include "Bullet.h"
#include "ResourceLoader.h"
#include "Obstacles.h"
#include "Human.h"
#include "GlobalVariables.h"
#include "AbductorBullet.h"
class Abductor
{


private:

	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();

	sf::Texture mTexture;
	sf::Texture bulletTexture;
	sf::Sprite mSprite;
	sf::Vector2f mPositon;
	sf::Vector2f velocity;
	int currentBehaviour;
	float range;
	bool canShoot;


	sf::Vector2f Direction;
	float shotdelay;
	float shotTimer;
	sf::Clock shotClock;
	sf::Vector2f seekPoint;
	sf::RectangleShape collisionRect;
	sf::RectangleShape FeildofView;
	enum Behaviour { Wander, Abduct };
	int neighbourRange;
	sf::Vector2f alignment;
	sf::Vector2f cohesion;
	sf::Vector2f seperation;
	sf::Vector2f obsticleseperation;
	float angle;
	int myIndex;
	bool alive;
	int humanindex;
	bool abducting;

public:
	Abductor();
	~Abductor();
	Abductor(sf::Vector2f pos);

	sf::Vector2f FindNewPoint();
	void Wandering(std::vector<Abductor*>& abductors, std::vector<Obstacles*>& obstacles, std::vector<Human*>& humans);
	sf::Vector2f ComputeAlignment(std::vector<Abductor*>& abductors);
	sf::Vector2f ComputeCohesion(std::vector<Abductor*>& abductors);
	sf::Vector2f ComputeSeperation(std::vector<Abductor*>& abductors);
	sf::Vector2f ComputeObsticleSeperation(std::vector<Obstacles*>& obstacles);
	void CheckForHuman(std::vector<Human*>& humans);
	void Abducting(std::vector<Human*>& humans);
	void Update(std::vector<Abductor*>& abductors, int indexofCurrentAbductor, std::vector<Obstacles*>& obstacles, std::vector<Human*>& humans, sf::Vector2f playerpos);
	void Shoot(sf::Vector2f playerpos);
	void Draw(sf::RenderWindow &window);
	sf::Vector2f getVelocity();
	sf::Vector2f getPosition();
	sf::RectangleShape getRect();
	sf::RectangleShape getFOVRect();
	sf::Vector2f getseekPoint();
	bool getAlive();
	void setPosition(sf::Vector2f vec);
	void DropHuman(std::vector<Human*>& humans);
	void  setseekPoint(sf::Vector2f point);


	int health = 1;


	std::vector<AbductorBullet*> bullets;
};

#endif
