#include "Game.h"

Game::Game(Camera* cam)
{
	camera = cam;
	init();

	font.loadFromFile("content\\fonts\\kenvector_future.TTF");

	gameOverText.setFont(font);
	gameOverText.setString("    Press Enter to Play Again! \n\n\nPress BackSpace to Restart");
	gameOverText.setPosition(player->getPosition().x - gameOverText.getLocalBounds().width / 2.0f, screenSize.y / 2);
	gameOverText.setCharacterSize(32);
}

Game::~Game()
{
}

void Game::init()
{
	terrain = new Terrain();
	player = new Player(screenSize * 0.5f, sf::Vector2f(30, 30));//set up player
	player->health = 3;

	isGameOver = false;
	currentLevel = 1;

	score = 0;
	scoreTimer = 0;

	spawn();

	meteorSpwanTimer = 0;
	meteorSpawnDelay = rand() % 5;

	UI();
}

void Game::update()
{
	if (isGameOver == false)
	{
		camera->update(player);
		player->Update();

		// Power Level
		if (player->powerLevel > 3)
		{
			player->powerLevel = 3;
			score += 1000 * currentLevel;
		}

		// Human
		for (int i = 0; i < humans.size(); i++)
		{
			humans[i]->Update(player->getPosition());

			bool target = false;

			// Collisions
			// Player
			if (collisionManager->RectangleCollision(player->getRect(), humans[i]->getRect()))
			{
				resource->menuSelect.play();

				humansToDelete.push_back(humans[i]); // For Memory Clean up
				humans.erase(humans.begin() + i);

				score += 1000 * currentLevel;

				i--;
				continue;
			}

			if (humans[i]->getPosition().y < 0)
			{
				humansToDelete.push_back(humans[i]); // For Memory Clean up
				humans.erase(humans.begin() + i);
			}
		}

		// Meteor
		for (int i = 0; i < meteors.size(); i++)
		{
			meteors[i]->Update();

			// Death
			if (meteors[i]->getPosition().y >= 1080)
			{
				resource->hit.play();

				meteorsToDelete.push_back(meteors[i]); // For Memory Clean up
				meteors.erase(meteors.begin() + i);
			}

			// Collisions
			// Player
			if (collisionManager->RectangleCollision(player->getRect(), meteors[i]->getRect()))
			{
				resource->hit.play();

				player->health--;
				player->powerLevel = 1;

				meteorsToDelete.push_back(meteors[i]); // For Memory Clean up
				meteors.erase(meteors.begin() + i);

				i--;
				continue;
			}

			// Bullets
			bool checkNext = true;
			for (int j = 0; checkNext && j < player->getBullets().size(); j++)
			{
				if (collisionManager->RectangleCollision(player->getBullets().at(j)->getRect(), meteors[i]->getRect()))
				{
					resource->hit.play();

					player->deleteBullet(j);

					meteorsToDelete.push_back(meteors[i]); // For Memory Clean up
					meteors.erase(meteors.begin() + i);

					score += 10 * currentLevel;

					j--;
					i--;
					checkNext = false;
					continue;
				}
			}
		}


		// Enemies with Health 
		// Nest
		for (int i = 0; i < nests.size(); i++)
		{
			nests[i]->Update(player->getRect(), abductors, meteors);

			// Collisions
			// Player
			if (collisionManager->RectangleCollision(player->getRect(), nests[i]->getRect()))
			{
				resource->hit.play();

				player->health--;
				player->powerLevel = 1;

				// Die
				resource->explosion.play();
				nestsToDelete.push_back(nests[i]); // For Memory Clean up
				nests.erase(nests.begin() + i);

				i--;
				continue;
			}

			// Bullets
			bool checkNext = true;
			for (int j = 0; checkNext && j < player->getBullets().size(); j++)
			{
				if (collisionManager->RectangleCollision(player->getBullets().at(j)->getRect(), nests[i]->getRect()))
				{
					resource->hit.play();

					player->deleteBullet(j);

					nests[i]->health--;

					// Die
					if (nests[i]->health <= 0)
					{
						resource->explosion.play();
						nestsToDelete.push_back(nests[i]); // For Memory Clean up
						nests.erase(nests.begin() + i);
						score += 150 * currentLevel;
					}

					j--;
					i--;
					checkNext = false;
					continue;
				}
			}

			// Meteor
			for (int j = 0; checkNext && j < meteors.size(); j++)
			{
				if (collisionManager->RectangleCollision(meteors[j]->getRect(), nests[i]->getRect()))
				{
					resource->hit.play();

					meteorsToDelete.push_back(meteors[j]); // For Memory Clean up
					meteors.erase(meteors.begin() + j);

					nests[i]->health--;

					// Die
					if (nests[i]->health <= 0)
					{
						resource->explosion.play();
						nestsToDelete.push_back(nests[i]); // For Memory Clean up
						nests.erase(nests.begin() + i);
					}

					j--;
					i--;
					checkNext = false;
					continue;
				}
			}

			// Bullet - Enemy
			for (int j = 0; checkNext &&  j < nests[i]->missileList.size(); j++)
			{
				if (collisionManager->RectangleCollision(nests[i]->missileList[j]->getRect(), player->getRect()))
				{
					resource->hit.play();

					player->health--;

					nests[i]->missileList.erase(nests[i]->missileList.begin() + j);

					j--;
					i--;
					checkNext = false;
					continue;
				}
			}
		}

		// Abductors
		for (int i = 0; i < abductors.size(); i++)
		{
			abductors[i]->Update(abductors, i, meteors, humans, player->getPosition());

			// Create Mutant
			if (abductors[i]->getAlive() == false)//if an abductor has successfully abducted a human
			{
				resource->back.play();

				mutants.push_back(new Mutant(abductors[i]->getPosition()));
				mutants.back()->health = currentLevel * 2;

				abductorsToDelete.push_back(abductors[i]); // For Memory Clean up
				abductors.erase(abductors.begin() + i);

				i--;
				continue;
			}

			// Collisions
			// Player
			if (collisionManager->RectangleCollision(player->getRect(), abductors[i]->getRect()))
			{
				resource->hit.play();

				player->health--;
				player->powerLevel = 1;

				// Die
				resource->explosion.play();
				abductors[i]->DropHuman(humans);
				abductorsToDelete.push_back(abductors[i]); // For Memory Clean up
				abductors.erase(abductors.begin() + i);

				i--;
				continue;
			}

			// Bullets
			bool checkNext = true;
			for (int j = 0; checkNext && j < player->getBullets().size(); j++)
			{
				if (collisionManager->RectangleCollision(player->getBullets().at(j)->getRect(), abductors[i]->getRect()))
				{
					resource->hit.play();

					player->deleteBullet(j);

					abductors[i]->health--;

					// Die
					if (abductors[i]->health <= 0)
					{
						resource->explosion.play();
						abductors[i]->DropHuman(humans);
						abductorsToDelete.push_back(abductors[i]); // For Memory Clean up
						abductors.erase(abductors.begin() + i);
						score += 50 * currentLevel;
					}

					j--;
					i--;
					checkNext = false;
					continue;
				}
			}

			// Meteor
			for (int j = 0; checkNext && j < meteors.size(); j++)
			{
				if (collisionManager->RectangleCollision(meteors[j]->getRect(), abductors[i]->getRect()))
				{
					resource->hit.play();

					meteorsToDelete.push_back(meteors[j]); // For Memory Clean up
					meteors.erase(meteors.begin() + j);

					abductors[i]->DropHuman(humans);

					abductors[i]->health--;

					// Die
					if (abductors[i]->health <= 0)
					{
						resource->explosion.play();
						abductorsToDelete.push_back(abductors[i]); // For Memory Clean up
						abductors.erase(abductors.begin() + i);
					}

					j--;
					i--;
					checkNext = false;
					continue;
				}
			}

			// Bullet - Enemy
			for (int j = 0; checkNext && j < abductors[i]->bullets.size(); j++)
			{
				if (collisionManager->RectangleCollision(abductors[i]->bullets[j]->getRect(), player->getRect()))
				{
					resource->hit.play();

					player->health--;

					abductors[i]->bullets.erase(abductors[i]->bullets.begin() + j);

					j--;
					i--;
					checkNext = false;
					continue;
				}
			}
		}

		//Mutants
		for (int i = 0; i < mutants.size(); i++)
		{
			mutants[i]->Update(mutants, i, meteors, player->getPosition(), player->getVelocity());

			// Collisions
			// Player
			if (collisionManager->RectangleCollision(player->getRect(), mutants[i]->getRect()))
			{
				resource->hit.play();

				player->health--;
				player->powerLevel = 1;

				// Die
				resource->explosion.play();
				mutantsToDelete.push_back(mutants[i]); // For Memory Clean up
				mutants.erase(mutants.begin() + i);

				i--;
				continue;
			}

			// Bullets
			bool checkNext = true;
			for (int j = 0; checkNext && j < player->getBullets().size(); j++)
			{
				if (collisionManager->RectangleCollision(player->getBullets().at(j)->getRect(), mutants[i]->getRect()))
				{
					resource->hit.play();

					player->deleteBullet(j);

					mutants[i]->health--;

					// Die
					if (mutants[i]->health <= 0)
					{
						resource->explosion.play();
						mutantsToDelete.push_back(mutants[i]); // For Memory Clean up
						mutants.erase(mutants.begin() + i);
						score += 250 * currentLevel;
					}

					j--;
					i--;
					checkNext = false;
					continue;
				}
			}

			// Meteor
			for (int j = 0; checkNext && j < meteors.size(); j++)
			{
				if (collisionManager->RectangleCollision(meteors[j]->getRect(), mutants[i]->getRect()))
				{
					resource->hit.play();

					meteorsToDelete.push_back(meteors[j]); // For Memory Clean up
					meteors.erase(meteors.begin() + j);

					mutants[i]->health--;

					// Die
					if (mutants[i]->health <= 0)
					{
						resource->explosion.play();
						mutantsToDelete.push_back(mutants[i]); // For Memory Clean up
						mutants.erase(mutants.begin() + i);
					}

					j--;
					i--;
					checkNext = false;
					continue;
				}
			}

			// Bullet - Enemy
			for (int j = 0; checkNext && j < mutants[i]->bullets.size(); j++)
			{
				if (collisionManager->RectangleCollision(mutants[i]->bullets[j]->getRect(), player->getRect()))
				{
					resource->hit.play();

					player->health--;

					mutants[i]->bullets.erase(mutants[i]->bullets.begin() + j);

					j--;
					i--;
					checkNext = false;
					continue;
				}
			}
		}

		if (mutants.empty() && abductors.empty() && nests.empty() && player->health != 0)
		{
			resource->levelUp.play();
			humans.clear();
			nextLevel();
			score += 1000 * currentLevel;
		}

		teleport();
		MeteorSpawn();
		scoreCounter();

		// Game over
		if (player->health <= 0)
		{
			resource->explosion.play();
			gameOver();
		}
	}
}

