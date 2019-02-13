#pragma once

#include "Component.h"


class GunComponent : public Component
{
public:
	GunComponent(float mt, int identification)
		: maxTimer(mt)
		, id(identification)
		, ammo(0)
		, fireTimer(0)
		, canFire(true)
		, triggered(false)
		, Component::Component(Component::Type::Gun)
	{
	}

	GunComponent(float mt, int a, int identification)
		: maxTimer(mt)
		, ammo(a)
		, id(identification)
		, fireTimer(0)
		, canFire(true)
		, triggered(false)
		, Component::Component(Component::Type::Gun)
	{
	}

	~GunComponent()
	{
	}

public:
	int ammo;
	int id;

	float fireTimer;
	float maxTimer;

	bool canFire;
	bool triggered;
};