#include "SceneManager.h"

#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"
#include "NetworkHandler.h"

SceneManager::SceneManager() : _level(1)
{
}

SceneManager::~SceneManager()
{
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}

bool SceneManager::Initialize(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	_running = SetupSDL(title, xpos, ypos, width, height, flags);

	if (_running)
	{
		MainMenu* menu = new MainMenu();
		menu->Initialize(_renderer);
		_currentScene.push_back(menu);

		_runningScene = 0;
		_currentScene[_runningScene]->Start();
	}

	return _running;
}

bool SceneManager::SetupSDL(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if (_window != 0)
		{
			_renderer = SDL_CreateRenderer(_window, -1, 0);
			if (_renderer != 0)
			{
				SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
				if (SDLNet_Init() != 0)
				{

				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	else
	{
		return false;
	}

	return true;
}

void SceneManager::Update()
{
	if (_currentScene.back()->IsRunning())
	{
		_previousScene = _runningScene;
		_currentScene.back()->Render();
		_runningScene = _currentScene.back()->Update();
		//std::cout << NetworkHandler::Instance().gameTime << std::endl;
		// Stop Previous Scene
		if (_runningScene != _previousScene)
		{
			_currentScene.back()->Stop();
			if (_previousScene == 1) // Check game over
			{
				_win = _currentScene.back()->_win;
			}

			// Delete Object
			//Change numbers into enums, use switch instead of ifs
			delete _currentScene.back();
			_currentScene.pop_back();

			if (_runningScene == 0)
			{
				MainMenu* menu = new MainMenu();
				menu->Initialize(_renderer);
				_currentScene.push_back(menu);
			}

			else if (_runningScene == 1)
			{
				Game* game = new Game();
				game->Initialize(_renderer, _ids, _level);
				_currentScene.push_back(game);
			}

			else if (_runningScene == 2)
			{
				Lobby* lobby = new Lobby();
				lobby->Initialize(_renderer, &_ids);
				_currentScene.push_back(lobby);
			}

			else if (_runningScene == 3)
			{
				Options* options = new Options();
				options->Initialize(_renderer);
				_currentScene.push_back(options);
			}

			else if (_runningScene == 4)
			{
				About* about = new About();
				about->Initialize(_renderer);
				_currentScene.push_back(about);
			}

			else if (_runningScene == 5)
			{
				_level++;
				GameOver* gameOver = new GameOver();
				gameOver->Initialize(_renderer, _win);
				_currentScene.push_back(gameOver);
			}
		}
	}

	else
	{
		_running = false;
		if (NetworkHandler::Instance().GetConnected())
			NetworkHandler::Instance().Disconnect();
	}
}

bool SceneManager::IsRunning()
{
	return _running;
}