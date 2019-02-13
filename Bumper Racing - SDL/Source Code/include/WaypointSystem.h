#pragma once
#include "System.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "Graph.h"
#include "InteractionSystemEvents.h"

using namespace helper;
class WaypointSystem : public System
{
public:
										WaypointSystem(std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests, std::map<InteractionSystemEvent, std::vector<std::pair<Entity*, Entity*>>>& interactionSystemEvents, float updateRate = 0.f);
										~WaypointSystem();

	void								Initialize(Graph* waypoint);
	void								Process(float dt) override;

private:
	void								CreatePowerUp(float dt);

	void								ListenForEvents();
	void								PowerUpDestructionEvent();

private:
	Graph*								_waypoints;
	float								_powerUpTimer;


	std::vector<
		std::pair<EntityType,
			std::vector<float>>>&		_creationRequests;
	std::map<InteractionSystemEvent,
		std::vector<
		std::pair<Entity*, Entity*>>>&  _interactionSystemEvents;

	int									_powerUpCount;
	const InteractionSystemEvent		POWER_UP_DESTRUCTION;
};