void Game::draw(sf::RenderWindow &window)
{
	//Game
	camera->drawGame(window);
	terrain->draw(window);
	player->Draw(window);

	if (isGameOver == false)
	{
		for (int i = 0; i < meteors.size(); i++)
		{
			if(meteors[i]->getPosition().x < player->getPosition().x + SCREEN_WIDTH_PIXEL && meteors[i]->getPosition().x > player->getPosition().x - SCREEN_WIDTH_PIXEL)
				meteors[i]->Draw(window);
		}
		for (int i = 0; i < humans.size(); i++)
		{
			if (humans[i]->getPosition().x < player->getPosition().x + SCREEN_WIDTH_PIXEL && humans[i]->getPosition().x > player->getPosition().x - SCREEN_WIDTH_PIXEL)
				humans[i]->Draw(window, false);
		}
		for (int i = 0; i < nests.size(); i++)
		{
			if (nests[i]->getPosition().x < player->getPosition().x + SCREEN_WIDTH_PIXEL && nests[i]->getPosition().x > player->getPosition().x - SCREEN_WIDTH_PIXEL)
				nests[i]->Draw(window);
		}
		for (int i = 0; i < abductors.size(); i++)
		{
			if (abductors[i]->getPosition().x < player->getPosition().x + SCREEN_WIDTH_PIXEL && abductors[i]->getPosition().x > player->getPosition().x - SCREEN_WIDTH_PIXEL)
				abductors[i]->Draw(window);
		}
		for (int i = 0; i < mutants.size(); i++)
		{
			if (mutants[i]->getPosition().x < player->getPosition().x + SCREEN_WIDTH_PIXEL && mutants[i]->getPosition().x > player->getPosition().x - SCREEN_WIDTH_PIXEL)
				mutants[i]->Draw(window);
		}

		//UI
		UIDraw(window);

		// Mini Map
		camera->drawRadar(window);
		terrain->draw(window);
		player->Draw(window);
		for (int i = 0; i < meteors.size(); i++)
		{
			meteors[i]->Draw(window);
		}
		for (int i = 0; i < humans.size(); i++)
		{
			humans[i]->Draw(window, true);
		}
		for (int i = 0; i < nests.size(); i++)
		{
			nests[i]->Draw(window);
		}
		for (int i = 0; i < abductors.size(); i++)
		{
			abductors[i]->Draw(window);
		}
	}
	else
	{
		window.draw(gameOverText);
		gameOverText.setPosition(player->getPosition().x - gameOverText.getLocalBounds().width / 2.0f, screenSize.y / 2);
	}
}

