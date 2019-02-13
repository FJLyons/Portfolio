#pragma once

#include "Manager_Resource.h"

class MCTSAction
{
public:
	// Create MCTS Action
	// @param action - move action controls
	MCTSAction(ActionState action = ActionState::Idle);
	// Destructor
	~MCTSAction();

	// Return MCTS action name
	std::string GetName() const;
	// Return MCTS action state
	ActionState GetAction() const;

	// Create actions for use in search
	void SetUpAction();

	// Add child to current action
	void AddChild(ActionState child);

	// Return actions children
	std::vector<ActionState> GetChildren();

private:
	// Children of action
	std::vector<ActionState> m_children;
	// Action move 
	ActionState m_action;
};

