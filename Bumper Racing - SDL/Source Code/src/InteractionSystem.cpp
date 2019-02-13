#include "InteractionSystem.h"



InteractionSystem::InteractionSystem(float updateRate)
	: _canUpdate(false)
	, _updateRate(updateRate)
{
}


InteractionSystem::~InteractionSystem()
{
}


void InteractionSystem::AddEntity(Entity* gameObject, Entity* otherObject)
{
	_entities.push_back(std::pair<Entity*, Entity*>(gameObject, otherObject));
}

void InteractionSystem::RemoveEntity(Entity* gameObject, bool firstObject)
{
	if (firstObject)
	{
		for (int i = 0; i < _entities.size(); i++)
		{
			if (_entities.at(i).first == gameObject)
			{
				_entities.erase(_entities.begin() + i);
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < _entities.size(); i++)
		{
			if (_entities.at(i).second == gameObject)
			{
				_entities.erase(_entities.begin() + i);
				break;
			}
		}
	}
}

void InteractionSystem::Process(float dt)
{
	_timer += dt;
	if (_timer >= _updateRate)
	{
		_timer = 0.f;
		_canUpdate = true;
	}
}