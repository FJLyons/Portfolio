#pragma once

#include <ostream>
#include <string>
#include <map>

#include "Manager_Resource.h"

class GOAPWorldState
{
public:
	// Create GOAP World State
	// @param name - name of world state
	GOAPWorldState(const std::string name = "");
	// destructor
	~GOAPWorldState();

	// Prioritise goal states
	float m_priority; 
	// State name
	std::string m_name; 
	// Map of variables that describe worldstate
	std::map<int, bool> m_variables; 

	// Return World State variables
	// @param var_id - key for map of variables
	bool GetVariable(const int var_id) const;
	// Set World State variables
	// @param var_id - key for map of variables
	// @param value - value for variables in map
	void SetVariable(const int var_id, const bool value);

	// Compare variables to detect goal state
	bool MeetsGoal(const GOAPWorldState& goal_state) const;
	// find path to goal using variables
	int DistanceTo(const GOAPWorldState& goal_state) const;
	// Operator overloader
	bool operator==(const GOAPWorldState& otherWorld) const;
};
