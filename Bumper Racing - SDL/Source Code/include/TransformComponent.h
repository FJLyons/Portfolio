#pragma once

#include "Component.h"
#include "SDL_rect.h"


class TransformComponent : public Component
{
public:
	TransformComponent(float xPos, float yPos, int w, int h)
		: Component::Component(Component::Type::Transform)
		, rect({ (int)xPos, (int)yPos, (int)w, (int)h })
		, origin({ (int)(w*0.5f), (int)(h*0.5f) })
		, angle(0)
	{
	}

	TransformComponent(float xPos, float yPos, int w, float h, float a)
		: Component::Component(Component::Type::Transform)
		, rect({(int)xPos, (int)yPos, (int)w, (int)h})
		, origin({ (int)(w*0.5f), (int)(h*0.5f)})
		, angle(a)
	{
	}

	TransformComponent(float xPos, float yPos, int w, int h, int oX, int oY, float a)
		: Component::Component(Component::Type::Transform)
		, rect({ (int)xPos, (int)yPos, (int)w, (int)h })
		, origin({ oX, oY })
		, angle(a)
	{
	}
	
	TransformComponent(SDL_Rect r)
		: Component::Component(Component::Type::Transform)
		, rect(r)
		, origin({ (int)(r.w*0.5f), (int)(r.h*0.5f) })
		, angle(0)
	{
	}

	TransformComponent(SDL_Rect r, float a)
		: Component::Component(Component::Type::Transform)
		, rect(r)
		, origin({ (int)(r.w*0.5f), (int)(r.h*0.5f) })
		, angle(a)
	{
	}

	TransformComponent(SDL_Rect r, SDL_Point o, float a)
		: Component::Component(Component::Type::Transform)
		, rect(r)
		, origin(o)
		, angle(a)
	{
	}

	~TransformComponent()
	{
	}

public:
	SDL_Rect	rect;
	SDL_Point	origin;

	float		angle;
};
