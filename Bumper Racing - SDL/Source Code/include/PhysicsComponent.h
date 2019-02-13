#pragma once

#include "Component.h"


class PhysicsComponent : public Component
{
public:
	PhysicsComponent(float xV, float yV, float xA, float yA, float maxVel)
		: xVelocity(xV)
		, yVelocity(yV)
		, xAcceleration(xA)
		, yAcceleration(yA)
		, maxVelocity(maxVel)
		, xDir(0)
		, yDir(0)
		, Component::Component(Component::Type::Physics)
	{
	}

	~PhysicsComponent() {}

public:
	float	xVelocity;
	float	yVelocity;

	float	xAcceleration;
	float	yAcceleration;

	float	xDir;
	float	yDir;
	
	float	maxVelocity;
};