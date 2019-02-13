#pragma once

// SFML Headers
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h> 

#include "GlobalVariables.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "ResourceLoader.h"

#include "Camera.h"
#include "Player.h"
#include "Human.h"
#include "Terrain.h"
#include "Obstacles.h"
#include "Nest.h"
#include "Abductor.h"
#include "Mutant.h"

class Game
{
private:
	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();
	InputManager* inputManager = InputManager::getInstance();
	CollisionManager* collisionManager = new CollisionManager();
	ResourceLoader* resource = ResourceLoader::instance();

public:
	Game(Camera* camera);
	~Game();

	void init();
	void update();
	void draw(sf::RenderWindow &window);
	void input(sf::Event Event);

private:
	sf::Vector2f screenSize = myGlobalOptions->screenSize;

	void goToScene(int screen);

	void controller(sf::Event Event);

	sf::Font font;
	sf::Text text;

	void teleport();

	int currentLevel;

	void MeteorSpawn();
	sf::Clock meteorSpawnClock;
	float meteorSpwanTimer;
	float meteorSpawnDelay;

	void clearVectors();
	void spawn();
	void nextLevel();
	void gameOver();
	void UI();
	void UIDraw(sf::RenderWindow &window);

	void scoreCounter();
	int score;
	int scoreTimer;

	bool isGameOver;
	sf::Text gameOverText;

protected:
	Camera* camera;
	Player* player;
	Terrain* terrain;

	std::vector<Human*> humans;
	std::vector<Obstacles*> meteors;
	std::vector<Abductor*> abductors;
	std::vector<Nest*> nests;
	std::vector<Mutant*> mutants;

	std::vector<Human*> humansToDelete = std::vector<Human*>();
	std::vector<Obstacles*> meteorsToDelete = std::vector<Obstacles*>();
	std::vector<Abductor*> abductorsToDelete = std::vector<Abductor*>();
	std::vector<Nest*> nestsToDelete = std::vector<Nest*>();
	std::vector<Mutant*> mutantsToDelete = std::vector<Mutant*>();

private:
	// UI
	sf::Text healthText;
	sf::Sprite healthSprite;

	sf::Sprite bombSprite;
	sf::Sprite warpSprite;

	sf::Text humanText;
	sf::Sprite humanSprite;

	sf::Text enemyText;
	sf::Sprite enemySprite;

	sf::Text scoreText;
};

