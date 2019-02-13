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
#include "Manager_Resource.h"

#include "Scene.h"
#include "Scene_Splash.h"
#include "Scene_Main_Menu.h"
#include "Scene_Game.h"
#include "Scene_Instructions.h"


class SceneManager
{
private:
	// Avoid unwanted construction
	SceneManager(const SceneManager&) = delete;
	// Avoid unwanted assignment
	SceneManager& operator=(const SceneManager&) = delete;

public:
	// Create Scene Manager
	SceneManager();
	// Destructor
	~SceneManager();

	// Initialise Scene Manager
	void Init();
	// Update and Draw running scene
	void Process();
	// Provide input to scenes
	void Input(sf::Event evt);
	// Swap current running scene
	void SwapScene();

private:
	// Splash screen object
	SplashScreen*			m_splash;
	// Main Menu scene object
	MainMenu*				m_mainMenu;
	// Game scene object
	Game*					m_game;
	// Instruction scene object
	InstructionsScreen*		m_instructions;

	// Current running scene
	std::vector<Scene*>		m_currentScene;

	// Identifier for running scene
	GameScene				m_runningScene;
	// Identifier for previous running scene
	GameScene				m_previousScene;
};

