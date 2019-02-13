#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include<vector>

#include "Unit.h"
#include "Structure.h"
#include "Logger.h"

class Unit;


class CollisionManager
{
public:
	// Return an instance of the CollisionManager class
	static CollisionManager * GetInstance();
private:
	// CollisionManager instance object - for returning instance
	static CollisionManager * collisionManagerInstance;

	// Avoid unwanted construction
	CollisionManager(const CollisionManager&) = delete;
	// Avoid unwanted assignment
	CollisionManager& operator=(const CollisionManager&) = delete;

public:
	CollisionManager();
	~CollisionManager();


public:
	//// ENTITIES

	// GOAP Castle
	Structure* m_leftCastle;
	// MCTS Castle
	Structure* m_rightCastle;

	// Map of GOAP Units
	std::map<int, Unit*> m_leftUnits;
	// Map of MCTS Units
	std::map<int, Unit*> m_rightUnits;

	// Vector of GOAP Walls
	std::vector<Structure*> m_leftWalls;
	// Vector of MCTS Walls
	std::vector<Structure*> m_rightWalls;

	// Vector of GOAP Resources
	std::vector<Structure*> m_leftResources;
	// Vector of MCTS Resources
	std::vector<Structure*> m_rightResources;


	//// TARGETS

	// Map of Unit target units
	std::map<Unit*, Unit*> m_targetUnit;
	// Map of Unit target walls
	std::map<Unit*, Structure*> m_targetWall;

	// Map of Unit target resources
	std::map<Unit*, Structure*> m_targetResource;
	// Map of Unit target building sites
	std::map<Unit*, Structure*> m_targetSite;

	// Map to store unit damage to be dealt at the end of update
	std::map<Unit*, int> m_targetUnitDamage;

	// Map of position waypoints for movement
	std::map<Unit*, std::vector<sf::Vector2f>> m_targetPosition;

private:
	//// COUNTERS

	// Total left side value - for results
	float m_leftSideValue;
	// Total right side value - for results
	float m_rightSideValue;

	// Total left side attack value - for results
	float m_leftAttackValue;
	// Total right side attack value - for results
	float m_rightAttackValue;

	// Total left side defence value - for results
	float m_leftDefenceValue;
	// Total right side defence value - for results
	float m_rightDefenceValue;

	// Total left side gold - for results and building
	int m_leftGold;
	// Total left side gold - for results and building
	int m_rightGold;

	// Total left side builders alive - for goal weighting
	int m_leftBuilders;
	// Total left side soldiers alive - for goal weighting
	int m_leftSoldiers;
	// Total left side archers alive - for goal weighting
	int m_leftArchers;
	// Total left side walls alive - for goal weighting
	int m_leftWallsBuilt;

	// Total right side builders alive - for goal weighting
	int m_rightBuilders;
	// Total right side soldiers alive - for goal weighting
	int m_rightSoldiers;
	// Total right side archers alive - for goal weighting
	int m_rightArchers;
	// Total right side walls alive - for goal weighting
	int m_rightWallsBuilt;

	// Left side build queue - for walls
	int m_leftBuildingQueue;
	// Right side build queue - for walls
	int m_rightBuildingQueue;

public:
	//// HIGHEST VALUE COUNTERS

	// Highest left side value achieved - for results
	float m_leftSideValueHighest;
	// Highest right side value achieved - for results
	float m_rightSideValueHighest;

	// Time of Highest left side value achieved - for results
	float m_leftSideValueHighestSeconds;
	// Time of Highest right side value achieved - for results
	float m_rightSideValueHighestSeconds;

	// Highest left side attack value achieved - for results
	float m_leftAttackValueHighest;
	// Highest right side attack value achieved - for results
	float m_rightAttackValueHighest;

	// Highest left side defence value achieved - for results
	float m_leftDefenceValueHighest;
	// Highest right side attack value achieved - for results
	float m_rightDefenceValueHighest;

	// Total GOAP Builders made - for results
	int m_leftBuildersTotal;
	// Total GOAP Soldiers made - for results
	int m_leftSoldiersTotal;
	// Total GOAP Archers made - for results
	int m_leftArchersTotal;
	// Total GOAP Resources collected - for results
	int m_leftResourcesTotal;
	// Total GOAP Walls made - for results
	int m_leftWallsTotal;

	// Total MCTS Builders made - for results
	int m_rightBuildersTotal;
	// Total MCTS Soldiers made - for results
	int m_rightSoldiersTotal;
	// Total MCTS Archers made - for results
	int m_rightArchersTotal;
	// Total MCTS Resources collected - for results
	int m_rightResourcesTotal;
	// Total MCTS Walls made - for results
	int m_rightWallsTotal;

private:
	//// WEIGHTS

	// Weight for engage castle goal
	float w_engageCastle;
	// Weight for engage enemy goal
	float w_engageEnemy;
	// Weight for engage wall goal
	float w_engageStructure;

