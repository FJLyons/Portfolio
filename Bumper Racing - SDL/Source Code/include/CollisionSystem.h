#pragma once

#include "System.h"
#include "Box2D\Box2D.h"
#include "InteractionSystemEvents.h"
#include "AudioManager.h"


class CollisionSystem : public System, public b2ContactListener
{
public:
				CollisionSystem(std::map<InteractionSystemEvent, std::vector<std::pair<Entity*, Entity*>>>& events, std::vector<std::pair<EntityType, std::vector<float>>>& requests, float updateRate = 0.f);
				~CollisionSystem();

	void		Process(float dt = 0.f) override;

	void		BeginContact(b2Contact* contact) override;
	void		EndContact(b2Contact* contact) override;

	void		PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	void		PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

private:
	void		CheckCharacterToCheckpointCollision(Entity*& player, Entity*& other);
	void		CheckCharacterToBulletCollision(Entity*& player, Entity*& other);
	void		CheckCharacterToFlagCollision(Entity*& player, Entity*& other);
	void		CheckCharacterToPowerUpCollision(Entity*& player, Entity*& other);

	void		CheckCharacterToObjectCollision(Entity*& player, Entity*& other);
	void		CheckCharacterToCharacterCollision(Entity*& player, Entity*& other);

	void		FindPlayer(b2Contact* contact, Entity*& player, Entity*& other);
	void		CharacterObstacleBounce(b2Contact * contact, Entity*& player);
	void		EntityBounce(b2Contact * contact, Entity*& player, Entity*& other);

private:
	std::map<InteractionSystemEvent, std::vector<std::pair<Entity*, Entity*>>>&	_interactionSystemEvents;
	std::vector<std::pair<EntityType, std::vector<float>>>&						_creationRequests;

	AudioManager*																_audioManager = AudioManager::GetInstance();
};

