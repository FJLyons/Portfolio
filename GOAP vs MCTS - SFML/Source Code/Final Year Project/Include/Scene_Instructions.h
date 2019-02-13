#pragma once

#include "Scene.h"

class InstructionsScreen : public Scene
{
private:
	// Avoid unwanted construction
	InstructionsScreen(const InstructionsScreen&) = delete;
	// Avoid unwanted assignment
	InstructionsScreen& operator=(const InstructionsScreen&) = delete;

public:
	InstructionsScreen();
	~InstructionsScreen(); 
	
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

	// Instructions text
	sf::Text m_text[INSUI];
};

