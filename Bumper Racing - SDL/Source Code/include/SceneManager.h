#pragma once

#include <SDL.h>

#include "Camera2D.h"

#include "Scene.h"
#include "MainMenu.h"
#include "Game.h"
#include "Lobby.h"
#include "Options.h"
#include "About.h"
#include "GameOver.h"


class SceneManager
{
public:
									SceneManager();
									~SceneManager();

	bool							Initialize(const char* title, int xpos, int ypos, int width, int height, int flags);
	void							Update();

	bool							IsRunning();

private:
	bool							SetupSDL(const char* title, int xpos, int ypos, int width, int height, int flags);

private:
	std::vector<Scene*>				_currentScene;

	SDL_Window*						_window;
	SDL_Renderer*					_renderer;

	std::vector<int>				_ids;

	int								_runningScene;
	int								_previousScene;

	bool							_running;

	bool							_win = false;

	int								_level;
};