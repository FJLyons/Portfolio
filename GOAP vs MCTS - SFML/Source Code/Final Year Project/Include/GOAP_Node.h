#pragma once

#include "GOAP_Action.h"
#include "GOAP_WorldState.h"

class GOAPNode
{
public:
	// Create Node
	GOAPNode();
	// Destructor
	~GOAPNode();

	// Create Node with values
	// @param state - world state of node - for a star comparison
	// @param g - g cost - for a star
	// @param h - h cost - for a star
	// @param parent_id - id of parent node
	// @param action - action available to use from node
	GOAPNode(const GOAPWorldState state, int g, int h, int parent_id, const GOAPAction* action);

	// Incrementing IDs to nodes
	static int m_previous_id;		

	// The state of the world at this node.
	GOAPWorldState m_worldState;
	// Node ID
	int m_id;	
	// The ID of this nodes immediate predecessor
	int m_parent_id;					
	// The A* cost from 'start' to 'here'
	int m_g;			
	// The estimated remaining cost to 'goal' form 'here'
	int m_h;		
	// The action that got us here 
	const GOAPAction* m_action;			

	// F = G + H
	int f() const 
	{
		return m_g + m_h;
	}
};

// Override comparitor operator
bool operator<(const GOAPNode& lhs, const GOAPNode& rhs);

