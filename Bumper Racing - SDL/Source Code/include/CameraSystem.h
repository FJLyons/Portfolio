#pragma once
#include "Camera2D.h"
#include "SDL_Rect.h"
#include "System.h"


class CameraSystem : public System
{
public:
						CameraSystem(float updateRate = 0.f);
						~CameraSystem();

	void				Process(float dt = 0.f) override;
	Camera2D::Camera&	GetCamera() { return _camera; }
	void				Initialize(int w, int h);

private:
	Camera2D::Camera	_camera;
};

