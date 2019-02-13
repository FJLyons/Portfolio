#pragma once

#include "System.h"


class PhysicsSystem : public System
{
public:
				PhysicsSystem(float updateRate = 0.f);
				~PhysicsSystem();

	void		Process(float dt) override;

	void		UpdateFlag(Entity*& e, float dt);
	void		UpdateOther(Entity*& e, float dt);
};

