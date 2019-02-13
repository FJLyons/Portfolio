#include "UISystem.h"

#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "WeaponComponent.h"
#include "GunComponent.h"
#include "CheckpointComponent.h"
#include "FlagComponent.h"
#include <math.h>

UISystem::UISystem(float updateRate)
	: System(updateRate)
{
}

UISystem::~UISystem()
{
	TTF_CloseFont(_font);
	DeleteEntites();
	DeleteText();
	DeleteDisplayText();
	DeleteBackButton();
	DeleteLobbyButton();
	DeleteReadyButton();
	//TTF_CloseFont(_font); // Free Font Memory
}

void UISystem::Initialize(SDL_Renderer* renderer)
{
	_renderer = renderer;

	// Text
	TTF_Init();
	_fontSize = 32;
	_font = TTF_OpenFont("Media\\Fonts\\font.ttf", _fontSize);

	_interactiveTextTexture = std::vector<SDL_Texture*>();
	_interactiveTextRectangle = std::vector<SDL_Rect>();

	_displayTextTexture = std::vector<SDL_Texture*>();
	_displayTextRectangle = std::vector<SDL_Rect>();

}

void UISystem::PostInitialize(std::vector<Entity*> characters, std::vector<Entity*> checkpoints, Entity* flag)
{
	_characters = characters;
	_checkpoints = checkpoints;
	_flag = flag;

	for (int i = 0; i < 3; i++)
	{
		topPlayers[i] = i;
		topCheckpoint[i] = 0;
		topLap[i] = 1;
		nextCheckpoint[i] = 1;
	}

	gameOver = false;
	win = false;
}

void UISystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				// Transform to screen
				TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
				SpriteComponent* sprite = static_cast<SpriteComponent*>(e->GetComponent(Component::Type::Sprite));

				SDL_Rect drawRectangle = transform->rect;
				drawRectangle.x -= transform->origin.x;
				drawRectangle.y -= transform->origin.y;

				SDL_RenderCopyEx(_renderer, sprite->texture, &sprite->sourceRect, &drawRectangle, transform->angle, &transform->origin, SDL_FLIP_NONE);
			}
		}

		// Draw Text
		for (int i = 0; i < _interactiveTextTexture.size(); i++)
		{
			SDL_RenderCopy(_renderer, _interactiveTextTexture[i], NULL, &_interactiveTextRectangle[i]);
		}

		for (int i = 0; i < _displayTextTexture.size(); i++)
		{
			SDL_RenderCopy(_renderer, _displayTextTexture[i], NULL, &_displayTextRectangle[i]);
		}

		SDL_RenderCopy(_renderer, _backButtonTexture, NULL, &_backButton);
		SDL_RenderCopy(_renderer, _lobbyButtonTexture, NULL, &_lobbyButton);
		SDL_RenderCopy(_renderer, _readyButtonTexture, NULL, &_readyButton);
	}
}

void UISystem::HUD(float dt)
{
	CheckpointComponent* check = static_cast<CheckpointComponent*>(_checkpoints[0]->GetComponent(Component::Type::Checkpoint));

	//check->highlighted; //  is active checkpoint
	//check->id; // id of checkpoint

	for (int i = 0; i < _characters.size(); i++)
	{
		WeaponComponent* weapon = static_cast<WeaponComponent*>(_characters[i]->GetComponent(Component::Type::Weapon));
		FlagComponent* flag = static_cast<FlagComponent*>(_characters[i]->GetComponent(Component::Type::Flag));
		
		// Top 3 Players
		if (flag->totalCheckpoints > topCheckpoint[0])
		{
			topCheckpoint[0] = flag->totalCheckpoints;
			topPlayers[0] = i;
			topLap[0] = flag->currentLap;
			nextCheckpoint[0] = flag->currentCheckpointID + 1;
		}

		else if (flag->totalCheckpoints > topCheckpoint[1] && topPlayers[0] != i)
		{
			topCheckpoint[1] = flag->totalCheckpoints;
			topPlayers[1] = i;
			topLap[1] = flag->currentLap;
			nextCheckpoint[1] = flag->currentCheckpointID + 1;
		}

		else if (flag->totalCheckpoints > topCheckpoint[2] && topPlayers[0] != i && topPlayers[1] != i)
		{
			topCheckpoint[2] = flag->totalCheckpoints;
			topPlayers[2] = i;
			topLap[2] = flag->currentLap;
			nextCheckpoint[2] = flag->currentCheckpointID + 1;
		}

		//flag->currentLap; // current lap
		//flag->hasFlag; // entity has flag
		//flag->currentCheckpointID; // next checkpoint

		//weapon->ammo; // ammo count
		//weapon->fired; // has fired
		//weapon->hasWeapon; // has a weapon
		//weapon->id; // type of weapon

		if (i == _characters.size() - 1) // Back is local player
		{
			currentLapLocal = flag->currentLap;
			if (weapon->id != -1) { currentAmmoLocal = weapon->ammo; }
			else { currentAmmoLocal = 0; }

			if (flag->hasFlag == true) // Find Next Checkpoint
			{
				TransformComponent* checkTran = static_cast<TransformComponent*>(_checkpoints[flag->currentCheckpointID]->GetComponent(Component::Type::Transform));
				TransformComponent* playTran = static_cast<TransformComponent*>(_characters[i]->GetComponent(Component::Type::Transform));
			}

			if (topCheckpoint[2] == flag->totalCheckpoints) { isTop = 2; }
			else if (topCheckpoint[1] == flag->totalCheckpoints) { isTop = 1; }
			else if (topCheckpoint[0] == flag->totalCheckpoints) { isTop = 0; }
			else { isTop = 3; }
		}

		if (flag->currentLap == 4 && i == _characters.size() - 1)
		{
			gameOver = true;
			win = true;
		}
		else if (flag->currentLap == 4 && i != _characters.size() - 1)
		{
			gameOver = true;
			win = false;
		}
	}
}