	// Weight for defend castle goal
	float w_defendCastle;
	// Weight for build wall goal
	float w_buildStructure;

	// Weight for gather resource goal
	float w_needResources;


public:
	////  MATHS FUNCTIONS

	// Calculate teams value
	// @param side - which team to calculate for
	// @param gameTime - total time passed since game start
	void CalculateValues(const Team& side, const float gameTime);
	// Normalise vector function
	// @param vec - vector to normalise
	sf::Vector2f Normalise(const sf::Vector2f& vec) const;
	// Apply all unit damage taken at the end of update
	void ApplyDamage();
	// Reset all collision manager variables and clear maps / vectors
	void Reset();

private:
	// Initialise all collision manager variables
	void InitVariables();
	// Set units to dead after damage dealt
	void CompareKills();

	// Find next target waypoint for moving left
	// @param id - id of unit moving
	// @param side - team of unit
	void PlanRouteLeft(const int id, const Team& side);
	// Find next target waypoint for moving right
	// @param id - id of unit moving
	// @param side - team of unit
	void PlanRouteRight(const int id, const Team& side);
	// Detect if Unit has reached target waypoint position
	// @param id - id of unit moving
	// @param side - team of unit
	void ReachedTarget(const int id, const Team& side);


public:
	///// CHECK WORLD 

	// Evaluate world for engaging castle
	// @param side - determine home and enemy team for comparison
	bool CheckWorld_EngageCastle(const Team& side);
	// Evaluate world for engaging enemy
	// @param side - determine home and enemy team for comparison
	bool CheckWorld_EngageEnemy(const Team& side);
	// Evaluate world for engaging wall
	// @param id - id for unit checking wall
	// @param side - determine home and enemy team for comparison
	bool CheckWorld_EngageStructure(const int id, const Team& side);

	// Evaluate world for defending castle
	// @param side - determine home and enemy team for comparison
	bool CheckWorld_DefendCastle(const Team& side);
	// Evaluate world for building wall
	// @param id - id for unit building wall
	// @param side - determine home and enemy team for comparison
	bool CheckWorld_BuildStructure(const int id, const Team& side);

	// Evaluate world for gathering resources
	// @param side - determine home and enemy team for comparison
	bool CheckWorld_NeedResources(const Team& side);


	//// CHECK WORLD OVERWRITERS

	// Evaluate world for all enemies dead
	// @param side - determine home and enemy team for comparison
	bool CheckWorld_EnemiesDead(const Team& side) const;
	// Evaluate world for all resources gone
	// @param side - determine home and enemy team for comparison
	bool CheckWorld_ResourcesGone(const Team& side) const;
	// Evaluate world for all enemy walls up
	// @param side - determine home and enemy team for comparison
	bool CheckWorld_WallsUp(const Team& side) const;
	// Evaluate world for all home walls up
	// @param side - determine home and enemy team for comparison
	bool CheckWorld_HomeWallsUp(const Team& side) const;
	// Evaluate world for unit touching wall
	// @param id - id for unit touching wall
	// @param side - determine home and enemy team for comparison
	bool CheckWorld_TouchingWall(const int id, const Team& side);

	// Request a wall to be build
	// @param side - determine home and enemy team for comparison
	void BuildStructureRequest(const Team& side);
	// Check if a request for a wall has been made
	// @param side - determine home and enemy team for comparison
	bool BuildStructureRequested(const Team& side);


	//// TARGETS

	// Target enemy unit
	// @param id - id of unit looking for a target
	// @param side - determine home and enemy team for comparison
	bool TargetEnemy(const int id, const Team& side);
	// Target enemy wall
	// @param id - id of unit looking for a target
	// @param side - determine home and enemy team for comparison
	bool TargetStructure(const int id, const Team& side);

	// Target resource
	// @param id - id of unit looking for a target
	// @param side - determine home and enemy team for comparison
	bool TargetResource(const int id, const Team& side);
	// Target building site
	// @param id - id of unit looking for a target
	// @param side - determine home and enemy team for comparison
	bool TargetSite(const int id, const Team& side);


	//// MOVE TO

	// Move to target castle
	// @param id - id of unit moving to target
	// @param side - determine home and enemy team for comparison
	int MoveToCastle(const int id, const Team& side);
	// Move to target enemy
	// @param id - id of unit moving to target
	// @param side - determine home and enemy team for comparison
	int MoveToEnemy(const int id, const Team& side);
	// Move to target wall
	// @param id - id of unit moving to target
	// @param side - determine home and enemy team for comparison
	int MoveToStructure(const int id, const Team& side);