void Game::goToScene(int scene)
{
	myGlobalOptions->setCurrentScene(scene);
}

void Game::input(sf::Event Event)
{
	if (Event.type == sf::Event::KeyPressed)
	{
		std::cout << Event.key.code << std::endl;
	}

	if (isGameOver == false)
	{
		if (inputManager->KeyPressed(sf::Keyboard::BackSpace))
		{
			camera->resetView();
			std::cout << "Back Space" << std::endl;
			goToScene(myGlobalOptions->MAINMENU);
			resource->musicMenu.play();
			resource->musicGame.pause();
		}
	}

	else
	{
		if (inputManager->KeyPressed(sf::Keyboard::BackSpace))
		{
			myGlobalOptions->restart = true;
			camera->resetView();
			std::cout << "Back Space" << std::endl;
			goToScene(myGlobalOptions->SPLASH);
			init();
		}

		if (inputManager->KeyPressed(sf::Keyboard::Return))
		{
			camera->resetView();
			std::cout << "Enter" << std::endl;
			init();
			resource->musicGame.play();
		}
	}

	controller(Event);
}

void Game::controller(sf::Event Event)
{
	// Left
	if (inputManager->KeyPressed(sf::Keyboard::A))
	{
		player->FaceLeft();
	}
	else if (inputManager->KeyHeld(sf::Keyboard::A) && !inputManager->KeyHeld(sf::Keyboard::D))
	{
		player->MoveLeft();
	}
	else if (inputManager->KeyReleased(sf::Keyboard::A) || !inputManager->KeyHeld(sf::Keyboard::D))
	{
		player->Decelerate();
	}

	// Right
	if (inputManager->KeyPressed(sf::Keyboard::D))
	{
		player->FaceRight();
	}
	else if (inputManager->KeyHeld(sf::Keyboard::D) && !inputManager->KeyHeld(sf::Keyboard::A))
	{
		player->MoveRight();
	}
	else if (inputManager->KeyReleased(sf::Keyboard::D) || !inputManager->KeyHeld(sf::Keyboard::A))
	{
		player->Decelerate();
	}

	// Up
	if (inputManager->KeyPressed(sf::Keyboard::W))
	{
		player->MoveUp(true);
	}
	else if (inputManager->KeyReleased(sf::Keyboard::W))
	{
		player->MoveUp(false);
	}

	// Down
	if (inputManager->KeyPressed(sf::Keyboard::S))
	{
		player->MoveDown(true);
	}
	else if (inputManager->KeyReleased(sf::Keyboard::S))
	{
		player->MoveDown(false);
	}

	// Shoot
	if (inputManager->KeyHeld(sf::Keyboard::Space))
	{
		player->Shoot();
	}
	if (inputManager->KeyHeld(sf::Keyboard::Num1) && player->getbombLoaded() == true && player->getBombfired() == false)
	{
		player->setBombfired(true);

	}
	if (inputManager->KeyHeld(sf::Keyboard::Num2) && player->getwarpready() == true)
	{
		player->setWarp(true);
	}
}

