
#include "EntityFactory.h"

#include "ColliderComponent.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "GunComponent.h"
#include "AIComponent.h"
#include "DestructionComponent.h"
#include "FlagComponent.h"
#include "CheckpointComponent.h"
#include "RemoteComponent.h"
#include "StatusEffectComponent.h"
#include "PowerUpComponent.h"
#include "AnimationComponent.h"
#include "WeaponComponent.h"
#include "NetworkHandler.h"

#include "ConstHolder.h"


EntityFactory::EntityFactory()	
{
}

EntityFactory::~EntityFactory()
{

}


void EntityFactory::Initialize(std::map<TextureID, SDL_Texture*>* th)
{
	_textureHolder = th;
}

Entity* EntityFactory::CreateEntity(EntityType t, int id)
{
	Entity* entity = nullptr;

	switch (t)
	{
	case EntityType::Tile:
		entity = CreateTileEntity(id);
		break;
	case EntityType::Checkpoint:
		entity = CreateCheckpointEntity(id);
		break;
	case EntityType::Bullet:
		entity = CreateBulletEntity(id);
		break;
	case EntityType::PowerUp:
		entity = CreatePowerUpEntity(id);
		break;
	case EntityType::Player:
		entity = CreatePlayerEntity(id);
		break;
	case EntityType::RemotePlayer:
		entity = CreateRemotePlayerEntity(id);
		break;
	case EntityType::AI:
		entity = CreateAIEntity(id);
		break;
	case EntityType::Weapon:
		entity = CreateWeaponEntity(id);
		break;
	case EntityType::Flag:
		entity = CreateFlagEntity(id);
		break;
	case EntityType::UI:
		entity = CreateUIEntity(id);
		break;
	case EntityType::Radar:
		entity = CreateRadarEntity(id);
		break;

	default:
		break;
	}

	return entity;
}

Entity* EntityFactory::CreatePlayerEntity(int id)
{
	Entity* player = new Entity(EntityType::Player);

	SpriteComponent* spriteComponent= new SpriteComponent((*_textureHolder)[TextureID::Player], id);
	spriteComponent->sourceRect.w /= 2;
	spriteComponent->sourceRect.h /= 7;

	player->AddComponent(spriteComponent);
	player->AddComponent(new TransformComponent(0, 0, 275 * 0.8f, 175 * 0.8f));
	player->AddComponent(new PhysicsComponent(0, 0, PLAYER_ACCEL_RATE, PLAYER_ACCEL_RATE, MAX_PLAYER_VELOCITY));
	player->AddComponent(new ColliderComponent());
	player->AddComponent(new FlagComponent());
	if (id != -1)//is in multiplayer game
		player->AddComponent(new RemoteComponent(id));
	player->AddComponent(new StatusEffectComponent());
	player->AddComponent(new AnimationComponent());
	player->AddComponent(new WeaponComponent());

	return player;
}

Entity* EntityFactory::CreateRemotePlayerEntity(int id)
{
	Entity* remotePlayer = new Entity(EntityType::RemotePlayer);
	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Player], id);
	spriteComponent->sourceRect.w /= 2;
	spriteComponent->sourceRect.h /= 7;

	remotePlayer->AddComponent(spriteComponent);
	remotePlayer->AddComponent(new TransformComponent(0, 0, 275 * 0.8f, 175 * 0.8f));
	remotePlayer->AddComponent(new PhysicsComponent(0, 0, PLAYER_ACCEL_RATE, PLAYER_ACCEL_RATE, MAX_PLAYER_VELOCITY));
	remotePlayer->AddComponent(new ColliderComponent());
	remotePlayer->AddComponent(new FlagComponent());
	remotePlayer->AddComponent(new RemoteComponent(id));
	remotePlayer->AddComponent(new StatusEffectComponent());
	remotePlayer->AddComponent(new AnimationComponent());
	remotePlayer->AddComponent(new WeaponComponent());
	remotePlayer->AddComponent(new DestructionComponent());

	return remotePlayer;
}

