#pragma once

#include <vector>
#include <string>

#include "MCTS_Action.h"

class MCTSWorldState
{
public:
	// Default creation of MCTS World State
	MCTSWorldState();
	// Create MCTS World State with a specific action
	// @param action - Current action to evaluate the state of
	MCTSWorldState(const MCTSAction& action);
	// Destructor
	~MCTSWorldState();

	// Return if state is terminal - end node in branch
	bool IsTerminal() const;

	// Simulate current action
	// @param action - action to simulate
	float SimulateAction(const MCTSAction& action);

	// Return available actions from state
	// @param actions - actions to manipulate and return to UCT search
	void GetActions(std::vector<MCTSAction>& actions) const;

	// Return random action - if available
	// @param action - random action to manipulate and return to UCT search
	bool GetRandomAction(MCTSAction& action) const;

	// evaluate this state and return a vector of rewards (for each agent)
	const float Evaluate();

private:
	//// VARIABLES

	// Unit ID - for evaulation
	int m_id;

	// Is state the final node in a branch?
	bool m_isTerminal;
	// Is the state a winning condition?
	int m_winner;
	// Moves available to nodes
	std::vector<ActionState> m_moves;

	// Previous nodes action
	MCTSAction m_previousAction;

	//// Objects

	// Reset state
	void reset();

public:
	// Return State ID - corrosponds to Unit ID
	int GetID();
	// Set States ID
	// @param newID - ID to be assigned to state
	void SetID(const int newID);
};