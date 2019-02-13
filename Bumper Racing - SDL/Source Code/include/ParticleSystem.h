#pragma once

#include "System.h"
#include "ParticleComponent.h"

class ParticleSystem : public System
{
public:
	ParticleSystem(float updateRate = 0.f);
	~ParticleSystem();

	void				Initialize();

	void				Process(float dt = 0.f) override;

	void				Render(SDL_Renderer*& renderer, Camera2D::Camera* camera);

};

