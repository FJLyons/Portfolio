#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>

#include "GlobalVariables.h"
#include "InputManager.h"
#include "ResourceLoader.h"

#define OPTIONS_MENU_INDEX 2

class OptionsMenu
{
private:
	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();
	InputManager* inputManager = InputManager::getInstance();
	ResourceLoader* resource = ResourceLoader::instance();

public:
	OptionsMenu();
	~OptionsMenu();

	void init();
	void update();
	void draw(sf::RenderWindow &window);
	void input(sf::Event Event);

private:
	sf::Vector2f screenSize = sf::Vector2f(1920, 1080);

	void goToScene(int screen);

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
	sf::Text text[OPTIONS_MENU_INDEX];

	enum MenuOptions {
		OPTION1,
		OPTION2
	};

	int volume = 50;
};