void Game::teleport()
{
	if (player->teleport())
	{
		// Human
		for (int i = 0; i < humans.size(); i++)
		{
			if (humans[i]->getPoint() < (SCREEN_WIDTH_POINTS) && player->teleportLeft == true)
			{
				humans[i]->setPoint(humans[i]->getPoint() + MAP_WIDTH_POINTS);
			}

			else if (humans[i]->getPoint() > FULL_WIDTH_POINTS - (SCREEN_WIDTH_POINTS) && player->teleportLeft == false)
			{
				humans[i]->setPoint(humans[i]->getPoint() - MAP_WIDTH_POINTS);
			}
		}

		// Obstacle
		for (int i = 0; i < meteors.size(); i++)
		{
			if (meteors[i]->getPosition().x < (SCREEN_WIDTH_PIXEL) && player->teleportLeft == true)
			{
				meteors[i]->setPosition(sf::Vector2f(meteors[i]->getPosition().x + (MAP_WIDTH_PIXEL), meteors[i]->getPosition().y));
			}

			else if (meteors[i]->getPosition().x > (FULL_WIDTH_PIXEL) -(HALF_SCREEN_WIDTH_PIXEL * 3) && player->teleportLeft == false)
			{
				meteors[i]->setPosition(sf::Vector2f(meteors[i]->getPosition().x - (MAP_WIDTH_PIXEL), meteors[i]->getPosition().y));
			}
		}
	}
}

