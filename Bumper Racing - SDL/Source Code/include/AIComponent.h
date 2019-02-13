#pragma once

#include "Component.h"
#include "GraphNode.h"
#include "AIState.h"

class AIComponent : public Component
{
public:
	AIComponent()
		: Component::Component(Component::Type::AI),
		path(vector<GraphNode*>()),
		pathfinderUpdateTimer(0),
		nextNode(nullptr),
		state(AIState::Entry),
		lastEntityWithFlag(nullptr),
		avoidanceSeekTimer(0),
		avoidanceSeekForce(0),
		avoidanceChaseTimer(0),
		avoidanceChaseForce(0),
		avoidanceCheckpointTimer(0),
		avoidanceCheckpointForce(0)
	{
	}

	~AIComponent()
	{
	}

public:
	vector<GraphNode*> path;
	float pathfinderUpdateTimer;
	GraphNode* destNode;
	GraphNode* nextNode;

	AIState state;

	Entity* lastEntityWithFlag;

	//for learning AI
	float avoidanceSeekTimer;
	float avoidanceSeekForce;

	float avoidanceChaseTimer;
	float avoidanceChaseForce;

	float avoidanceCheckpointTimer;
	float avoidanceCheckpointForce;


	
};
