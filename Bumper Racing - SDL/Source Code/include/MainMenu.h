#pragma once

#include "Scene.h"
#include "UISystem.h"

class MainMenu : public Scene
{
public:
									MainMenu();
									~MainMenu();

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

private:
	UISystem						_uiSystem;

	CurrentScene					_swapScene;

	int								GetPressedItem();
	int								_selectedItemIndex;
};

