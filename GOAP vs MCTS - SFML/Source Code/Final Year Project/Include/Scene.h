#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>

#include "Manager_Resource.h"
#include "Manager_Input.h"

class Scene
{
private:
	// Avoid unwanted construction
	Scene(const Scene&) = delete;
	// Avoid unwanted assignment
	Scene& operator=(const Scene&) = delete;

public:
	// Create Scene
	Scene();
	// Virtual destructor to enable destruction of scene pointer objects
	virtual ~Scene();

	// Initialise scene
	virtual void Init();
	// Update scene
	virtual GameScene Update();
	// Draw scene
	// @param window - SFML window
	virtual void Draw(sf::RenderWindow &window);
	// Update Input
	// @param event - SF Event
	virtual void Input(sf::Event Event);

protected:
	// Input Manager instance object - returns single instance on Input manager
	InputManager* m_inputManager = InputManager::GetInstance();

	// Current runnng scene - for scene manager and scene updates
	GameScene m_currentScene;
};

