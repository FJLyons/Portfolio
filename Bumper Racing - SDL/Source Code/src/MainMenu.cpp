#include "MainMenu.h"

#include "ConstHolder.h"

#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "NetworkHandler.h"

MainMenu::MainMenu()
	: _uiSystem(0)
{
	_running = false;
	_textureHolder = std::map<TextureID, SDL_Texture*>();
}


MainMenu::~MainMenu()
{
}

void MainMenu::Initialize(SDL_Renderer* renderer)
{
	Scene::Initialize(renderer);

	_selectedItemIndex = 0;

	_uiSystem.Initialize(_renderer);

	Start();
	LoadContent();

	Entity* ui = new Entity(EntityType::UI);
	SpriteComponent* spriteComponent = new SpriteComponent((_textureHolder)[TextureID::UI], 0);
	ui->AddComponent(spriteComponent);
	ui->AddComponent(new TransformComponent(spriteComponent->sourceRect.w*0.5f, spriteComponent->sourceRect.h*0.5f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	_uiSystem.AddEntity(ui);
	
	//Input
	BindInput();
}

int MainMenu::Update()
{
	unsigned int currentTime = LTimer::gameTime();		//millis since game started
	float dt = (float)(currentTime - _lastTime) / 1000.0f;	//time since last update
	NetworkHandler::Instance().gameTime += dt;
	//UPDATE HERE
	// Use yo Update using Poll Event (Menus, single presses)
	_inputManager->ProcessInput();

	//save the curent time for next frame
	_lastTime = currentTime;

	return (int)_swapScene;
}

void MainMenu::Render()
{
	SDL_RenderClear(_renderer);

	//RENDER HERE
	_uiSystem.Process(0);

	SDL_RenderPresent(_renderer);
}

bool MainMenu::IsRunning()
{
	if (_swapScene != CurrentScene::MAIN_MENU) { _swapScene = CurrentScene::MAIN_MENU; }
	return _running;
}

void MainMenu::Start()
{
	_running = true;
	_swapScene = CurrentScene::MAIN_MENU;

	if (!_audioManager->IsMusicPlaying())
	{
		_audioManager->PlayMusic("MusicMenu");
	}
}

void MainMenu::Stop()
{
	_running = false;
	CleanUp();
}

void MainMenu::OnEvent(Event evt, Type typ)
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
			case Event::w:
				_audioManager->PlayFX("Click");
				break;
			case Event::s:
				_audioManager->PlayFX("Click");
				break;
			case Event::RETURN:
				_audioManager->PlayFX("Enter");
				break;
			}
			break;

		default:
			break;
		}
	}
}

void MainMenu::LoadContent()
{
	// Text
	_uiSystem.CreateText("Arcade Mode", 25, 450);
	_uiSystem.CreateText("Multiplayer", _uiSystem.GetInteractiveTextRectangle()[0].x + _uiSystem.GetInteractiveTextRectangle()[0].w / 2, _uiSystem.GetInteractiveTextRectangle()[0].y + 100);
	_uiSystem.CreateText("Options", _uiSystem.GetInteractiveTextRectangle()[1].x + _uiSystem.GetInteractiveTextRectangle()[1].w / 2, _uiSystem.GetInteractiveTextRectangle()[1].y + 100);
	_uiSystem.CreateText("About", _uiSystem.GetInteractiveTextRectangle()[2].x + _uiSystem.GetInteractiveTextRectangle()[2].w / 2, _uiSystem.GetInteractiveTextRectangle()[2].y + 100);
	_uiSystem.CreateText("Quit", _uiSystem.GetInteractiveTextRectangle()[3].x + _uiSystem.GetInteractiveTextRectangle()[3].w / 2, _uiSystem.GetInteractiveTextRectangle()[3].y + 100);

	_uiSystem.CreateDisplayTextColoured(">", _uiSystem.GetInteractiveTextRectangle()[0].x + _uiSystem.GetInteractiveTextRectangle()[0].w + 50, _uiSystem.GetInteractiveTextRectangle()[0].y, 255, 0, 0, 255);

	//TTF_CloseFont(_font); // Free Font Memory

	_textureHolder[TextureID::UI] = LoadTexture("Media/Menus/MainMenu.png");
}

void MainMenu::CleanUp()
{
	_inputManager->EmptyKeys();
}

void MainMenu::BindInput()
{
	Command* enterIn = new InputCommand([&]() 
	{ 
		if (_selectedItemIndex == _uiSystem.GetInteractiveTextRectangle().size() -1) { _running = false; }
		else { _swapScene = static_cast<CurrentScene>(_selectedItemIndex + 1); }
		if (_swapScene == CurrentScene::GAME) { _audioManager->StopMusic(); }
	}, Type::Press);

	_inputManager->AddKey(Event::RETURN, enterIn, this);

	Command* mlIn = new InputCommand([&]()
	{
		SDL_Point mousePos = _inputManager->GetMousePos();
		SDL_Rect mouseRect = { mousePos.x, mousePos.y, 1, 1 };

		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle().back()))) { _running = false; }
		for (int i = 0; i < _uiSystem.GetInteractiveTextRectangle().size() - 1; i++)
		{
			if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[i]))) 
			{ 
				_selectedItemIndex = i;
				_swapScene = static_cast<CurrentScene>(_selectedItemIndex + 1);
				if (_swapScene == CurrentScene::GAME) { _audioManager->StopMusic(); }
				_audioManager->PlayFX("Enter");
			}
		}

	}, Type::Press);

	_inputManager->AddKey(Event::MOUSE_LEFT, mlIn, this);

	Command* sIn = new InputCommand([&]() { MainMenu::MoveDown(); }, Type::Press);
	_inputManager->AddKey(Event::s, sIn, this);

	Command* wIn = new InputCommand([&]() { MainMenu::MoveUp(); }, Type::Press);
	_inputManager->AddKey(Event::w, wIn, this);

	_inputManager->AddListener(Event::ESCAPE, this);
}

void MainMenu::MoveUp()
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

void MainMenu::MoveDown()
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

int MainMenu::GetPressedItem()
{
	// Index in menu
	return _selectedItemIndex;
}