void UISystem::DeleteEntites()
{
	for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
	{
		for (Entity* e : (*it).second)
		{
			delete e;
		}
	}
	_entities.clear();
}

// Textures
SDL_Texture* UISystem::LoadTexture(const std::string & path)
{
	SDL_Texture* texture = NULL;

	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(_renderer, surface);
		if (texture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(surface);
	}

	return texture;
}

// Delete Text
void UISystem::DeleteText()
{
	for (int i = 0; i < _interactiveTextTexture.size(); i++)
	{
		SDL_DestroyTexture(_interactiveTextTexture[i]);
	}
	_interactiveTextTexture.erase(_interactiveTextTexture.begin(), _interactiveTextTexture.end());
	_interactiveTextTexture.clear();

	_interactiveTextRectangle.erase(_interactiveTextRectangle.begin(), _interactiveTextRectangle.end());
	_interactiveTextRectangle.clear();
}

void UISystem::DeleteDisplayText()
{
	for (int i = 0; i < _displayTextTexture.size(); i++)
	{
		SDL_DestroyTexture(_displayTextTexture[i]);
	}
	_displayTextRectangle.erase(_displayTextRectangle.begin(), _displayTextRectangle.end());
	_displayTextRectangle.clear();

	_displayTextTexture.erase(_displayTextTexture.begin(), _displayTextTexture.end());
	_displayTextTexture.clear();
}

// Interactive Text
void UISystem::CreateText(std::string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture.back(), NULL, &_interactiveTextRectangle.back());
}

void UISystem::CreateTextAtOrigin(std::string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x - width / 2;
	textRectangle.y = y - height / 2;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture.back(), NULL, &_interactiveTextRectangle.back());
}

void UISystem::CreateTextAtCenter(std::string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x - width / 2;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture.back(), NULL, &_interactiveTextRectangle.back());
}

void UISystem::CreateTextColoured(std::string message, int x, int y, Uint8 r, Uint8 b, Uint8 g, Uint8 a)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ r, g, b, a });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture.back(), NULL, &_interactiveTextRectangle.back());
}

//Update Interactive Text
void UISystem::UpdateText(std::string message, int index)
{
	// Destroy Previous Image
	SDL_DestroyTexture(_interactiveTextTexture[index]);

	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture[index] = textTexture;

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture[index], NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = _interactiveTextRectangle[index].x;
	textRectangle.y = _interactiveTextRectangle[index].y;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle[index] = textRectangle;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture[index], NULL, &_interactiveTextRectangle[index]);
}

void UISystem::UpdateTextAtCenter(std::string message, int index)
{
	// Destroy Previous Image
	SDL_DestroyTexture(_interactiveTextTexture[index]);

	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture[index] = textTexture;

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture[index], NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = (_interactiveTextRectangle[index].x + _interactiveTextRectangle[index].w / 2) - width / 2;
	textRectangle.y = _interactiveTextRectangle[index].y;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle[index] = textRectangle;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture[index], NULL, &_interactiveTextRectangle[index]);
}

