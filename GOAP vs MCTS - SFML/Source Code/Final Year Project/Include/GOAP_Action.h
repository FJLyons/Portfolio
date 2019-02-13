#pragma once

#include <string>
#include <unordered_map>

#include "Manager_Resource.h"

class GOAPWorldState;

class GOAPAction
{
public:
	// Create GOAP Acton
	GOAPAction(const ActionState& name, const int cost);
	// Destructor
	~GOAPAction();

private:
	// Name of action
	ActionState m_name;
	// Cost of action - for A Star
	int m_cost;

	// Map or preconditions for an action to be used
	std::unordered_map<ActionState, bool> m_preconditions;
	// Map of effects an action produces
	std::unordered_map<ActionState, bool> m_effects;

public:
	// Return action name
	std::string GetName() const;
	// return action cost
	int GetCost() const;

	// Set Preconditions for action
	// @param key - map key to identify action state
	// @value - map value for precondition
	void SetPreconditions(ActionState key, bool value);
	// Set effects of action
	// @param key - map key to identify action
	// @value - map value for effect
	void SetEffects(ActionState key, bool value);

	// Return Effects
	std::unordered_map<ActionState, bool> GetEffects() const;

	// Return action
	ActionState GetAction()const;

	// Check if action precondition allows for path
	bool OperableOn(const GOAPWorldState& worldState) const;
	// Set world state to equal current actin
	GOAPWorldState ActOn(const GOAPWorldState& worldState) const;

	// Return GOAP actions for search
	std::vector<GOAPAction*>* SetUpAction();
	// Actions for search
	std::vector<GOAPAction*>* actions;
};
