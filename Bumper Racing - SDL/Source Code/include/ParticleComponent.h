#pragma once
#include "Component.h"
#include "ParticleManager.h"

class ParticleComponent : public Component
{
public: 
	
	ParticleComponent(std::vector<Vector2b *> posVec,std::vector<ParticleManager::ParticleSettings> settingsVec, SDL_Renderer * renderer = nullptr)
		: Component::Component(Component::Type::Particle)
	{
		for (int i = 0; i < posVec.size(); ++i)
		{
			ParticleManager::ParticleSettings settings = settingsVec[i];
			settings._positionToParentTo = posVec[i];

			m_particleManagers.push_back(ParticleManager(settings, renderer));
		}
	}	
	

	std::vector<ParticleManager> m_particleManagers;
};
