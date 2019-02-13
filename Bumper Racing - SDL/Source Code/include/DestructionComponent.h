#pragma once

#include "Component.h"
#include "Box2D\Box2D.h"


class DestructionComponent : public Component
{
public:
	DestructionComponent()
		: destroy(false)
		, Component::Component(Component::Type::Destroy)
	{
	}

	~DestructionComponent()
	{
	}

public:
	bool destroy;
};