void Game::MeteorSpawn()
{
	meteorSpwanTimer += 1.0f / 60.0f;

	if (meteorSpwanTimer >= meteorSpawnDelay / currentLevel)
	{
		meteors.push_back(new Obstacles());
		meteorSpawnDelay = rand() % 5;
		meteorSpwanTimer = 0;
	}
}

void Game::spawn()
{
	for (int i = 0; i < currentLevel; i++)
	{

		humans.push_back(new Human(terrain->getPoints()));

		nests.push_back(new Nest(sf::Vector2f(rand() % (1920 * 9) + 1, rand() % (500) + 1)));

		abductors.push_back(new Abductor(sf::Vector2f(rand() % (1920 * 8), 50)));
		abductors.push_back(new Abductor(sf::Vector2f(rand() % (1920 * 8), 50)));
		abductors.push_back(new Abductor(sf::Vector2f(rand() % (1920 * 8), 50)));
		abductors.push_back(new Abductor(sf::Vector2f(rand() % (1920 * 8), 50)));
	}
}

void Game::clearVectors()
{
	for (std::vector<Human*>::iterator it = humansToDelete.begin(); it != humansToDelete.end(); ++it)
	{
		delete *it;
	}
	humansToDelete.clear();

	for (std::vector<Obstacles*>::iterator it = meteorsToDelete.begin(); it != meteorsToDelete.end(); ++it)
	{
		delete *it;
	}
	meteorsToDelete.clear();

	for (std::vector<Abductor*>::iterator it = abductorsToDelete.begin(); it != abductorsToDelete.end(); ++it)
	{
		delete *it;
	}
	abductorsToDelete.clear();

	for (std::vector<Nest*>::iterator it = nestsToDelete.begin(); it != nestsToDelete.end(); ++it)
	{
		delete *it;
	}
	nestsToDelete.clear();

	for (std::vector<Mutant*>::iterator it = mutantsToDelete.begin(); it != mutantsToDelete.end(); ++it)
	{
		delete *it;
	}
	mutantsToDelete.clear();
}

void Game::nextLevel()
{
	clearVectors();

	player->health++;
	currentLevel++;

	spawn();

	for (int i = 0; i < abductors.size(); i++)
	{
		abductors[i]->health = currentLevel;
	}

	for (int i = 0; i < nests.size(); i++)
	{
		nests[i]->health = currentLevel + (currentLevel / 2);
	}
}

void Game::gameOver()
{
	isGameOver = true;

	clearVectors();

	// Clear what is on screen
	for (std::vector<Human*>::iterator it = humans.begin(); it != humans.end(); ++it)
	{
		delete *it;
	}
	humans.clear();

	for (std::vector<Obstacles*>::iterator it = meteors.begin(); it != meteors.end(); ++it)
	{
		delete *it;
	}
	meteors.clear();

	for (std::vector<Abductor*>::iterator it = abductors.begin(); it != abductors.end(); ++it)
	{
		delete *it;
	}
	abductors.clear();

	for (std::vector<Nest*>::iterator it = nests.begin(); it != nests.end(); ++it)
	{
		delete *it;
	}
	nests.clear();

	for (std::vector<Mutant*>::iterator it = mutants.begin(); it != mutants.end(); ++it)
	{
		delete *it;
	}
	mutants.clear();

	resource->musicMenu.stop();
	resource->musicGame.stop();
}

