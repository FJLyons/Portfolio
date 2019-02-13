#pragma once
#pragma once

#include "Scene.h"
#include "UISystem.h"

class GameOver : public Scene
{
public:
	GameOver();
	~GameOver();

	void							Initialize(SDL_Renderer* renderer, bool win);

	int								Update() override;
	void							Render() override;

	bool							IsRunning() override;

	void							Start() override;
	void							Stop() override;

	void							OnEvent(Event evt, Type typ) override;

private:
	void							BindInput() override;

	void							LoadContent() override;
	void							CleanUp() override;

private:
	UISystem						_uiSystem;
};
