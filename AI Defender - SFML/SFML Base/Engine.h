#pragma once

// SFML Headers
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

// C++ Headers
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

// Custom Classes
#include "OptionsLoader.h"
#include "SplashScreen.h"
#include "MainMenu.h"
#include "Game.h"
#include "LoadScreen.h"
#include "OptionsMenu.h"
#include "InstructionsScreen.h"

class Engine
{
public:
	Engine();
	~Engine();

	void init();
	void update();
	void draw(sf::RenderWindow &window);

	OptionsLoader* optionsLoader;
	SplashScreen* splashScreen;
	MainMenu* mainMenu;
	Game* game;
	LoadScreen* load;
	OptionsMenu* optionsMenu;
	InstructionsScreen* instructions;

private:
	sf::Vector2f screenSize = sf::Vector2f(1920, 1080);
};

