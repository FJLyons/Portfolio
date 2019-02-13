#pragma once

#include "Component.h"


class PowerUpComponent : public Component
{
public:
	enum class Type
	{
		Handgun,
		Shotgun,
		SMG,
		Invisibility,
		Speed,

		Count
	};

public:
	PowerUpComponent(Type t)
		: type(t)
		, collected(false)
		, Component::Component(Component::Type::PowerUp)
	{
	}

	~PowerUpComponent()
	{
	}

public:
	Type	type;
	int		waypointIndex;
	bool	collected;
};