	// Move to target base
	// @param id - id of unit moving to target
	// @param side - determine home and enemy team for comparison
	int MoveToBase(const int id, const Team& side);
	// Move to target resource
	// @param id - id of unit moving to target
	// @param side - determine home and enemy team for comparison
	int MoveToResource(const int id, const Team& side);
	// Move to target building site
	// @param id - id of unit moving to target
	// @param side - determine home and enemy team for comparison
	int MoveToSite(const int id, const Team& side);


	//// ENEMY DETECTION

	// Detect if enemy is in vacinity
	// @param id - id of unit checking
	// @param side - determine home and enemy team for comparison
	bool DetectedEnemy(const int id, const Team& side);
	// Detect if wall is in vacinity
	// @param id - id of unit checking
	// @param side - determine home and enemy team for comparison
	bool DetectedWall(const int id, const Team& side);


	//// TARGET REACHED

	// Detect if target castle reached
	// @param id - id of unit checking
	// @param side - determine home and enemy team for comparison
	bool ReachedCastle(const int id, const Team& side);
	// Detect if target enemy reached
	// @param id - id of unit checking
	// @param side - determine home and enemy team for comparison
	bool ReachedEnemy(const int id, const Team& side);
	// Detect if target wall reached
	// @param id - id of unit checking
	// @param side - determine home and enemy team for comparison
	bool ReachedStructure(const int id, const Team& side);

	// Detect if target base reached
	// @param id - id of unit checking
	// @param side - determine home and enemy team for comparison
	bool ReachedBase(const int id, const Team& side);
	// Detect if target resource reached
	// @param id - id of unit checking
	// @param side - determine home and enemy team for comparison
	bool ReachedResource(const int id, const Team& side);
	// Detect if target building site reached
	// @param id - id of unit checking
	// @param side - determine home and enemy team for comparison
	bool ReachedSite(const int id, const Team& side);


	//// ATTACK

	// Attack target castle
	// @param id - id of unit attacking
	// @param side - determine home and enemy team for comparison
	int AttackCastle(const int id, const Team& side);
	// Attack target enemy
	// @param id - id of unit attacking
	// @param side - determine home and enemy team for comparison
	int AttackEnemy(const int id, const Team& side);
	// Attack target wall
	// @param id - id of unit attacking
	// @param side - determine home and enemy team for comparison
	int AttackStructure(const int id, const Team& side);


	// WORK

	// Gather target resource
	// @param id - id of unit gathering
	// @param side - determine home and enemy team for comparison
	int GatherResource(const int id, const Team& side);
	// Build target wall
	// @param id - id of unit building
	// @param side - determine home and enemy team for comparison
	int BuildStructure(const int id, const Team& side);


public:
	//// MCTS

	// MCTS Idle evaluation
	int MCTS_Idle();

	// MCTS move to castle evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_MoveToCastle(const int id);
	// MCTS attack castle evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_AttackCastle(const int id);
	// MCTS castle dead evaluation
	int MCTS_CastleDead();

	// MCTS move to enemy evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_MoveToEnemy(const int id);
	// MCTS attack enemy evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_AttackEnemy(const int id);
	// MCTS enemy dead evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_EnemyDead(const int id);

	// MCTS move to wall evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_MoveToStructure(const int id);
	// MCTS attack wall evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_AttackStructure(const int id);
	// MCTS wall dead evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_StructureDead(const int id);

	// MCTS move to base evaluation
	int MCTS_MoveToBase();
	// MCTS base reached evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_BaseReached(const int id);

	// MCTS move to resource evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_MoveToResource(const int id);
	// MCTS gather resource evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_GatherResources(const int id);
	// MCTS resource collected evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_ResourcesCollected(const int id);

	// MCTS move to building site evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_MoveToSite(const int id);
	// MCTS build wall evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_BuildStructure(const int id);
	// MCTS wall built evaluation
	// @param id - id of MCTS unit evaluating
	int MCTS_StuctureBuilt(const int id);

public:
	// Return teams gold based on side
	// @param side - side of desired gold reserve
	int GetGold(const Team& side) const;
	// Reduce teams gold
	// @param side - side of desired gold reserve
	// @param cost - cost to reduce gold by
	void SpendGold(const Team& side, const int cost);

	// Return teams value based on side
	// @param side - side of desired gold reserve
	float GetSideValue(const Team& side) const;

	// Return teams attack value based on side
	// @param side - side of desired attackers
	float GetAttackValue(const Team& side) const;

	// Return teams defence value based on side
	// @param side - side of desired defenders
	float GetDefenceValue(const Team& side) const;

	// Return number of units based on type
	// @param side - side of desired units
	// @param type - desired units type 
	float GetUnitCount(const Team& side, const UnitType& type) const;

	// Return walls built for a side
	// @param side - side of desired walls
	float GetWallCount(const Team& side) const;

	// Return weight of goal
	// @param worldState - desired goal weight
	float GetWeight(const WorldState& worldState) const;
};

