#pragma once

// SFML Headers
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h> 

#include "Manager_Resource.h"
#include "Manager_Input.h"

#include "Scene.h"

#include "Logger.h"

#include "Map.h"
#include "Unit.h"
#include "Structure.h"
#include "Unit_Factory.h"

#include "GOAP_Action.h"
#include "GOAP_WorldState.h"
#include "GOAP_Planner.h"

#include "MCTS_Action.h"
#include "MCTS_WorldState.h"
#include "MCTS_Planner.h"

class Game : public Scene
{
private:
	// Avoid unwanted construction
	Game(const Game&) = delete;
	// Avoid unwanted assignment
	Game& operator=(const Game&) = delete;

public:
	Game();
	~Game();

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

	// Clock to determie time since last update
	sf::Clock m_clock;

	// GOAP UI text
	sf::Text m_textGOAP[UIS];
	// MCTS UI text
	sf::Text m_textMCTS[UIS];
	// Time UI text
	sf::Text m_time;
	// Game counter UI text
	sf::Text m_gameCounter;

	// Minutes counter
	int m_minutes;
	// Seconds counter
	int m_seconds;
	// Clock tic cunter
	float m_tic;

	// GOAP UI strings - for text
	std::string m_UI_G[UIS];
	// MCTS UI strings - for text
	std::string m_UI_M[UIS];

	// GOAP value bar - for UI
	sf::RectangleShape GOAPLine;
	// MCTS value bar - for UI
	sf::RectangleShape MCTSLine;
	// HUB box - for UI
	sf::RectangleShape HUD;

	// GOAP win count
	int m_GOAPWins;
	// MCTS win count
	int m_MCTSWins;
	// Winner win count - for logger
	int m_winnerWins;

	// Debug level - for UI
	int m_debug;

	// Is game paused?
	bool m_pause;
	// Is UI active?
	bool m_UI;

	// Current game time
	float m_gameTime;

	////OBJECTS

	// Background map
	Map m_map;

	// GOAP Base
	Structure* m_leftCastle;
	// MCTS Base
	Structure* m_rightCastle;

	// Entity Factory - for spawning units
	Factory* m_factory;

	// List of all GOAP Units
	std::vector<Unit*> m_leftUnits;
	// List of all MCTS Units
	std::vector<Unit*> m_rightUnits;

	// List of all GOAP Walls
	std::vector<Structure*> m_leftWalls;
	// List of all MCTS Walls
	std::vector<Structure*> m_rightWalls;

	// List of all GOAP Resources
	std::vector<Structure*> m_leftResources;
	// List of all MCTS Resources
	std::vector<Structure*> m_rightResources;

	// GOAP Build Queue timer
	float m_leftTimer;
	// MCTS Build Queue timer
	float m_rightTimer;	

	// GOAP build queue
	std::vector<BuildQueue> m_leftQueue;
	// MCTS build queue
	std::vector<BuildQueue> m_rightQueue;

	// Builders in GOAP queue
	int m_leftQueueBuilders;
	// Soldiers in GOAP queue
	int m_leftQueueSoldiers;
	// Archers in GOAP queue
	int m_leftQueueArchers;
	// Walls in GOAP queue
	int m_leftQueueWalls;

	// Builders in MCTS queue
	int m_rightQueueBuilders;
	// Soldiers in MCTS queue
	int m_rightQueueSoldiers;
	// Archers in MCTS queue
	int m_rightQueueArchers;
	// Walls in MCTS queue
	int m_rightQueueWalls;

	// Current game number
	int m_gameNumber;
	// Is game over?
	bool m_gameOver;
	// Current game winner - for logger
	std::string m_winner;
 
	//// FUNCTIONS

	// Army builder and team value calculation AI
	// @param dt - Time since last update
	void TeamBuilder(const float dt);

	// GOAP build queue assignment
	// @param dt - Time since last update
	void BuildLeft(const float dt);
	// MCTS build queue assignment
	// @param dt - Time since last update
	void BuildRight(const float dt);

	// World evaluation and goal priority calculation
	// @param unit - Unit in need of new goal
	void WorldEvaluation(Unit* unit);
	// Assign new goals to units
	// @param unit - Unit in need of new goal
	// @param currentState - current state of the game world
	void GoalAllocation(Unit* unit, const WorldState& currentState);

	// Update GOAP Units
	// @param dt - Time since last update
	void UpdateGOAP(const float dt);
	// Update MCTS Units
	// @param dt - Time since last update
	void UpdateMCTS(const float dt);

	// Initialise GOAP units and system
	void InitGOAP();
	// Initialise MCTS units and system
	void InitMCTS();

	// Run GOAP system
	// @param i - Index of GOAP Unit
	void GOAP(int i);
	// Run MCTS system
	// @param i - Index of MCTS Unit
	void MCTS(int i);

	// Initilise game castles
	void SetUpCastles();
	// Initilise game units
	void SetUpUnits();
	// Initilise game walls
	void SetUpWalls();
	// Initilise game resources
	void SetUpResources();

	// Check is game over
	// @param dt - Time since last update
	void GameOver(const float dt);
	// String of variables - for logger to produce results
	std::string Log();

	// Reset the current game
	void Reset();
	// Initialise game variables
	void InitVariables();
	// Initialise UI
	void InitUI();

	//// GOAP VARIABLES

	// Unit engage castle
	GOAPWorldState* goap_state_engage_castle;
	// Unit engaging enemy
	GOAPWorldState* goap_state_engage_enemy;
	// Unit engaging structure
	GOAPWorldState* goap_state_engage_wall;

	// Unit defending castle
	GOAPWorldState* goap_state_defend;
	// Unit gathering resource
	GOAPWorldState* goap_state_gather;
	// Unit building structire
	GOAPWorldState* goap_state_build;

	// Map of each GOAP Units current state, based on ID
	std::map<int, GOAPWorldState*> goap_current_state;

	// Units goal is to engage castle
	GOAPWorldState* goap_goal_kill_castle;
	// Units goal is to engage enemy
	GOAPWorldState* goap_goal_kill_enemy;
	// Units goal is to engage wall
	GOAPWorldState* goap_goal_kill_wall;

	// Units goal is to defend base
	GOAPWorldState* goap_goal_defend;
	// Units goal is to gather resource
	GOAPWorldState* goap_goal_gather;
	// Units goal is to build wall
	GOAPWorldState* goap_goal_build;

	// Map of GOAP Units current goal, based on ID
	std::map<int, GOAPWorldState*> goap_current_goal;

	// GOAP Plan for Units to conduct
	std::map<Unit*, std::vector<GOAPAction>> GOAP_Plan;
	// GOAP Planner to calculate path from curent state to target goal
	GOAPPlanner* GOAPSearch;

	//// MCTS VARIABLES

	// Map of MCTS Units current action, based on ID
	std::map<int, MCTSAction> MCTS_Action;
	// Map of MCTS Units current state, based on ID
	std::map<int, MCTSWorldState> MCTS_State;
	// MCTS Planner to calcuate next move based on available actions from current state
	MCTSPlanner* MCTSSearch;
};

