#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>

#define MAP_WIDTH_POINTS  192 * 9
#define FULL_WIDTH_POINTS  192 * 10
#define SCREEN_WIDTH_POINTS 192
#define HALF_SCREEN_WIDTH_POINTS 192 / 2

#define MAP_WIDTH_PIXEL  1920 * 9
#define FULL_WIDTH_PIXEL  1920 * 10
#define SCREEN_WIDTH_PIXEL 1920
#define HALF_SCREEN_WIDTH_PIXEL 1920 / 2

class GlobalVariables
{
public:
	static GlobalVariables * getInstance();
private:
	static GlobalVariables * globalVariablesInstance;

public:
	GlobalVariables();
	~GlobalVariables();

	void init();
	void update();
	void draw(sf::RenderWindow &window);

	int getCurrentScene();
	void setCurrentScene(int current);

	enum GameStates {
		CONTINUE,
		GAME,
		LOAD,
		OPTIONS,
		INSTRUCTIONS,
		PAUSE,
		GAMEOVER,
		MAINMENU,
		SPLASH
	};

	sf::Vector2f screenSize = sf::Vector2f(1920, 1080);
	sf::RenderWindow* windowReference;

	bool drawCollisionBox = false;
	bool restart = false;

private:

	int currentScene;
};