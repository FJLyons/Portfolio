#include "DestructionSystem.h"
#include "DestructionComponent.h"
#include "ColliderComponent.h"


DestructionSystem::DestructionSystem(float update)
{
}


DestructionSystem::~DestructionSystem()
{
}


void DestructionSystem::Initialize(b2World* b2world)
{
	_b2world = b2world;
}

void DestructionSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;

		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (int i = 0; i < (*it).second.size(); i++)
			{
				Entity*& e = (*it).second.at(i);
				DestructionComponent* destroy = static_cast<DestructionComponent*>(e->GetComponent(Component::Type::Destroy));

				if (destroy->destroy)
				{
					_toBeDestroyed.push_back(e);
					(*it).second.erase((*it).second.begin() + i);
					i--;
				}
			}
		}
	}
}

void DestructionSystem::DestroyEntity()
{
	if (!_toBeDestroyed.empty())
	{
		int index = _toBeDestroyed.size() - 1;
		
		ColliderComponent* collider = static_cast<ColliderComponent*>(_toBeDestroyed.at(index)->GetComponent(Component::Type::Collider));

		if (collider != nullptr)
		{
			_b2world->SetAllowSleeping(true);
			_b2world->DestroyBody(collider->body);
			_b2world->SetAllowSleeping(false);
		}

		delete _toBeDestroyed.at(index);
		_toBeDestroyed.at(index) = nullptr;

		_toBeDestroyed.pop_back();
	}
}

std::vector<Entity*>& DestructionSystem::GetEntitiesToBeDestroyed()
{
	return _toBeDestroyed;
}