void UISystem::UpdateTextColoured(std::string message, int index, Uint8 r, Uint8 b, Uint8 g, Uint8 a)
{
	// Destroy Previous Image
	SDL_DestroyTexture(_interactiveTextTexture[index]);
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ r, g, b, a });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture[index] = textTexture;

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture[index], NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = _interactiveTextRectangle[index].x;
	textRectangle.y = _interactiveTextRectangle[index].y;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle[index] = textRectangle;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture[index], NULL, &_interactiveTextRectangle[index]);
}

// Display Text
void UISystem::CreateDisplayText(std::string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_displayTextTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_displayTextTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x - width / 2;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_displayTextRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _displayTextTexture.back(), NULL, &_displayTextRectangle.back());
}

void UISystem::CreateDisplayTextColoured(std::string message, int x, int y, Uint8 r, Uint8 b, Uint8 g, Uint8 a)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ r, g, b, a });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_displayTextTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_displayTextTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x - width / 2;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_displayTextRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _displayTextTexture.back(), NULL, &_displayTextRectangle.back());
}

void UISystem::DeleteDisplayTextByID(int id)
{
	if (id >= 0 && id < _displayTextTexture.size())
		SDL_DestroyTexture(_displayTextTexture[id]);
}

void UISystem::UpdateDisplayText(std::string message, int index)
{
	// Destroy Previous Image
	SDL_DestroyTexture(_displayTextTexture[index]);

	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_displayTextTexture[index] = textTexture;

	int width, height;
	SDL_QueryTexture(_displayTextTexture[index], NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = _displayTextRectangle[index].x;
	textRectangle.y = _displayTextRectangle[index].y;
	textRectangle.w = width;
	textRectangle.h = height;
	_displayTextRectangle[index] = textRectangle;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _displayTextTexture[index], NULL, &_displayTextRectangle[index]);
}

void UISystem::UpdateDisplayTextColoured(std::string message, int index, Uint8 r, Uint8 b, Uint8 g, Uint8 a)
{
	// Destroy Previous Image
	SDL_DestroyTexture(_displayTextTexture[index]);

	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ r, g, b, a });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_displayTextTexture[index] = textTexture;

	int width, height;
	SDL_QueryTexture(_displayTextTexture[index], NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = _displayTextRectangle[index].x;
	textRectangle.y = _displayTextRectangle[index].y;
	textRectangle.w = width;
	textRectangle.h = height;
	_displayTextRectangle[index] = textRectangle;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _displayTextTexture[index], NULL, &_displayTextRectangle[index]);
}

void UISystem::CreateBackButton(std::string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_backButtonTexture = textTexture;

	int width, height;
	SDL_QueryTexture(_backButtonTexture, NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x - width / 2;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_backButton = textRectangle;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _backButtonTexture, NULL, &_backButton);
}

void UISystem::CreateLobbyButton(std::string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_lobbyButtonTexture = textTexture;

	int width, height;
	SDL_QueryTexture(_lobbyButtonTexture, NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x - width / 2;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_lobbyButton = textRectangle;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _lobbyButtonTexture, NULL, &_lobbyButton);
}

void UISystem::CreateReadyButton(std::string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_readyButtonTexture = textTexture;

	int width, height;
	SDL_QueryTexture(_readyButtonTexture, NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x - width / 2;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_readyButton = textRectangle;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _readyButtonTexture, NULL, &_readyButton);
}


std::vector<SDL_Rect>& UISystem::GetDisplayTextRectangle()
{
	return _displayTextRectangle;
}

std::vector<SDL_Rect>& UISystem::GetInteractiveTextRectangle()
{
	return _interactiveTextRectangle;
}

SDL_Rect & UISystem::GetBackButton()
{
	return _backButton;
}

SDL_Rect & UISystem::GetLobbyButton()
{
	return _lobbyButton;
}

SDL_Rect & UISystem::GetReadyButton()
{
	return _readyButton;
}

void UISystem::DeleteBackButton()
{
	SDL_DestroyTexture(_backButtonTexture);
}

void UISystem::DeleteLobbyButton()
{
	SDL_DestroyTexture(_lobbyButtonTexture);
}

void UISystem::DeleteReadyButton()
{
	SDL_DestroyTexture(_readyButtonTexture);
}

std::vector<Entity*> UISystem::GetCharacters()
{
	return _characters;
}

std::vector<Entity*> UISystem::GetCheckpoints()
{
	return _checkpoints;
}

Entity * UISystem::GetFlag()
{
	return _flag;
}
