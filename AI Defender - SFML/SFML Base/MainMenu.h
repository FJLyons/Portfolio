#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>

#include "GlobalVariables.h"
#include "InputManager.h"
#include "ResourceLoader.h"

#define MENU_INDEX 5

class MainMenu
{
private:
	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();
	InputManager* inputManager = InputManager::getInstance();
	ResourceLoader* resource = ResourceLoader::instance();

public:
	MainMenu();
	~MainMenu();

	void init();
	void update();
	void draw(sf::RenderWindow &window);
	void input(sf::Event Event);

private:
	sf::Vector2f screenSize = sf::Vector2f(1920, 1080);

	void goToScene(int screen);
	void swapScreen();

	void moveUp();
	void moveDown();

	int getPressedItem();

	// Art
	sf::Texture logoTexture;
	sf::Sprite logoSprite;
	sf::Vector2f scaleFactor; // Resolution vs Screen

							  // Index
	int selectedItemIndex;

	// Font
	sf::Font font;
	sf::Text text[MENU_INDEX];

	enum MenuOptions {
		GAME,
		LOAD,
		OPTIONS,
		INSTRUCTIONS,
		QUIT
	};
};

