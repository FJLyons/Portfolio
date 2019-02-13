#include "SystemManager.h"

#include "ConstHolder.h"


SystemManager::SystemManager()
	: _creationRequests(std::vector<std::pair<EntityType, std::vector<float>>>())
{
}

SystemManager::~SystemManager()
{
	delete _creationSystem;

	GetUISystem()->DeleteDisplayText();
	GetUISystem()->DeleteText();

	SystemMapIterator it = _systems.begin();
	for (; it != _systems.end(); ++it)
	{
		delete it->second;
	}

	for (InteractionSystemMapIterator it = _interactionSystems.begin(); it != _interactionSystems.end(); ++it)
	{
		delete it->second;
	}
}



void SystemManager::Initialize(SDL_Renderer*& renderer, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, int width, int height)
{
	InitializeSystems(renderer, entityFactory, bodyFactory, world, width, height);
	InitializeInteractionSystems();
}


#pragma region Initialization

void SystemManager::InitializeSystems(SDL_Renderer*& renderer, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, int width, int height)
{
	//SETUP CAMERA SYSTEM
	CameraSystem* cameraSystem = new CameraSystem(CAMERA_SYSTEM_UPDATE);
	cameraSystem->Initialize(width, height);
	_systems[SystemType::Camera] = cameraSystem;

	//SETUP RENDER SYSTEM
	RenderSystem* rendererSystem = new RenderSystem(0);
	rendererSystem->Initialize(renderer, &cameraSystem->GetCamera());
	_systems[SystemType::Render] = rendererSystem;

	//SETUP PHYSICS SYSTEM
	PhysicsSystem* physicsSystem = new PhysicsSystem(PHYSICS_SYSTEM_UPDATE);
	_systems[SystemType::Physics] = physicsSystem;

	//SETUP COLLISION SYSTEM
	CollisionSystem* collisionSystem = new CollisionSystem(_interactionSystemEvents, _creationRequests, COLLISION_SYSTEM_UPDATE);
	world->SetContactListener(collisionSystem);
	_systems[SystemType::Collision] = collisionSystem;

	//SETUP UI SYSTEM
	UISystem* uiSystem = new UISystem(0);
	uiSystem->Initialize(renderer);
	_systems[SystemType::UI] = uiSystem;

	//SETUP GUN SYSTEM
	GunSystem* gunSystem = new GunSystem(_creationRequests, 0);
	_systems[SystemType::Gun] = gunSystem;

	//SETUP STATUS EFFECT SYSTEM
	StatusEffectSystem* statusEffectSystem = new StatusEffectSystem(0);
	_systems[SystemType::StatusEffect] = statusEffectSystem;

	//SETUP AI SYSTEM
	AISystem* aiSystem = new AISystem(0);
	_systems[SystemType::AI] = aiSystem;

	//SETUP Waypoint SYSTEM
	WaypointSystem* waypointSystem = new WaypointSystem(_creationRequests, _interactionSystemEvents, 0);
	_systems[SystemType::Waypoint] = waypointSystem;


	//SETUP DESTROY SYSTEM
	DestructionSystem* destructionSystem = new DestructionSystem(0);
	destructionSystem->Initialize(world);
	_systems[SystemType::Destruction] = destructionSystem;

	//SETUP CREATION SYSTEM
	_creationSystem = new CreationSystem(_creationRequests);
	_creationSystem->Initialize(entityFactory, bodyFactory);

	//SETUP Animation SYSTEM
	AnimationSystem* animationSystem = new AnimationSystem();
	_systems[SystemType::Animation] = animationSystem;

	//SETUP remote SYSTEM
	RemoteSystem*_remoteSystem = new RemoteSystem(REMOTE_PACKET_RATE, _creationRequests, _interactionSystemEvents);
	_systems[SystemType::Remote] = _remoteSystem;
}

void SystemManager::InitializeInteractionSystems()
{
	//SETUP WEAPON INTERACTION SYSTEM
	WeaponSystem* weaponSystem = new WeaponSystem(_interactionSystemEvents, 0);
	_interactionSystems[InteractionSystemType::Weapon] = weaponSystem;

	FlagCheckpointSystem* flagSystem = new FlagCheckpointSystem(_interactionSystemEvents, 0);
	_interactionSystems[InteractionSystemType::Flag] = flagSystem;
}

#pragma endregion

