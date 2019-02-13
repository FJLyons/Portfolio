#pragma once

#include "Component.h"

#include "SDL_image.h"


class SpriteComponent : public Component
{
public:
	SpriteComponent(SDL_Texture* texture, int id)
		: texture(texture)
		, id(id)
		, Component::Component(Component::Type::Sprite)
	{
		sourceRect = { 0, 0, 0, 0 };
		SDL_QueryTexture(texture, NULL, NULL, &sourceRect.w, &sourceRect.h);
	}

	SpriteComponent(SDL_Texture* texture , SDL_Rect source, int id)
		: texture(texture)
		, id(id)
		, sourceRect(source)
		, Component::Component(Component::Type::Sprite)
	{
	}

	~SpriteComponent()
	{
	}

public:
	SDL_Texture* texture;
	SDL_Rect sourceRect;
	int id;
};
