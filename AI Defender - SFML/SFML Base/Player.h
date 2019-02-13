#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "SFML\Graphics.hpp"

#include "GlobalVariables.h"
#include "InputManager.h"

#include "Bullet.h"
#include "ResourceLoader.h"
#include "CollisionManager.h"


class Player
{

private:
	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();
	ResourceLoader* resource = ResourceLoader::instance();

private:
	sf::Texture mTexture;
	sf::Sprite mSprite;
	sf::Vector2f mPositon;
	sf::Vector2f maxVelocity;
	sf::Vector2f velocity;
	sf::Vector2f mScale;
	sf::Texture powerupTex;
	sf::Vector2f powerupPos;
	sf::Sprite powerupSprite;
	sf::RectangleShape collisionRect;
	sf::RectangleShape powerupCollisionRect;
	sf::RectangleShape bombRectangle;
	std::vector<Bullet*> bulletList;
	sf::Vector2f bombrectSize;
	bool playerFacingRight;
	bool canShoot;
	bool bombLoaded;
	bool warping;
	float flipSpeed;
	float shotdelay;
	float shotTimer;
	float powerupScale;
	float bombTimer;
	float warptimer;
	bool warpReady;
	bool invisible;
	sf::Clock shotClock;
	bool bombfired;
	float powerupdelay;
	bool powercollected;
	float powertimer;

public:
	Player();
	Player(sf::Vector2f pos, sf::Vector2f maxVel);

	void FaceLeft();
	void FaceRight();

	void MoveUp(bool pressed);
	void MoveDown(bool pressed);
	void MoveLeft();
	void MoveRight();
	void ShootBomb();
	void Decelerate();
	void updatePowerup();
	void Shoot();
	void Warp();
	void Update();
	void Draw(sf::RenderWindow &window);
	void Flip();
	sf::Vector2f getVelocity();
	sf::Vector2f getPosition();
	sf::RectangleShape getRect();
	bool teleport();
	bool teleportLeft = true;
	bool getBombfired();
	void setBombfired(bool fire);
	bool getbombLoaded();
	bool getwarpready();
	void setWarp(bool warp);
	std::vector<Bullet*> getBullets();

	void deleteBullet(int index);

	int health = 3;

	int powerLevel;
};

#endif