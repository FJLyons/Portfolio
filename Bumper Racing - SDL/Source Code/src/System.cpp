#include "System.h"



System::System(float updateRate)
	: _canUpdate(false)
	, _updateRate(updateRate)
{
}


System::~System()
{
}


void System::AddEntity(Entity* gameObject)
{
	_entities[gameObject->GetType()].push_back(gameObject);
}

void System::RemoveEntity(EntityType tag, Entity* gameObject)
{
	for (int i = 0; i < _entities[tag].size(); i++)
	{
		if (_entities[tag].at(i) == gameObject)
		{
			_entities[tag].erase(_entities[tag].begin() + i);
			break;
		}
	}
}

void System::Process(float dt)
{
	_timer += dt;
	if (_timer >= _updateRate)
	{
		_timer = 0.f;
		_canUpdate = true;
	}
}
