#pragma once
#include "Camera2D.h"
#include "SDL_render.h"
#include "System.h"


class RenderSystem : public System
{
public:
						RenderSystem(float updateRate = 0.f);
						~RenderSystem();

	void				Initialize(SDL_Renderer*& renderer, Camera2D::Camera* camera);

	void				Process(float dt = 0.f) override;

	virtual void		AddEntity(Entity* gameObject) override;
	virtual void		RemoveEntity(EntityType tag, Entity* gameObject) override;

private:
	Camera2D::Camera*	_camera;
	SDL_Renderer*		_renderer;
	std::vector<Entity*>_minimapThings;
};

