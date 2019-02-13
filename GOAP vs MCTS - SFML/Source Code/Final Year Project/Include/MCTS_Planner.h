#pragma once

#include "MCTS_Node.h"
#include <cfloat>
#include <chrono>

class MCTSPlanner
{
private:
	// Avoid unwanted construction
	MCTSPlanner(const MCTSPlanner&) = delete;
	// Avoid unwanted assignment
	MCTSPlanner& operator=(const MCTSPlanner&) = delete;

public:
	MCTSPlanner();
	~MCTSPlanner();

	// MCTS UCT Search Algorithm
	// @param currentState - state to define nodes potential actions and paths
	MCTSAction Plan(MCTSWorldState& currentState);

private:
	//// VARIABLES 
	// Number of iterations in UCT search
	int m_iterations;
	// k value in UCT function - default = sqrt(2)
	float m_uct_k;		
	// Maximum iterations allowed in search
	int m_maxIterations;		
	// Time limit for searching
	std::chrono::milliseconds m_timeLimit; 
	// How many tics to run simulation for
	int m_simulationDepth;		

	// Return iteraions
	const int GetIterations() const;

	// Run through a UCT search
	// @param currentState - state to define nodes potential actions and paths
	// @param rootNode - start node of search, created from current state
	float DoIteration(MCTSWorldState& currentState, MCTSNode* root_node);

	// Test available nodes to find node with best cost and return
	// @param node - current node to test
	// @param uct_k - UCT cost seed
	MCTSNode* GetBestUCTChild(MCTSNode* node, float uct_k) const;

	// Return most visited child node
	// @param node - current node to test
	MCTSNode* GetMostVisitedChild(MCTSNode* node) const;


	// Nodes to delete at the end of the game
	std::vector<MCTSNode*> ToDelete;
};

