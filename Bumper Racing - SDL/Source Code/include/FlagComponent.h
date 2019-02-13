#pragma once

#include "Component.h"


class FlagComponent : public Component
{
public:
	FlagComponent()
		: currentCheckpointID(0)
		, totalCheckpoints(0)
		, currentLap(1)
		, hasFlag(false)
		, Component::Component(Component::Type::Flag)
	{
	}

	~FlagComponent()
	{
	}

public:
	int currentCheckpointID;
	int currentLap;

	bool hasFlag;

	int totalCheckpoints;
};