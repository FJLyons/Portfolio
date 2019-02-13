#pragma once

#include "Entity.h"
#include "SystemTypes.h"

#include "SDL_rect.h"
#include "EntityFactory.h"
#include "BodyFactory.h"

#include "TransformComponent.h"
#include "PhysicsComponent.h"

class CreationSystem
{
public:
	typedef std::map<EntityType, std::vector<Entity*>>::iterator EntityMapIterator;

public:
												CreationSystem(std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests);
												~CreationSystem();

	void										Initialize(EntityFactory* entityFactory, BodyFactory* bodyFactory);

	void										Process(float dt);

	void										EntityToSystemAssigned();

	void										RemoveEntitiy(EntityType type, Entity* e);

	bool										Empty();
	std::pair<
		std::vector<SystemType>, Entity*>		GetSystemCreatedEntity() const;

private:
	std::pair<std::vector<SystemType>, Entity*>	SetupWeaponEntity(const std::pair<EntityType, std::vector<float>>& information);
	std::pair<std::vector<SystemType>, Entity*>	SetupPlayerEntity(const std::pair<EntityType, std::vector<float>>& information);
	std::pair<std::vector<SystemType>, Entity*>	SetupRemotePlayerEntity(const std::pair<EntityType, std::vector<float>>& information);
	std::pair<std::vector<SystemType>, Entity*>	SetupAIEntity(const std::pair<EntityType, std::vector<float>>& information);
	std::pair<std::vector<SystemType>, Entity*>	SetupPowerUpEntity(const std::pair<EntityType, std::vector<float>>& information);
	std::pair<std::vector<SystemType>, Entity*>	SetupBulletEntity(const std::pair<EntityType, std::vector<float>>& information);
	std::pair<std::vector<SystemType>, Entity*>	SetupCheckpointEntity(const std::pair<EntityType, std::vector<float>>& information);
	std::pair<std::vector<SystemType>, Entity*>	SetupFlagEntity(const std::pair<EntityType, std::vector<float>>& information);
	std::pair<std::vector<SystemType>, Entity*>	SetupTileEntity(const std::pair<EntityType, std::vector<float>>& information);
	std::pair<std::vector<SystemType>, Entity*> SetupUIEntity(const std::pair<EntityType, std::vector<float>>& information);
	std::pair<std::vector<SystemType>, Entity*> SetupRadarEntity(const std::pair<EntityType, std::vector<float>>& information);

	void										SetupPosition(TransformComponent*& transform, const std::vector<float> &information, int& index);
	void										SetupSize(TransformComponent*& transform, const std::vector<float> &information, int& index);
	void										SetupAngle(TransformComponent*& transform, const std::vector<float> &information, int& index);
	void										SetupVelocity(PhysicsComponent*& physics, const std::vector<float> &information, int& index);

private:
	std::vector<
		std::pair<EntityType, 
			std::vector<float>>>&				_creationRequests;

	std::vector<
		std::pair<
		std::vector<SystemType>, Entity*>>		_systemCreatedEntities;

	EntityFactory*								_entityFactory;
	BodyFactory*								_bodyFactory;

	std::vector<Entity*>						_entities;
	std::map<EntityType, std::vector<Entity*>>	_destroyableEntities;
};

