#pragma once

#include "GOAP_Action.h"
#include "GOAP_Node.h"
#include "GOAP_WorldState.h"

#include <ostream>
#include <unordered_map>
#include <vector>

class GOAPPlanner
{
private:
	// Avoid unwanted construction
	GOAPPlanner(const GOAPPlanner&) = delete;
	// Avoid unwanted assignment
	GOAPPlanner& operator=(const GOAPPlanner&) = delete;

public:
	// Create GOAP Planner for A* searc
	GOAPPlanner();
	// Destructor
	~GOAPPlanner();

private:
	// A Star open list
	std::vector<GOAPNode> m_open;
	// A Star closed list
	std::vector<GOAPNode> m_closed;

	// Calculate heuristic cost for search
	// @param now - current GOAP world state
	// @param goal - GOAP world state search is looking for
	int CalculateHeuristic(const GOAPWorldState& now, const GOAPWorldState& goal) const;

	// Add to open list and maintain order
	// @param node - node to add to open list
	void AddToOpenList(GOAPNode&& node);

	// Pop node from open list and add to closed list
	GOAPNode& PopAndClose();

	// Check if node is in closed list
	// @param worldState - world state for node comparison
	bool MemberOfClosed(const GOAPWorldState& worldState) const;
	// Check if node is in open list
	// @param worldState - world state for node comparison
	std::vector<GOAPNode>::iterator MemberOfOpen(const GOAPWorldState& worldState);

public:
	// A Star Search to find goal from current state
	// @param start - start of path
	// @param goal - end of path
	// @param actions - available actions to form path between start and goal
	std::vector<GOAPAction> Plan(const GOAPWorldState& start, const GOAPWorldState& goal, const std::vector<GOAPAction*>& actions);
};

