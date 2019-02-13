#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <map>

#include "Entity.h"
#include "SDL_image.h"
#include "ResourceIdentifier.h"

class EntityFactory
{
public:
	EntityFactory();
	~EntityFactory();

	void								Initialize(std::map<TextureID, SDL_Texture*>* th);

	Entity*								CreateEntity(EntityType t, int id);

private:
	Entity*								CreateWeaponEntity(int id);
	Entity*								CreatePlayerEntity(int id);
	Entity*								CreateRemotePlayerEntity(int id);
	Entity*								CreateAIEntity(int id);
	Entity*								CreatePowerUpEntity(int id);
	Entity*								CreateBulletEntity(int id);
	Entity*								CreateCheckpointEntity(int id);
	Entity*								CreateFlagEntity(int id);
	Entity*								CreateRadarEntity(int id);
	Entity*								CreateTileEntity(int id);
	Entity*								CreateUIEntity(int id);

private:
	std::map<TextureID, SDL_Texture*>*	_textureHolder;
};

#endif