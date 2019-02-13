#pragma once

#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "CameraSystem.h"
#include "CollisionSystem.h"
#include "GunSystem.h"
#include "AISystem.h"
#include "DestructionSystem.h"
#include "UISystem.h"
#include "WeaponSystem.h"
#include "FlagCheckpointSystem.h"
#include "WaypointSystem.h"
#include "CreationSystem.h"
#include "RemoteSystem.h"
#include "StatusEffectSystem.h"
#include "AnimationSystem.h"
#include "RadarSystem.h"

#include "InteractionSystemEvents.h"
#include "SystemTypes.h"


class SystemManager
{
public:
	typedef std::map<InteractionSystemType, InteractionSystem*>::iterator InteractionSystemMapIterator;
	typedef std::map<SystemType, System*>::iterator SystemMapIterator;

	SystemManager();
	~SystemManager();


	void								Initialize(SDL_Renderer*& renderer, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, int width, int height);
	void								InitializeSystems(SDL_Renderer*& renderer, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, int width, int height);
	void								InitializeInteractionSystems();
	void								PostInitialize(Entity*& player, Graph* waypoints);

	void								Process(float dt = 0.f);
	void								Render(float dt = 0.f);

	void								AddRequest(std::pair<EntityType, std::vector<float>>& creationRequest);
	void								AddRequest(std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests);

	void								AddEntity(SystemType type, Entity* entity);
	void								AddEntity(InteractionSystemType type, Entity* actor, Entity* item);

	void								RemoveEntity(SystemType type, Entity* entity);
	void								RemoveEntity(InteractionSystemType type, Entity* actor, Entity* item);
	void								RemoveEntity(InteractionSystemType type, Entity* actor, bool firstItem);

	RenderSystem*						GetRenderSystem();
	PhysicsSystem*						GetPhysicsSystem();
	CameraSystem*						GetCameraSystem();
	CollisionSystem*					GetCollisionSystem();
	GunSystem*							GetGunSystem();
	UISystem*							GetUISystem();
	AISystem*							GetAISystem();
	RemoteSystem*						GetRemoteSystem();
	StatusEffectSystem*					GetStatusEffectSystem();
	WaypointSystem*						GetWaypointSystem();
	AnimationSystem*					GetAnimationSystem();
	WeaponSystem*						GetWeaponInteractionSystem();
	FlagCheckpointSystem*				GetFlagCheckpointSystem();

	Camera2D::Camera&					GetCamera();

private:

	void								TryToCreateEntities(float dt);
	void								TryToDestroy(SystemMapIterator& it, float dt);
	void								ProcessAllSystems(SystemMapIterator& it, float dt);
	void								ProcessAllInteractionSystems(SystemMapIterator& it, float dt);
	void								DestroyBasedOnType(Entity*& entity);

private:
	std::map<InteractionSystemType, 
		InteractionSystem*>				_interactionSystems;
	std::map<SystemType, System*>		_systems;
	std::map<InteractionSystemEvent,
		std::vector<
		std::pair<Entity*, Entity*>>>	_interactionSystemEvents;
	std::vector<
		std::pair<EntityType,
		std::vector<float>>>			_creationRequests;

	CreationSystem*						_creationSystem;
	RadarSystem							_radarSystem;
};
