#pragma once

#include "System.h"

#include "Box2D\Box2D.h"


class DestructionSystem : public System
{
public:
							DestructionSystem(float update = 0.f);
							~DestructionSystem();

	void					Initialize(b2World* b2world);

	void					Process(float dt = 0.f) override;

	void					DestroyEntity();

	std::vector<Entity*>&	GetEntitiesToBeDestroyed();

private:
	std::vector<Entity*>	_toBeDestroyed;
	b2World*				_b2world;
};

