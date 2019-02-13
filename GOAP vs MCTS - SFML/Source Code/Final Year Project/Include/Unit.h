#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <map>
#include <vector>
#include <queue>

#include "Manager_Resource.h"
#include "Manager_Collision.h"

#include "GOAP_Action.h"
#include "MCTS_Action.h"

using namespace std;

class Unit
{
private:
	// Avoid unwanted construction
	Unit(const Unit&) = delete;
	// Avoid unwanted assignment
	Unit& operator=(const Unit&) = delete;

public:
	// Create Unit
	// @param side - Set side for Unit in game
	// @param type - Set type for Unit in game
	Unit(const Team& side, const UnitType& type);
	// Destructor
	~Unit();

	// Initialise Unit
	void Init();

	// Update Unit
	// @param currentState - action for Unit to perform
	// @param dt - time since last update
	int Update(const ActionState& currentState, const float dt);

	// Draw Unit
	// @param window - SFML window
	// @param debugLevel - Variation of debug information to draw
	void Draw(sf::RenderWindow& window, const int debugLevel);

private:
	//// VARIABLES

	// Unit Health and Current Action - for UI
	sf::Text m_text[2];

	// Units Current Position
	sf::Vector2f m_position;

	// Units Body - for drawing Unit and detecting collisions
	sf::CircleShape BODY;
	// Units Attack Range - for detecting collisions
	sf::CircleShape RANGE;
	// Units Sight Range - for detecting enemy Units and Structures
	sf::CircleShape FOV;


	// Units Type - Builder / Soldier / Archer
	UnitType m_type;

	// Units Side - GOAP / MCTS
	Team m_side;

	// Units current Target
	Target m_target;

	// Units current objective
	WorldState m_objective;


	// Actions for GOAP Unit
	std::vector<GOAPAction*>* m_GOAPActions;
	// GOAP Initial action
	GOAPAction* init_action;

	// Action for MCTS Unit
	MCTSAction* m_MCTSAction;

	// Return current action condition - for Update
	int m_working;
	// Units ID for collision checking
	int m_id;

	// Is Unit Alive?
	bool alive;

	// Units Attack Value
	float m_valueAttack;
	// Units Health
	float m_valueDefense;
	// Time for Unit to attack
	float m_attackTimer;
	// Units build value
	float m_value;
	// Units movement speed
	float m_speedMovement;
	// Units attack speed
	float m_speedAttack;
	// Units attack range
	float m_rangeAttack;
	// Units sight range
	float m_rangeSight;
	// Unit Radius - for Body / Range / FOV
	float m_radius;

	// Units attack sound
	sf::Sound s_attack;
	// Units attack sound buffer
	sf::SoundBuffer b_attack;

	//// FUNCTIONS

	// Initialise Circles - for drawing and collision detection
	void InitCircles();

	// Initialise Builder Unit
	void InitBuilder();
	// Initialise Soldier Unit
	void InitSoldier();
	// Initialise Archer Unit
	void InitArcher();

	// Set up GOAP Actions
	void CreateGOAPActions();
	// Set up MCTS Actions
	void CreateMCTSActions();

	// Unit is Idle
	int IsIdle();
	// Unit Finished its goal
	int Done();

	// Unit moving to castle
	int MoveToCastle();
	// Unit attacking castle
	// @param dt - Time since last update
	int AttackCastle(const float dt);

	// Unit moving to enemy
	int MoveToEnemy();
	// Unit attacking enemy
	// @param dt - Time since last update
	int AttackEnemy(const float dt);

	// Unit moving to wall
	int MoveToStructure();
	// Unit attacking wall
	// @param dt - Time since last update
	int AttackStructure(const float dt);

	// Unit moving to base
	int	MoveToBase();

	// Unit moving to resource
	int MoveToResource();
	// Unit gathering resource
	// @param dt - Time since last update
	int GatherResources(const float dt);

	// Unit moving to site
	int MoveToSite();
	// Unit building wall
	// @param dt - Time since last update
	int BuildStructure(const float dt);

public:

	//// GETTERS / SETTERS

	// Return Units alive status
	bool GetAlive() const;
	// Set Units alive status
	// @param isAlive - Alive status to be assigned to unit
	void SetAlive(const bool isAlive);

	// Return Units ID
	int GetID() const;
	// Set Units ID
	// @param newID - ID to be assigned to unit
	void SetID(const int newID);

	// Return Unit Objective
	WorldState GetObjective() const;
	// Set Units Objective
	// @param newObjective - Objective to be assigned to unit
	void SetObjective(const WorldState& newObjective);

	// Return Units Side
	Team GetSide() const;
	// Set Units Side
	// @param newSide - Side to be assigned to unit
	void SetSide(const Team& newSide);

	// Return Units Type
	UnitType GetType() const;
	// Set Units Type
	// @param newType - Type to be assigned to unit
	void SetType(const UnitType& newType);

	// Return GOAP Actions
	std::vector<GOAPAction*>* GetGOAPActions() const;

	// Return MCTS Action
	MCTSAction* GetMCTSAction() const;

	// Return Unit Position
	sf::Vector2f GetPosition() const;
	// Set Unit Position
	// @param newPosition - Position to be assigned to unit
	void SetPosition(const sf::Vector2f& newPosition);

	// Return Unit Body
	sf::CircleShape GetBody() const;

	// Return Unit Ranger
	sf::CircleShape GetRange() const;

	// Return Unit FOV
	sf::CircleShape GetFOV() const;

	// Return Unit Target
	Target GetTarget() const;
	// Set Units Target
	// @param newTarget - Target to be assigned to unit
	void SetTarget(const Target& newTarget);

	// Return Movement Speed
	float GetMovementSpeed() const;

	// Return Attack Timer
	float GetAttackTimer() const;
	// Set Units Attack Timer
	// @param newTime - Current time of timer
	void SetAttackTimer(const float newTime);

	// Return Attack Value
	float GetValue() const;

	// Return Attack Value
	float GetAttackValue() const;

	// Return Attack Value
	float GetDefenceValue() const;
	// Set Units Defence Value
	// @param newHealth - New Unit HP
	void SetDefenceValue(const float newHealth);
	// Remove Damage from current Defence Value
	// @param damage - damage to be removed
	void ReduceHealthBy(const float damage);
};

