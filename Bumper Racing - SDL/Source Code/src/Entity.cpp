#include "Entity.h"



Entity::Entity(EntityType type)
	: _type(type)
{
}


Entity::~Entity()
{
	for (int i = 0; i < _components.size(); i++)
	{
		if (_components[i] != nullptr)
			delete _components[i];
	}
	_components.clear();
}


void Entity::AddComponent(Component* component)
{
	_components.push_back(component);
}
void Entity::RemoveComponent(Component::Type componentType)
{
	for (int i = 0; i < _components.size(); i++)
	{
		if (_components.at(i)->GetType() == componentType)
		{
			delete _components[i];
			_components.erase(_components.begin() + i);
			break;
		}
	}
}

Component* Entity::GetComponent(Component::Type componentType)
{
	for (int i = 0; i < _components.size(); i++)
	{
		if (_components.at(i)->GetType() == componentType)
		{
			return _components.at(i);
		}
	}
	return nullptr;
}

std::vector<Component*>& Entity::GetComponents()
{
	return _components;
}

EntityType Entity::GetType() const
{
	return _type;
}

std::string Entity::GetTypeAsString() const
{
	std::string s;
	switch (_type)
	{
	case EntityType::Flag:
		s = "Flag";
		break;
	case EntityType::Checkpoint:
		s = "Checkpoint";
		break;
	case EntityType::Tile:
		s = "Tile";
		break;
	case EntityType::Obstacle:
		s = "Obstacle";
		break;
	case EntityType::Bullet:
		s = "Bullet";
		break;
	case EntityType::PowerUp:
		s = "PowerUp";
		break;
	case EntityType::AI:
		s = "AI";
		break;
	case EntityType::RemotePlayer:
		s = "RemotePlayer";
		break;
	case EntityType::Player:
		s = "Player";
		break;
	case EntityType::Point:
		s = "Point";
		break;
	}
	return s;
}
