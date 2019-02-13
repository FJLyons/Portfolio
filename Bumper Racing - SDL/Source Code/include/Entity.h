#pragma once

#include "Component.h"

#include <vector>
#include "EntityType.h"

class Entity
{
public:
								Entity(EntityType type);
								~Entity();

	void						AddComponent(Component* component);
	void						RemoveComponent(Component::Type componentType);

	Component*					GetComponent(Component::Type componentType);
	std::vector<Component*>&	GetComponents();
	EntityType					GetType() const;
	std::string					GetTypeAsString() const;

private:
	std::vector<Component*>		_components;
	EntityType					_type;
};