void SystemManager::PostInitialize(Entity*& player, Graph* waypoints)
{
	_creationSystem->Process(0);

	Entity* flag = nullptr;
	Entity* radar = nullptr;

	std::vector<Entity*> checkpoints = std::vector<Entity*>();
	std::vector<Entity*> characters = std::vector<Entity*>();

	while (!_creationSystem->Empty())
	{
		std::pair<std::vector<SystemType>, Entity*>& systemCreatedEntity = _creationSystem->GetSystemCreatedEntity();

		for (int i = 0; i < systemCreatedEntity.first.size(); i++)
		{
			AddEntity(systemCreatedEntity.first.at(i), systemCreatedEntity.second);
		}

		if (systemCreatedEntity.second->GetType() == EntityType::Player)
		{
			player = systemCreatedEntity.second;
		}
		else if (systemCreatedEntity.second->GetType() == EntityType::Flag)
		{
			flag = systemCreatedEntity.second;
		}
		else if (systemCreatedEntity.second->GetType() == EntityType::Checkpoint)
		{
			checkpoints.push_back(systemCreatedEntity.second);
		}
		else if (systemCreatedEntity.second->GetType() == EntityType::Radar)
		{
			radar = systemCreatedEntity.second;
		}
		else if (systemCreatedEntity.second->GetType() == EntityType::AI || systemCreatedEntity.second->GetType() == EntityType::RemotePlayer)
		{
			characters.push_back(systemCreatedEntity.second);
		}

		_creationSystem->EntityToSystemAssigned();
	}

	_radarSystem.Initialize(player, radar, flag, characters, checkpoints);

	characters.push_back(player);


	//SETUP FLAG INTERACTION SYSTEM
	GetFlagCheckpointSystem()->Initialize(checkpoints);
	//SETUP WAYPOINT AND AI SYSTEM
	GetWaypointSystem()->Initialize(waypoints);
	GetAISystem()->Initialize(waypoints, player);
	GetRemoteSystem()->Initialize(waypoints, this);
	GetUISystem()->PostInitialize(characters, checkpoints, flag);
}

void SystemManager::Process(float dt)
{
	//Skip RenderSystem and UI
	SystemMapIterator it = _systems.begin();
	it++;
	it++;

	TryToDestroy(it, dt);

	TryToCreateEntities(dt);

	ProcessAllSystems(it, dt);

	ProcessAllInteractionSystems(it, dt);

	_radarSystem.Process(dt);
}

void SystemManager::TryToCreateEntities(float dt)
{
	_creationSystem->Process(dt);

	while (!_creationSystem->Empty())
	{
		std::pair<std::vector<SystemType>, Entity*>& systemCreatedEntity = _creationSystem->GetSystemCreatedEntity();

		for (int i = 0; i < systemCreatedEntity.first.size(); i++)
		{
			AddEntity(systemCreatedEntity.first.at(i), systemCreatedEntity.second);
		}

		if (systemCreatedEntity.second->GetType() == EntityType::Weapon)
		{
			for (int i = 0; i < _interactionSystemEvents[InteractionSystemEvent::WeaponCreated].size(); i++)
			{
				if (_interactionSystemEvents[InteractionSystemEvent::WeaponCreated].at(i).second == nullptr)
				{
					_interactionSystemEvents[InteractionSystemEvent::WeaponCreated].at(i).second = systemCreatedEntity.second;
					break;
				}
			}
			
		}

		_creationSystem->EntityToSystemAssigned();
	}
}

void SystemManager::TryToDestroy(SystemMapIterator& it, float dt)
{
	DestructionSystem* destructionSystem = static_cast<DestructionSystem*>(it->second);
	destructionSystem->Process(dt);

	int index = destructionSystem->GetEntitiesToBeDestroyed().size() - 1;
	while (index >= 0)
	{
		DestroyBasedOnType(destructionSystem->GetEntitiesToBeDestroyed().at(index));
		destructionSystem->DestroyEntity();

		index--;
	}
}

void SystemManager::ProcessAllSystems(SystemMapIterator& it, float dt)
{
	for (; it != _systems.end(); ++it)
	{
		it->second->Process(dt);
	}
}
void SystemManager::ProcessAllInteractionSystems(SystemMapIterator& it, float dt)
{
	for (InteractionSystemMapIterator it = _interactionSystems.begin(); it != _interactionSystems.end(); ++it)
	{
		it->second->Process(dt);
	}
}

