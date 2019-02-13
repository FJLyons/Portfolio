#include "Lobby.h"

#include "ConstHolder.h"

#include <string>
#include <sstream>

#include "TransformComponent.h"
#include "SpriteComponent.h"

Lobby::Lobby()
	: _uiSystem(0.f)
	, _startingGame(false)
	, _connectTimer(0.f)
	, _readyTimer(READY_COUNTDOWN)
	, _startReadyTimer(false)
	, _countdownTextId(-1)
	, _pingTimer(0.f)
	, _pinging(true)
{
	_running = false;
	_textureHolder = std::map<TextureID, SDL_Texture*>();
}

Lobby::~Lobby()
{
}

void Lobby::Initialize(SDL_Renderer* renderer, std::vector<int>* ids)
{
	Scene::Initialize(renderer);

	_ids = ids;

	_running = true;
	_selectedItemIndex = 0;

	// UI
	_uiSystem.Initialize(_renderer);

	Start();// - activate when loaded
	LoadContent();

	Entity* ui = new Entity(EntityType::UI);
	SpriteComponent* spriteComponent = new SpriteComponent((_textureHolder)[TextureID::UI], 0);
	ui->AddComponent(spriteComponent);
	ui->AddComponent(new TransformComponent(spriteComponent->sourceRect.w*0.5f, spriteComponent->sourceRect.h*0.5f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	_uiSystem.AddEntity(ui);

	Refresh();

	_lastTime = LTimer::gameTime();
	_readyTimer = READY_COUNTDOWN;
}

int Lobby::Update()
{
	unsigned int currentTime = LTimer::gameTime();		//millis since game started
	float dt = (float)(currentTime - _lastTime) / 1000.0f;	//time since last update
	NetworkHandler::Instance().gameTime += dt;
	//UPDATE HERE	
	// Use yo Update using Poll Event (Menus, single presses)
	_inputManager->ProcessInput();
	//save the curent time for next frame

	NetworkHandler& network = NetworkHandler::Instance();

	if (network.GetConnected() == false)
	{
		_connectTimer += dt;
		if (_connectTimer > CONNECT_RETRY)
		{
			ConnectData data;
			network.Send(&data);
			_connectTimer = 0.f;
		}
	}

	if (_pinging)
	{
		_pingTimer += dt;
		if (_pingTimer > PING_SEND_RATE)
		{
			PingData ping;
			network.Send(&ping);
			_pingTimer = 0.f;
		}
	}
	ReceivedData receivedData = network.Receive();

	if (receivedData.Empty() == false)
	{

		switch (receivedData.GetType())
		{
		case MessageType::Connect:
		{
			ConnectData data = receivedData.GetData<ConnectData>();
			std::cout << "Got assigned ID: " << data.id << std::endl;
			network.SetPlayerID(data.id);
			network.SetConnected(true);
			break;
		}
		case MessageType::Ping:
		{
			PingData data = receivedData.GetData<PingData>();
			float rtt = network.gameTime - data.ts;
			float serverTimeDelta = (data.serverTime - network.gameTime) + (rtt * 0.5f);
			if (_serverDeltas.empty()) //update gametime the first time
				network.gameTime += serverTimeDelta;
			_serverDeltas.push_back(serverTimeDelta);
			if (_serverDeltas.size() > PING_SEND_COUNT)
			{
				_serverDeltas.erase(_serverDeltas.begin());
				_pinging = false;
				//take median
				float median;
				int size = _serverDeltas.size();

				sort(_serverDeltas.begin(), _serverDeltas.end());

				if (size % 2 == 0)
				{
					median = (_serverDeltas[size / 2 - 1] + _serverDeltas[size / 2]) / 2;
				}
				else
				{
					median = _serverDeltas[size / 2];
				}
				network.serverTimeDelta = median;
				network.gameTime += network.serverTimeDelta;
			}
			std::cout << "GameTime: " << data.ts << ", serverTime: " << data.serverTime << std::endl;
			break;
		}		
		case MessageType::CheckConnection:
		{
			CheckConnectionData data = receivedData.GetData<CheckConnectionData>();
			network.Send(&data);
			break;
		}
		case MessageType::SessionList:
		{
			SessionListData data = receivedData.GetData<SessionListData>();
			std::cout << "Sessions Available: " << data.count << std::endl;
			std::vector<Session> sessions;
			for (int i = 0; i < data.count; i++)
			{
				Session s;
				s.id = data.sessionIDs[i];
				s.currentPlayers = data.currentPlayers[i];
				std::cout << "Session ID: " << s.id;
				std::cout << "  Players: " << s.currentPlayers << "/" << data.maxPlayers << std::endl;
				sessions.push_back(s);
			}
			Refresh(sessions, data.maxPlayers);
			break;
		}//two below are sent at the same time in response to client sending joinSession. 
		case MessageType::JoinSession:
		{
			JoinSessionData data = receivedData.GetData<JoinSessionData>(); //client gets session id, switch scene to player list, get id before or after scene switch??
			std::cout << "Joined session: " << data.sessionID << std::endl;
			network.SetSessionID(data.sessionID);
			network.SetHost(data.host);
			std::cout << "host :  " << data.host << std::endl;
			std::cout << "my id: " << NetworkHandler::Instance().GetPlayerID() << std::endl;
			break;
		}
		case MessageType::PlayerList:
		{
			std::cout << "Player list data in lobby received" << std::endl;
			PlayerListData data = receivedData.GetData<PlayerListData>();
			for (int i = 0; i < data.count; i++)
			{
				std::cout << "Player: " << data.players[i] << std::endl;
			}
			std::cout << "my id: " << NetworkHandler::Instance().GetPlayerID() << std::endl;
			Refresh(data.players);
			break;
		}
		case MessageType::Ready:
		{
			std::cout << "Got ready message" << std::endl;
			ReadyData data = receivedData.GetData<ReadyData>();
			for (int i = 0; i < data.ids.size(); i++)
			{
				std::cout << "ID: " << data.ids[i] << " Ready: " << data.ready[i] << std::endl;
			}
			*_ids = data.ids;
			if (data.allReady)
			{
				_startReadyTimer = true;
				_gameStartTime = data.gameStartTime;
				std::cout << "Seeded" << std::endl;
				srand(data.seed);
			}
			Refresh(data.ids, data.ready);
			break;
		}
		case MessageType::SetHost:
		{
			std::cout << "Becoming new host" << std::endl;
			SetHostData data = receivedData.GetData<SetHostData>();
			network.SetHost(true);
			break;
		}
		//TODO: have to add in host change here, need host? make it so 4 players required to start and then they ready up
		}
	}

	if (_startReadyTimer)
	{
		_uiSystem.UpdateDisplayTextColoured("Game starting in....." + std::to_string(_readyTimer), 
			_uiSystem.GetDisplayTextRectangle().size() - 1, 255, 0, 0, 255); // Update back text - is blank
		_readyTimer -= dt;
		if (network.gameTime > _gameStartTime)
		{
			_startingGame = true;
		}
	}



	_lastTime = currentTime;
	if (_startingGame)
		return (int)CurrentScene::GAME;
	else
		return (int)_swapScene;
}

void Lobby::Render()
{
	SDL_RenderClear(_renderer);

	//RENDER HERE
	_uiSystem.Process();

	SDL_RenderPresent(_renderer);
}

bool Lobby::IsRunning()
{
	if (_swapScene != CurrentScene::LOBBY) { _swapScene = CurrentScene::LOBBY; }
	return _running;
}

void Lobby::Start()
{
	//Input
	BindInput();

	ConnectData data;
	NetworkHandler::Instance().Send(&data);

	_swapScene = CurrentScene::LOBBY;
}

void Lobby::Stop()
{
	//_inputManager->ResetKey(Event::BACKSPACE);
	//_inputManager->ResetKey(Event::h);

	_running = false;
	CleanUp();
}

void Lobby::OnEvent(Event evt, Type typ)
{
	if (_running)
	{
		switch (typ)
		{
		case Type::Press:
			switch (evt)
			{
			case Event::ESCAPE:
				_running = false;
				break;
			//case Event::w:
			//	_audioManager->PlayFX("Click");
			//	break;
			//case Event::s:
			//	_audioManager->PlayFX("Click");
			//	break;
			}
			break;

		default:
			break;
		}
	}
}

void Lobby::LoadContent()
{
	_uiSystem.CreateDisplayText("", SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85); // Ready Up Text
	_textureHolder[TextureID::UI] = LoadTexture("Media/Menus/SessionsMenu.png");
}

void Lobby::CleanUp()
{
	_inputManager->EmptyKeys();
}

void Lobby::BindInput()
{
	Command* enterIn = new InputCommand([&]()
	{
		if (_selectedItemIndex == _uiSystem.GetInteractiveTextRectangle().size() - 1) { _running = false; }
		else { _swapScene = static_cast<CurrentScene>(_selectedItemIndex + 1); }
	}, Type::Press);

	_inputManager->AddKey(Event::RETURN, enterIn, this);

	Command* mlIn = new InputCommand([&]()
	{
		SDL_Point mousePos = _inputManager->GetMousePos();
		SDL_Rect mouseRect = { mousePos.x, mousePos.y, 1, 1 };

		// Create New Lobby
		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetLobbyButton())))
		{
			JoinSessionData data;
			NetworkHandler::Instance().Send(&data);
			std::cout << "Create New Lobby" << std::endl;
		}

		// Ready
		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetReadyButton())))
		{
			ReadyData data;
			NetworkHandler::Instance().Send(&data);
			std::cout << "Ready" << std::endl;
		}

		// Back
		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetBackButton())))
		{
			NetworkHandler::Instance().Disconnect();
			_swapScene = Scene::CurrentScene::MAIN_MENU;
			std::cout << "Back" << std::endl;
		}

		// Lobbies
		for (int i = 0; i < _uiSystem.GetInteractiveTextRectangle().size(); i++)
		{
			if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[i])))
			{
				if (_session.size() > 0 && i != _session.size())
				{
					JoinSessionData data;
					data.sessionID = _session[i].id;
					NetworkHandler::Instance().Send(&data);
					std::cout << "ID: " << data.sessionID << std::endl;
				}
			}
		}

	}, Type::Press);

	_inputManager->AddKey(Event::MOUSE_LEFT, mlIn, this);


	Command* rIn = new InputCommand([&]() //ready up
	{
		ReadyData data;
		NetworkHandler::Instance().Send(&data);
	}, Type::Press);
	_inputManager->AddKey(Event::r, rIn, this);

	// Back to Main Menu
	Command* backIn = new InputCommand([&]() { 
		NetworkHandler::Instance().Disconnect();
		_swapScene = Scene::CurrentScene::MAIN_MENU; 
	}, Type::Press);
	_inputManager->AddKey(Event::BACKSPACE, backIn, this);

	// Exit
	_inputManager->AddListener(Event::ESCAPE, this);
}