void Game::UI()
{
	// Health
	healthSprite.setTexture(resource->gethealthTexture());
	healthSprite.setPosition(sf::Vector2f(player->getPosition().x - 900, 50));
	healthSprite.setOrigin(sf::Vector2f(healthSprite.getLocalBounds().width / 2, healthSprite.getLocalBounds().height / 2));
	healthSprite.setScale(sf::Vector2f(1.f, 1.f));

	healthText.setFont(font);
	healthText.setString(std::to_string(player->health));
	healthText.setPosition(sf::Vector2f(healthSprite.getPosition().x + 25, 50));

	// Bomb
	bombSprite.setTexture(resource->getbombTexture());
	bombSprite.setPosition(sf::Vector2f(player->getPosition().x - 800, 50));
	bombSprite.setOrigin(sf::Vector2f(bombSprite.getLocalBounds().width / 2, bombSprite.getLocalBounds().height / 2));
	bombSprite.setScale(sf::Vector2f(1.f, 1.f));

	// Warp
	warpSprite.setTexture(resource->getwarpTexture());
	warpSprite.setPosition(sf::Vector2f(player->getPosition().x - 700, 50));
	warpSprite.setOrigin(sf::Vector2f(warpSprite.getLocalBounds().width / 2, warpSprite.getLocalBounds().height / 2));
	warpSprite.setScale(sf::Vector2f(1.f, 1.f));

	// human
	humanSprite.setTexture(resource->gethumanTexture());
	humanSprite.setPosition(sf::Vector2f(player->getPosition().x - 600, 50));
	humanSprite.setOrigin(sf::Vector2f(humanSprite.getLocalBounds().width / 2, humanSprite.getLocalBounds().height / 2));
	humanSprite.setScale(sf::Vector2f(2.f, 2.f));

	humanText.setFont(font);
	humanText.setString(std::to_string(humans.size()));
	humanText.setPosition(sf::Vector2f(humanSprite.getPosition().x + 25, 50));

	// enemy
	enemySprite.setTexture(resource->getenemyTexture());
	enemySprite.setPosition(sf::Vector2f(player->getPosition().x - 500, 50));
	enemySprite.setOrigin(sf::Vector2f(enemySprite.getLocalBounds().width / 2, enemySprite.getLocalBounds().height / 2));
	enemySprite.setScale(sf::Vector2f(2.f, 2.f));

	enemyText.setFont(font);
	int size = abductors.size() + nests.size() + mutants.size();
	enemyText.setString(std::to_string(size));
	enemyText.setPosition(sf::Vector2f(enemySprite.getPosition().x + 25, 50));

	// score
	scoreText.setFont(font);
	scoreText.setString(std::to_string(score));
	scoreText.setPosition(sf::Vector2f(960, 50));
}

void Game::UIDraw(sf::RenderWindow &window)
{
		// health
		window.draw(healthSprite);
		healthSprite.setPosition(sf::Vector2f(player->getPosition().x - 900, 50));

		window.draw(healthText);
		healthText.setString(std::to_string(player->health));
		healthText.setPosition(sf::Vector2f(healthSprite.getPosition().x + 25, 50));

		// Bomb
		window.draw(bombSprite);
		bombSprite.setPosition(sf::Vector2f(player->getPosition().x - 800, 50));
		if (player->getbombLoaded() == false) { bombSprite.setColor(sf::Color(125, 125, 125, 125)); }
		else{ bombSprite.setColor(sf::Color(255, 255, 255, 255)); }

		// Warp
		window.draw(warpSprite);
		warpSprite.setPosition(sf::Vector2f(player->getPosition().x - 700, 50));
		if (player->getwarpready() == false) { warpSprite.setColor(sf::Color(125, 125, 125, 125)); }
		else { warpSprite.setColor(sf::Color(255, 255, 255, 255)); }

		// Human
		window.draw(humanSprite);
		humanSprite.setPosition(sf::Vector2f(player->getPosition().x - 600, 50));

		window.draw(humanText);
		humanText.setString(std::to_string(humans.size()));
		humanText.setPosition(sf::Vector2f(humanSprite.getPosition().x + 25, 50));

		// enemy
		window.draw(enemySprite);
		enemySprite.setPosition(sf::Vector2f(player->getPosition().x - 500, 50));

		window.draw(enemyText);
		int size = abductors.size() + nests.size() + mutants.size();
		enemyText.setString(std::to_string(size));
		enemyText.setPosition(sf::Vector2f(enemySprite.getPosition().x + 25, 50));

		// Score
		window.draw(scoreText);
		scoreText.setString(std::to_string(score));
		scoreText.setPosition(sf::Vector2f(player->getPosition().x, 50));
}

void Game::scoreCounter()
{
	scoreTimer += 1.0f;

	if (scoreTimer >= 60)
	{
		score += currentLevel;
		scoreTimer -= 60;
	}
}
