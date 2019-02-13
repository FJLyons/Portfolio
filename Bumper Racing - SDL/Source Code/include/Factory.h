#pragma once
#include "Entity.h"
#include "Factory.h"
#include "SpriteComponent.h"
#include "BoundsComponent.h"
#include "SDL_image.h"

class Factory {
public:

	Entity* CreateTile(SDL_Texture* texture, SDL_Rect source, SDL_Rect dest)
	{
		Entity* tile = new Entity(Entity::Type::Tile);
		tile->AddComponent(new SpriteComponent(texture, source));
		tile->AddComponent(new BoundsComponent(dest, 1.0f, 1.0f, 0));
		return tile;
	}

	Entity* CreateFlag(SDL_Texture* texture, SDL_Rect source, SDL_Rect dest)
	{
		Entity* flag = new Entity(Entity::Type::Wall);
		flag->AddComponent(new SpriteComponent(texture, source));
		flag->AddComponent(new BoundsComponent(dest, 1.0f, 1.0f, 0));
		return flag;
	}
	Entity* CreateCheakpoint(SDL_Texture* texture, SDL_Rect source, SDL_Rect dest)
	{
		Entity* checkpoint = new Entity(Entity::Type::Wall);
		checkpoint->AddComponent(new SpriteComponent(texture, source));
		checkpoint->AddComponent(new BoundsComponent(dest, 1.0f, 1.0f, 0));

		return checkpoint;
	}
	Entity* CreateWall(SDL_Texture* texture, SDL_Rect source, SDL_Rect dest)
	{
		Entity* wall = new Entity(Entity::Type::Wall);
		wall->AddComponent(new SpriteComponent(texture, source));
		wall->AddComponent(new BoundsComponent(dest, 1.0f, 1.0f, 0));
		return wall;
	}

};