void Lobby::MoveUp()
{
	if (_selectedItemIndex - 1 >= 0)
	{
		_selectedItemIndex--;
	}
	// Jump to bottom
	else
	{
		// _textRectangle.size() - 2 = 1 before icon
		_selectedItemIndex = _uiSystem.GetInteractiveTextRectangle().size() - 1;
	}
	_uiSystem.GetDisplayTextRectangle().back().x = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].x + _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].w + 50;
	_uiSystem.GetDisplayTextRectangle().back().y = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].y;
}

void Lobby::MoveDown()
{
	if (_selectedItemIndex < _uiSystem.GetInteractiveTextRectangle().size() - 1)
	{
		_selectedItemIndex++;
	}
	// Jump to top
	else
	{
		_selectedItemIndex = 0;
	}
	_uiSystem.GetDisplayTextRectangle().back().x = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].x + _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].w + 50;
	_uiSystem.GetDisplayTextRectangle().back().y = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].y;
}


int Lobby::GetPressedItem()
{
	// Index in menu
	return _selectedItemIndex;
}

void Lobby::Refresh(const std::vector<Session>& sessions, int maxPlayers)
{
	_uiSystem.DeleteText();
	_uiSystem.DeleteDisplayText();
	_uiSystem.DeleteBackButton();
	_uiSystem.DeleteLobbyButton();
	_uiSystem.DeleteReadyButton();

	_uiSystem.CreateDisplayText("", SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85); // Ready Up Text

	if (sessions.empty())
	{
		_uiSystem.CreateText("No sessions available. Create a new one or refresh.", 50, 300);
	}
	for (int i = 0; i < sessions.size(); i++)
	{
		std::ostringstream oss;
		oss << "[" << sessions[i].id << "]" << " - " << sessions[i].currentPlayers << "/" << maxPlayers;
		std::string var = oss.str();

		if (i == 0)
		{
			_uiSystem.CreateText(var, 50, 200);
		}

		else
		{
			_uiSystem.CreateText(var, 50, _uiSystem.GetInteractiveTextRectangle()[i - 1].y + 50);
		}
	}

	_session = sessions;

	_uiSystem.CreateLobbyButton("Create New Lobby", SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85f);
	_uiSystem.CreateBackButton("Back", SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.85f);
}

