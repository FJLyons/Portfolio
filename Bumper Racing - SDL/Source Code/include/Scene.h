#pragma once

// C++
#include <vector>
#include <queue>
#include <map>

// SDL
#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "FLInputManager.h"
#include "ResourceIdentifier.h"
#include "AudioManager.h"
#include "LTimer.h"

class Scene : public EventListener
{
protected:
	enum class CurrentScene 
	{
		MAIN_MENU,
		GAME,
		LOBBY,
		OPTIONS,
		ABOUT,
		GAMEOVER
	};

public:
	Scene();
	~Scene();

	virtual void						Initialize(SDL_Renderer* renderer);

	virtual int							Update();
	virtual void						Render();

	virtual bool						IsRunning();

	virtual void						Start();
	virtual void						Stop();

protected:
	virtual void						BindInput();

	virtual void						LoadContent();
	virtual void						CleanUp();

	SDL_Texture*						LoadTexture(const std::string & path);

protected:
	InputManager*						_inputManager = InputManager::GetInstance();
	AudioManager*						_audioManager = AudioManager::GetInstance();

	SDL_Renderer*						_renderer;

	std::map<TextureID, SDL_Texture*>	_textureHolder;

	bool								_running;
	unsigned int						_lastTime; //time of last update;

	CurrentScene						_swapScene;

public:
	bool								_win = false;
};