void SystemManager::DestroyBasedOnType(Entity*& entity)
{
	switch (entity->GetType())
	{
	case EntityType::Flag:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::Collision]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::Physics]->RemoveEntity(entity->GetType(), entity);
		//Implement Later
		break;
	case EntityType::Checkpoint:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		//Implement Later
		break;
	case EntityType::Tile:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		//Implement Later
		break;
	case EntityType::Bullet:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::Physics]->RemoveEntity(entity->GetType(), entity);
		_creationSystem->RemoveEntitiy(entity->GetType(), entity);
		break;
	case EntityType::PowerUp:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		_creationSystem->RemoveEntitiy(entity->GetType(), entity);
		break;
	case EntityType::AI:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::Physics]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::AI]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::StatusEffect]->RemoveEntity(entity->GetType(), entity);
		break;
	case EntityType::Player:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::Physics]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::Camera]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::Waypoint]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::StatusEffect]->RemoveEntity(entity->GetType(), entity);
		break;
	case EntityType::Weapon:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::Gun]->RemoveEntity(entity->GetType(), entity);
		_creationSystem->RemoveEntitiy(entity->GetType(), entity);
		//fix weapon deletion
		static_cast<WeaponSystem*>(_interactionSystems[InteractionSystemType::Weapon])->RemoveEntity(entity, false);
		break;

	default:
		break;
	}
}

void SystemManager::Render(float dt)
{
	_systems[SystemType::Render]->Process(dt);
	_systems[SystemType::UI]->Process(dt);
}

void SystemManager::AddRequest(std::pair<EntityType, std::vector<float>>& creationRequest)
{
	_creationRequests.push_back(creationRequest);
}
void SystemManager::AddRequest(std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests)
{
	for (int i = 0; i < creationRequests.size(); i++)
	{
		_creationRequests.push_back(creationRequests.at(i));
	}
}

void SystemManager::AddEntity(SystemType type, Entity* entity)
{
	_systems[type]->AddEntity(entity);
}
void SystemManager::AddEntity(InteractionSystemType type, Entity* actor, Entity* item)
{
	_interactionSystems[type]->AddEntity(actor, item);
}

void SystemManager::RemoveEntity(SystemType type, Entity* entity)
{
	_systems[type]->RemoveEntity(entity->GetType(), entity);
}
void SystemManager::RemoveEntity(InteractionSystemType type, Entity* actor, Entity* item)
{
	_interactionSystems[type]->RemoveEntity(actor, item);
}
void SystemManager::RemoveEntity(InteractionSystemType type, Entity* entity, bool firstItem)
{
	_interactionSystems[type]->RemoveEntity(entity, firstItem);
}

#pragma region Get Systems

RenderSystem* SystemManager::GetRenderSystem()
{
	return static_cast<RenderSystem*>(_systems[SystemType::Render]);
}
PhysicsSystem* SystemManager::GetPhysicsSystem()
{
	return static_cast<PhysicsSystem*>(_systems[SystemType::Physics]);
}
CameraSystem* SystemManager::GetCameraSystem()
{
	return static_cast<CameraSystem*>(_systems[SystemType::Camera]);
}
CollisionSystem* SystemManager::GetCollisionSystem()
{
	return static_cast<CollisionSystem*>(_systems[SystemType::Collision]);
}
GunSystem* SystemManager::GetGunSystem()
{
	return static_cast<GunSystem*>(_systems[SystemType::Gun]);
}
AISystem* SystemManager::GetAISystem()
{
	return static_cast<AISystem*>(_systems[SystemType::AI]);
}

RemoteSystem * SystemManager::GetRemoteSystem()
{
	return static_cast<RemoteSystem*>(_systems[SystemType::Remote]);
}

StatusEffectSystem* SystemManager::GetStatusEffectSystem()
{
	return static_cast<StatusEffectSystem*>(_systems[SystemType::StatusEffect]);
}
WaypointSystem* SystemManager::GetWaypointSystem()
{
	WaypointSystem* waypointSystem = static_cast<WaypointSystem*>(_systems[SystemType::Waypoint]);
	return waypointSystem;
}


AnimationSystem * SystemManager::GetAnimationSystem()
{
	return static_cast<AnimationSystem*>(_systems[SystemType::Animation]);
}

UISystem * SystemManager::GetUISystem()
{
	return static_cast<UISystem*>(_systems[SystemType::UI]);
}
#pragma endregion

#pragma region Get Interaction Systems

WeaponSystem* SystemManager::GetWeaponInteractionSystem()
{
	return static_cast<WeaponSystem*>(_interactionSystems[InteractionSystemType::Weapon]);
}

FlagCheckpointSystem* SystemManager::GetFlagCheckpointSystem()
{
	return static_cast<FlagCheckpointSystem*>(_interactionSystems[InteractionSystemType::Flag]);
}

#pragma endregion

Camera2D::Camera& SystemManager::GetCamera()
{
	return static_cast<CameraSystem*>(_systems[SystemType::Camera])->GetCamera();
}