void Lobby::Refresh(const std::vector<int>& players, std::vector<bool> ready)
{
	if (ready.empty())
	{
		ready = std::vector<bool>(players.size(), false);
	}
	_uiSystem.DeleteText();
	_uiSystem.DeleteDisplayText();
	_uiSystem.DeleteBackButton();
	_uiSystem.DeleteLobbyButton();
	_uiSystem.DeleteReadyButton();

	_uiSystem.CreateDisplayText("Lobbies", SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.5f);
	_uiSystem.CreateDisplayText("________", SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.51f);
	_uiSystem.CreateDisplayText("", SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85); // Ready Up Text

	for (int i = 0; i < players.size(); i++)
	{
		std::ostringstream oss;
		oss << "Player [" << players[i] << "]";

		if (players[i] == NetworkHandler::Instance().GetPlayerID())
		{
			oss << " (you!).";
		}

		std::string var = oss.str();
		if (i == 0)
		{
			if (ready[i])
			{
				_uiSystem.CreateTextColoured(var, 50, 200, 0, 0, 255, 255);
			}
			else
			{
				_uiSystem.CreateText(var, 50, 200);
			}
		}

		else
		{
			if (ready[i])
			{
				_uiSystem.CreateTextColoured(var, 50, _uiSystem.GetInteractiveTextRectangle()[i - 1].y + 50, 0, 0, 255, 255);
			}
			else
			{
				_uiSystem.CreateText(var, 50, _uiSystem.GetInteractiveTextRectangle()[i - 1].y + 50);
			}
		}
	}

	_uiSystem.CreateReadyButton("Ready", SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85f);
	_uiSystem.CreateBackButton("Back", SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.85f);
}
