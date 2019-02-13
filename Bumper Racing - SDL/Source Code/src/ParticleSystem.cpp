#include "ParticleSystem.h"
#include "TransformComponent.h"

ParticleSystem::ParticleSystem(float updateRate)
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Initialize()
{

}

void ParticleSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				ParticleComponent* particle = static_cast<ParticleComponent*>(e->GetComponent(Component::Type::Particle));
				particle->m_particleManagers[0].TimedTurnOn(1.5f);

				TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));

				for (int j = 0; j < particle->m_particleManagers.size(); ++j)
				{
					ParticleManager * particleManager = &particle->m_particleManagers[j];

					particleManager->update(dt, Vector2b(transform->rect.x, transform->rect.y));
				}
			}
		}
	}
}

void ParticleSystem::Render(SDL_Renderer*& renderer, Camera2D::Camera* camera)
{

	for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
	{
		for (Entity* e : (*it).second)
		{
			ParticleComponent* particle = static_cast<ParticleComponent*>(e->GetComponent(Component::Type::Particle));
			particle->m_particleManagers[0].TimedTurnOn(1.5f);


			for (int j = 0; j < particle->m_particleManagers.size(); ++j)
			{
				ParticleManager * particleManager = &particle->m_particleManagers[j];

				particleManager->render(renderer, camera);
			}
		}
	}
}