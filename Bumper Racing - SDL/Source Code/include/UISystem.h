#pragma once

#include "System.h"

#include "SDL_image.h"
#include "SDL_ttf.h"

#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <map>

#include "Camera2D.h"
#include "ResourceIdentifier.h"

class UISystem : public System
{
public:
	UISystem(float dt);
	~UISystem();

	void							Initialize(SDL_Renderer * renderer);
	void							PostInitialize(std::vector<Entity*> player, std::vector<Entity*> checkpoints, Entity* flag);

	void							Process(float dt = 0.f) override;
	void							HUD(float dt = 0.f);

	SDL_Texture*					LoadTexture(const std::string & path);

	void							DeleteEntites();

	void							DeleteText();
	void							DeleteDisplayText();

	void							CreateText(std::string message, int x, int y);
	void							CreateTextAtOrigin(std::string message, int x, int y);
	void							CreateTextAtCenter(std::string message, int x, int y);
	void							CreateTextColoured(std::string message, int x, int y, Uint8 r, Uint8 b, Uint8 g, Uint8 a);

	void							UpdateText(std::string message, int index);
	void							UpdateTextAtCenter(std::string message, int index);
	void							UpdateTextColoured(std::string message, int index, Uint8 r, Uint8 b, Uint8 g, Uint8 a);

	void							CreateDisplayText(std::string message, int x, int y);
	void							CreateDisplayTextColoured(std::string message, int x, int y, Uint8 r, Uint8 b, Uint8 g, Uint8 a);
	void							DeleteDisplayTextByID(int id);
	void							UpdateDisplayText(std::string message, int index);
	void							UpdateDisplayTextColoured(std::string message, int index, Uint8 r, Uint8 b, Uint8 g, Uint8 a);

	void							CreateBackButton(std::string message, int x, int y);
	void							CreateLobbyButton(std::string message, int x, int y);
	void							CreateReadyButton(std::string message, int x, int y);

	std::vector<SDL_Rect>&			GetInteractiveTextRectangle();
	std::vector<SDL_Rect>&			GetDisplayTextRectangle();

	SDL_Rect&						GetBackButton();
	SDL_Rect&						GetLobbyButton();
	SDL_Rect&						GetReadyButton();

	void							DeleteBackButton();
	void							DeleteLobbyButton();
	void							DeleteReadyButton();

	std::vector<Entity*>			GetCharacters();
	std::vector<Entity*>			GetCheckpoints();
	Entity*							GetFlag();

private:
	SDL_Renderer*					_renderer;

	TTF_Font*						_font = NULL;
	int								_fontSize;

	std::vector<SDL_Rect>			_displayTextRectangle;
	std::vector<SDL_Rect>			_interactiveTextRectangle;
	SDL_Rect						_backButton;
	SDL_Rect						_readyButton;
	SDL_Rect						_lobbyButton;

	std::vector<SDL_Texture*>		_displayTextTexture;
	std::vector<SDL_Texture*>		_interactiveTextTexture;
	SDL_Texture*					_backButtonTexture;
	SDL_Texture*					_readyButtonTexture;
	SDL_Texture*					_lobbyButtonTexture;

	std::map<TextureID, SDL_Texture*>_textureHolder;

	std::vector<Entity*>			_characters;
	std::vector<Entity*>			_checkpoints;
	Entity*							_flag;

public:
	int								currentLapLocal;
	int								currentAmmoLocal;
	int								isTop;

	// Poll
	int								topLap[3];
	int								topCheckpoint[3];
	int								nextCheckpoint[3];
	int								topPlayers[3];

	bool							gameOver;
	bool							win;
};

// process - entities
// player
// weapon
// player to weapon - weapon gets ammunition / belongs to player
// weapon - gun - ammunition
// access flag component - current checkpoint / laps