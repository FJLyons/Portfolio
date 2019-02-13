#pragma once

#include "Scene.h"
#include "NetworkHandler.h"
#include "UISystem.h"

class Lobby : public Scene
{
private:
	struct Session {
		int id;
		int currentPlayers;
	};

public:
									Lobby();
									~Lobby();

	void							Initialize(SDL_Renderer* renderer, std::vector<int>* ids);

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
	
	void							Refresh(const std::vector<Session>& sessions = std::vector<Session>(), int maxPlayers = 0);
	void							Refresh(const std::vector<int>& players, std::vector<bool> ready = std::vector<bool>());

	int								GetPressedItem();

private:
	UISystem						_uiSystem;

	std::vector<int>*				_ids;

	float							_connectTimer;
	float							_readyTimer;

	int								_selectedItemIndex;
	int								_countdownTextId;

	std::vector<Session>			_session;

	bool							_startReadyTimer;
	bool							_startingGame;
	std::vector<float>				_serverDeltas;
	const float						PING_SEND_RATE = 0.2f;
	const int						PING_SEND_COUNT = 5;
	float							_pingTimer;
	bool							_pinging;
	float							_gameStartTime;

	const int						READY_COUNTDOWN = 3;
};

