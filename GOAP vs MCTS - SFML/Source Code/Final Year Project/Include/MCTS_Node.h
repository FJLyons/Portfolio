#pragma once

#include <memory>
#include <math.h>
#include <vector>
#include <algorithm>

#include "MCTS_WorldState.h"
#include "MCTS_Action.h"

class MCTSNode
{
private:
	// Avoid unwanted construction
	MCTSNode(const MCTSNode&) = delete;
	// Avoid unwanted assignment
	MCTSNode& operator=(const MCTSNode&) = delete;

public:
	// Shared pointer - for node children
	typedef std::shared_ptr< MCTSNode> Ptr;

public:
	// Create MCTS Node
	// @param
	MCTSNode(const MCTSWorldState& state, MCTSNode* parent = NULL);
	// Destructor
	~MCTSNode();

	// Expand Node as part of MCTS search for best child
	MCTSNode* Expand();

	// Update value of node for MCTS best child
	// @param rewards - value to add to node
	void Update(float & rewards);

	// Return Nodes state
	const MCTSWorldState& GetState() const;
	// Return Nodes action
	const MCTSAction& GetAction() const;

	// Check to see if node is fully expanded
	bool IsFullyExpanded() const;
	// Check to see if node is terminal
	bool IsTerminal() const;
	// Return number of visits to node
	int GetNumVisits() const;
	// Return Node value
	float GetValue() const;
	// Return node depth
	int GetDepth() const;
	// Return number of children in node
	int GetNumChildren() const;

	// Return Nodes child
	// @param i - index of chid to get
	MCTSNode* GetChild(int i) const;
	// Return nodes parent
	MCTSNode* GetParent() const;

private:
	//// VARIABLES 

	// The state of this Node
	MCTSWorldState m_state;	
	// The action which led to the state of this Node
	MCTSAction m_action;	
	// Parent of this Node
	MCTSNode* m_parent;		

	// Number of times node has been visited
	int m_num_visits;		
	// Value of this node
	float m_value;	
	// Depth of this node
	int m_depth;

	// Total current children
	std::vector< Ptr > m_children;		
	// Possible actions from this state
	std::vector< MCTSAction > m_actions;	

	// Create a clone of the current state, apply action, and add as child
	// @param newAction - create a new child with a specific action
	MCTSNode* AddChildWithAction(const MCTSAction& newAction);
};

