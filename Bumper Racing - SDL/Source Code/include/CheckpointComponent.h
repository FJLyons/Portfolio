#pragma once

#include "Component.h"


class CheckpointComponent : public Component
{
public:
	CheckpointComponent(int i = -1)
		: id(i)
		, highlighted(false)
		, Component::Component(Component::Type::Checkpoint)
	{
	}

	~CheckpointComponent()
	{
	}

public:
	int id;

	bool highlighted;
};