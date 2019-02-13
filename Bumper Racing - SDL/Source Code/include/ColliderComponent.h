#pragma once

#include "Component.h"
#include "Box2D\Box2D.h"


class ColliderComponent : public Component
{
public:
	ColliderComponent()
		: body(nullptr)
		, setActive(true)
		, checkpointCollision(std::pair<bool, int>(false, -1))
		, Component::Component(Component::Type::Collider)
	{
	}

	~ColliderComponent()
	{
	}

public:
	b2Body*					body;
	bool					setActive;
	std::pair<bool, int>	checkpointCollision;
};