Entity* EntityFactory::CreateAIEntity(int id)
{
	Entity* ai = new Entity(EntityType::AI);
	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Player], id);
	spriteComponent->sourceRect.w /= 2;
	spriteComponent->sourceRect.h /= 7;

	ai->AddComponent(spriteComponent);

	ai->AddComponent(new TransformComponent(0, 0, 275 * 0.8f, 175 * 0.8f));
	ai->AddComponent(new PhysicsComponent(0, 0, PLAYER_ACCEL_RATE, PLAYER_ACCEL_RATE, MAX_PLAYER_VELOCITY));
	ai->AddComponent(new ColliderComponent());
	ai->AddComponent(new FlagComponent());
	if (id != -1)//is in multiplayer game
		ai->AddComponent(new RemoteComponent(id));
	ai->AddComponent(new AIComponent());
	ai->AddComponent(new StatusEffectComponent());
	ai->AddComponent(new AnimationComponent());
	ai->AddComponent(new WeaponComponent());

	return ai;
}
Entity* EntityFactory::CreatePowerUpEntity(int id)
{
	Entity* powerUp = new Entity(EntityType::PowerUp);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::PowerUp], id);
	spriteComponent->sourceRect = { id * 120, 0, 120, 120 };

	powerUp->AddComponent(spriteComponent);
	powerUp->AddComponent(new TransformComponent(0, 0, 64, 64));
	powerUp->AddComponent(new ColliderComponent());
	powerUp->AddComponent(new DestructionComponent());
	powerUp->AddComponent(new PowerUpComponent(static_cast<PowerUpComponent::Type>(id)));

	return powerUp;
}
Entity* EntityFactory::CreateWeaponEntity(int id)
{
	Entity* weapon = new Entity(EntityType::Weapon);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Weapon], id);
	spriteComponent->sourceRect = { id * 275, 0, 275, 175 };

	weapon->AddComponent(new TransformComponent(0.f, 0.f, 275 * 0.8f, 175 * 0.8f));
	weapon->AddComponent(spriteComponent);
	weapon->AddComponent(new GunComponent(FIRE_RATE[id], AMMO[id], id));
	if (id != -1)//is in multiplayer game
		weapon->AddComponent(new RemoteComponent(id));
	weapon->AddComponent(new DestructionComponent());

	return weapon;
}
Entity* EntityFactory::CreateBulletEntity(int id)
{
	Entity* bullet = new Entity(EntityType::Bullet);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Bullet], id);
	spriteComponent->sourceRect = { id * 16, 0, 16, 16 };

	bullet->AddComponent(new TransformComponent(0.f, 0.f, 16, 16));
	bullet->AddComponent(spriteComponent);
	bullet->AddComponent(new PhysicsComponent(0, 0, 0, 0, MAX_BULLET_VELOCITY));
	bullet->AddComponent(new ColliderComponent());
	bullet->AddComponent(new DestructionComponent());

	return bullet;
}
Entity* EntityFactory::CreateCheckpointEntity(int id)
{
	Entity* checkpoint = new Entity(EntityType::Checkpoint);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Checkpoint], id);
	spriteComponent->sourceRect.w *= 0.5f;

	checkpoint->AddComponent(spriteComponent);
	checkpoint->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	checkpoint->AddComponent(new ColliderComponent());
	checkpoint->AddComponent(new CheckpointComponent(id));

	return checkpoint;
}
Entity* EntityFactory::CreateFlagEntity(int id)
{
	Entity* flag = new Entity(EntityType::Flag);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Flag], id);

	flag->AddComponent(spriteComponent);
	flag->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	flag->AddComponent(new ColliderComponent());
	if (id != -1)//is in multiplayer game
	{
		flag->AddComponent(new RemoteComponent(id));
	}
	flag->AddComponent(new PhysicsComponent(0, 0, 0, 0, MAX_FLAG_VELOCITY));
	return flag;
}
Entity* EntityFactory::CreateTileEntity(int id)
{
	Entity* tile = new Entity(EntityType::Tile);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::TilemapSpriteSheet], id);

	switch (id)
	{//FIX THISSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
	case 1:
	{
		spriteComponent->sourceRect = { 0, 0, 0, 0 };
		break;
	}
	case 2:
	{
		spriteComponent->sourceRect = { 1, 0, 0, 0 };
		break;
	}
	case 3:
	{
		spriteComponent->sourceRect = { 2, 0, 0, 0 };
		break;
	}
	case 4:
	{
		spriteComponent->sourceRect = { 3, 0, 0, 0 };
		break;
	}
	case 5:
	{
		spriteComponent->sourceRect = { 4, 0, 0, 0 };
		break;
	}
	case 6:
	{
		spriteComponent->sourceRect = { 0, 1, 0, 0 };
		break;
	}
	case 7:
	{
		spriteComponent->sourceRect = { 1, 1, 0, 0 };
		break;
	}
	case 8:
	{
		spriteComponent->sourceRect = { 2, 1, 0, 0 };
		break;
	}
	case 9:
	{
		spriteComponent->sourceRect = { 3, 1, 0, 0 };
		break;
	}
	case 10:
	{
		spriteComponent->sourceRect = { 4, 1, 0, 0 };
		break;
	}
	case 11:
	{
		spriteComponent->sourceRect = { 0, 2, 0, 0 };
		break;
	}
	case 12:
	{
		spriteComponent->sourceRect = { 1, 2, 0, 0 };
		break;
	}
	case 13:
	{
		spriteComponent->sourceRect = { 2, 2, 0, 0 };
		break;
	}
	case 14:
	{
		spriteComponent->sourceRect = { 3, 2, 0, 0 };
		break;
	}
	case 15:
	{
		spriteComponent->sourceRect = { 4, 2, 0, 0 };
		break;
	}
	default:
		break;
	}

	tile->AddComponent(spriteComponent);
	tile->AddComponent(new TransformComponent(0, 0, 0, 0));

	return tile;
}
Entity* EntityFactory::CreateUIEntity(int id)
{
	Entity* ui = new Entity(EntityType::UI);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::UI], id);

	switch (id)
	{
	case 1:
	{
		spriteComponent->sourceRect = { 0, 0, 0, 0 };
		break;
	}
	case 2:
	{
		spriteComponent->sourceRect = { 1, 0, 0, 0 };
		break;
	}
	case 3:
	{
		spriteComponent->sourceRect = { 2, 0, 0, 0 };
		break;
	}
	case 4:
	{
		spriteComponent->sourceRect = { 3, 0, 0, 0 };
		break;
	}
	case 5:
	{
		spriteComponent->sourceRect = { 4, 0, 0, 0 }; // radar
		break;
	}

	default:
		break;
	}


	ui->AddComponent(spriteComponent);
	ui->AddComponent(new TransformComponent(0, 0, 0, 0));

	return ui;
}
Entity* EntityFactory::CreateRadarEntity(int id)
{
	Entity* radar = new Entity(EntityType::Radar);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Radar], id);

	radar->AddComponent(spriteComponent);
	radar->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	if (id != -1)//is in multiplayer game
		radar->AddComponent(new RemoteComponent(id));

	return radar;
}