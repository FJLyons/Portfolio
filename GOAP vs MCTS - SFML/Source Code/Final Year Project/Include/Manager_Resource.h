#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>

#include "Variables_Const.h"
#include "Variables_Enum.h"

class ResourceManager
{
public:
	
	// Return an instance of the GlobalVaraibles class
	static ResourceManager* GetInstance();
private:
	// ResourceManager instance object - for returning instance
	static ResourceManager* globalVariablesInstance;

	// Avoid unwanted construction
	ResourceManager(const ResourceManager&) = delete;
	// Avoid unwanted assignment
	ResourceManager& operator=(const ResourceManager&) = delete;

public:
	ResourceManager();
	~ResourceManager();

	// Global Window Reference - for drawing
	sf::RenderWindow* windowReference;

	// Global Font - for avoiding loading multiple
	sf::Font FONT;

	// Map Texture
	sf::Texture t_map;

	// GOAP Builder Texture
	sf::Texture t_blue_builder;
	// GOAP Soldier Texture
	sf::Texture t_blue_soldier;
	// GOAP Archer Texture
	sf::Texture t_blue_archer;
	// GOAP Wall Texture
	sf::Texture t_blue_wall;
	// GOAP Castle Texture
	sf::Texture t_blue_castle;
	// GOAP Resource Texture
	sf::Texture t_blue_resource;

	// MCTS Builder Texture
	sf::Texture t_red_builder;
	// MCTS Soldier Texture
	sf::Texture t_red_soldier;
	// MCTS Archer Texture
	sf::Texture t_red_archer;
	// MCTS Wall Texture
	sf::Texture t_red_wall;
	// MCTS Castle Texture
	sf::Texture t_red_castle;
	// MCTS Resource Texture
	sf::Texture t_red_resource;

	// Game Music
	sf::Music music;

	// Builder sound - for units to copy
	sf::Sound s_builder;
	// Soldier sound - for units to copy
	sf::Sound s_soldier;
	// Archer sound - for units to copy
	sf::Sound s_archer;
	// Walking sound - for units to copy
	sf::Sound s_walk;

	// Menu Click sound
	sf::Sound s_menu;
	// Enter Button sound
	sf::Sound s_enter;

	// Builder sound buffer  - for units to copy
	sf::SoundBuffer b_builder;
	// Soldier sound buffer  - for units to copy
	sf::SoundBuffer b_soldier;
	// Archer sound buffer  - for units to copy
	sf::SoundBuffer b_archer;
	// Walking sound buffer  - for units to copy
	sf::SoundBuffer b_walk;

	// Menu Click sound buffer
	sf::SoundBuffer b_menu;
	// Enter Button sound buffer
	sf::SoundBuffer b_enter;

	// State Names - for printing current action state of unit
	std::map<ActionState, std::string> stateNames;

	// Setup for stateNames map.
	void createStateNames();
};