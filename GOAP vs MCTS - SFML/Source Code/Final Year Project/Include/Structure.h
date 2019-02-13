#pragma once

// SFML Headers
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "Manager_Resource.h"

class Structure
{
private:
	// Avoid unwanted construction
	Structure(const Structure&) = delete;
	// Avoid unwanted assignment
	Structure& operator=(const Structure&) = delete;

public:
	// Create Castle
	// @param side - Set side for Structure in game
	// @param type - Set type for Structure in game
	Structure(const Team& side, const StructureType& type);
	// Create Structure
	// @param side - Set side for Structure in game
	// @param type - Set type for Structure in game
	// @param position - Set position for Structure in game
	Structure(const Team& side, const StructureType& type, const sf::Vector2f& position);
	// Destructor
	~Structure();

	// Initialise Structure
	void Init();

	// Update Structure
	void Update();

	// Draw Structure
	// @param window - SFML window
	// @param debugLevel - Variation of debug information to draw
	void Draw(sf::RenderWindow &window, const int debugLevel);

private:
	//// VARIABLES

	// Structure Health - for UI
	sf::Text m_text;

	// Structures Body - for drawing Structure and detecting collisions
	sf::RectangleShape m_rectangleShape;

	// Size of Structure
	sf::Vector2f m_size;
	// Structure origin
	sf::Vector2f m_sizeHalf;
	// Structures Current Position
	sf::Vector2f m_position;

	// Structures Type - Castle / Wall / Resource
	StructureType m_type;

	// Structures Side - GOAP / MCTS
	Team m_side;

	// Is Structure Alive?
	bool alive;
	// Is Structure being built?
	bool m_building;

	// Structures build value
	float m_value;
	// Structures Health
	float m_valueDefense;
	// Structures build time
	float m_buildTime;

	//// FUNCTIONS

	// Initialise Castle Structure
	void InitCastle();
	// Initialise Wall Structure
	void InitWall();
	// Initialise Resource Structure
	void InitResource();

public:

	//// GETTERS / SETTERS

	// Return Structures alive status
	bool GetAlive() const;
	// Set Units alive status
	// @param isAlive - Alive status to be assigned to structure
	void SetAlive(const bool isAlive);

	// Return Structures building status
	bool GetBuilding() const;
	// Set Units alive status
	// @param isBuilding - Building status to be assigned to structure
	void SetBuilding(const bool isBuilding);

	// Return Structure Position
	sf::Vector2f GetPosition() const;
	// Set Structure Position
	// @param newPosition - Position to be assigned to structure
	void SetPosition(const sf::Vector2f& newPosition);

	// Return Attack Value
	float GetValue() const;

	// Return Attack Value
	float GetDefenceValue() const;
	// Set Structures Defence Value
	// @param newHealth - New Structure HP
	void SetDefenceValue(const float newHealth);
	// Remove Damage from current Defence Value
	// @param damage - damage to be removed
	void ReduceHealthBy(const float damage);
	// Increase Health from current building work
	// @param work - work health to be added
	void IncreaseHealthBy(const float work);

	// Return Structure Size
	sf::Vector2f GetSize() const;

	// Return Structures Body
	sf::RectangleShape GetBody() const;

	// Calculate and return a radius for structures
	float GetRadius() const;
};

