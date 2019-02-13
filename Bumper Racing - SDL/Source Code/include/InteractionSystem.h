#pragma once

#include "Entity.h"

#include <map>


class InteractionSystem
{
public:
	typedef std::vector<std::pair<Entity*, Entity*>>::iterator EntityMapIterator;

									InteractionSystem(float updateRate = 0.f);
									~InteractionSystem();

	virtual void					AddEntity(Entity* gameObject, Entity* otherObject);
	virtual void					RemoveEntity(Entity* gameObject, bool firstObject);

	virtual void					Process(float dt = 0.f);

protected:
	std::vector<
			std::pair<Entity*, 
					Entity*>>		_entities;
	float							_updateRate;
	float							_timer;
	bool							_canUpdate;
};

