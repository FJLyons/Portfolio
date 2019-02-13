#pragma once

#include "Scene.h"

class MainMenu : public Scene
{
private:
	// Avoid unwanted construction
	MainMenu(const MainMenu&) = delete;
	// Avoid unwanted assignment
	MainMenu& operator=(const MainMenu&) = delete;

public:
	MainMenu();
	~MainMenu();

	// Initialise scene
	void Init() override;
	// Update scene
	GameScene Update() override;
	// Draw scene
	// @param window - SFML window
	void Draw(sf::RenderWindow &window) override;
	// Update Input
	// @param event - SF Event
	void Input(sf::Event Event) override;

private:
	//// VARIABLES

	// Logo Texture
	sf::Texture	 m_logoTexture;
	// Logo Sprite
	sf::Sprite m_logoSprite;

	// Menu item text
	sf::Text text[MAIN_MENU_INDEX];

	// Resolution vs Screen
	sf::Vector2f scaleFactor;	

	// Menu item index
	int selectedItemIndex;

	//// FUNTIONS

	// Swap current scene to new scene
	void SwapScreen();

	//Traverse menu up
	void MoveUp();
	//Traverse menu down
	void MoveDown();

	// Return current menu item index
	int GetPressedItem();
};

