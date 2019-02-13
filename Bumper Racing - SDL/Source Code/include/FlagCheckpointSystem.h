#pragma once

#include "InteractionSystem.h"
#include "InteractionSystemEvents.h"


class FlagCheckpointSystem : public InteractionSystem
{
public:
										FlagCheckpointSystem(std::map<InteractionSystemEvent, std::vector<std::pair<Entity*, Entity*>>>& events, float updateRate = 0.f);
										~FlagCheckpointSystem();

	void								Initialize(std::vector<Entity*>& checkpoints);

	void								Process(float dt = 0.f) override;

private:
	void								ListenForEvents();
	void								FlagPickedEvent();
	void								FlagDroppedEvent();

private:
	std::map<InteractionSystemEvent,
		std::vector<
		std::pair<Entity*, Entity*>>>&	_interactionSystemEvents;
	std::vector<Entity*>				_checkpoints;

	const InteractionSystemEvent		FLAG_PICKED;
	const InteractionSystemEvent		FLAG_DROPPED;
};

