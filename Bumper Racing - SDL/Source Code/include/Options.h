#pragma once

#include "Scene.h"
#include "UISystem.h"

class Options : public Scene
{
public:
	Options();
	~Options();

	void							Initialize(SDL_Renderer* renderer) override;

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

	void							MoveUp();
	void							MoveDown();

	int								GetPressedItem();

	std::string						GetMusicVolume();
	std::string						GetHumVolume();
	std::string						GetWeaponVolume();
	std::string						GetCheckpointVolume();
	std::string						GetCollisionVolume();
	std::string						GetUIVolume();

private:
	UISystem						_uiSystem;

	int								_selectedItemIndex;
};

