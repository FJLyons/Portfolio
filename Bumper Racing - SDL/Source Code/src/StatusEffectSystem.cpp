#include "StatusEffectSystem.h"

#include "StatusEffectComponent.h"

StatusEffectSystem::StatusEffectSystem(float update)
	: System(update)
{
}

StatusEffectSystem::~StatusEffectSystem()
{
}

void StatusEffectSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				StatusEffectComponent* statusEffects = static_cast<StatusEffectComponent*>(e->GetComponent(Component::Type::StatusEffect));
				
				UpdateStatusEffect(statusEffects->staggered, statusEffects->staggeredTimer, dt);
				UpdateStatusEffect(statusEffects->speedUp, statusEffects->speedUpTimer, dt);
				UpdateStatusEffect(statusEffects->invisible, statusEffects->invisibleTimer, dt);
			}
		}
	}
}

void StatusEffectSystem::UpdateStatusEffect(bool& effect, float& effectTimer, float dt)
{
	if (effect)
	{
		if (effectTimer < 0)
		{
			effect = false;
			effectTimer = 0;
		}
		else
		{
			effectTimer -= dt;
		}
	}
}