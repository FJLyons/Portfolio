#pragma once

#include "System.h"
#include "PhysicsSystem.h"

#include "ConstHolder.h"
#include "Helpers.h"
#include "Graph.h"

#include "AIComponent.h"
#include "TransformComponent.h" 
#include "FlagComponent.h"

using namespace helper;
class AISystem : public System
{
public:
			AISystem(float updateRate = 0.f);
			~AISystem();

	void Initialize(Graph* waypoints, Entity* player);
	void Process(float dt = 0.f) override;

private:
	Graph* _waypoints;
	GraphNode* _flagNode;
	vector<GraphNode*> _checkpointNode;
	vector<Entity*> _players;

	void Entry(AIComponent* ai, TransformComponent* t);

	void SeekCheckpoint(AIComponent* ai, FlagComponent* f, TransformComponent* t);
	
	GraphNode* FindClosestPowerUp(helper::Vector2& velocity, AIComponent* ai, TransformComponent* t);

	void AStarUpdate(float dt, AIComponent* ai,  helper::Vector2& AIPosition);
	void AStar(AIComponent* ai);
	GraphNode* FindClosestNode(AIComponent* ai, TransformComponent* t);

	void TranverseNode(AIComponent* ai, TransformComponent* t);
	void Prediction(Entity* entityWithFlag, AIComponent* ai, TransformComponent* t);

	void Avoidance(Entity* self, helper::Vector2& velocity, const helper::Vector2& AIPosition, float force);
	helper::Vector2 CalculateAvoidanceForce(const helper::Vector2& AIPosition, const helper::Vector2& otherPosition, float force);

	Entity* FindEntityWithFlag(Entity* self);

	void UpdateLearning(float dt, AIComponent* ai);
};

