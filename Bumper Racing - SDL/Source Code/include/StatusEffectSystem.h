#pragma once

#include "System.h"


class StatusEffectSystem : public System
{
public:
							StatusEffectSystem(float update = 0.f);
							~StatusEffectSystem();

	void					Process(float dt) override;

private:
	void					UpdateStatusEffect(bool& effect, float& effectTimer, float dt);
};