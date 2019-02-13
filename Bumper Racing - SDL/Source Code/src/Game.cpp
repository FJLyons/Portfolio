#include <string>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "Game.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"

#include <assert.h>
#include "PhysicsComponent.h"
#include "WeaponComponent.h"
#include "ParticleComponent.h"
#include "StatusEffectComponent.h"

#include "SpriteComponent.h"
#include "Vector2b.h"

Game::Game() 
	: _gravity(0.f, 0.f)
	, _world(_gravity)
	, _waypoints()
{
	_running = false;
	_textureHolder = std::map<TextureID, SDL_Texture*>();
}

Game::~Game()
{

}

void Game::Initialize(SDL_Renderer* renderer, const std::vector<int>& ids, int level)
{
	Scene::Initialize(renderer);

	_systemManager.Initialize(_renderer, &_entityFactory, &_bodyFactory, &_world, SCREEN_WIDTH, SCREEN_HEIGHT);
	_particleSystem.Initialize();

	_world.SetAllowSleeping(false);

	_entityFactory.Initialize(&_textureHolder);
	_bodyFactory.Initialize(&_world);

	Start();
	LoadContent(ids, level);

	_player = nullptr;

	CreateUI();

	_systemManager.PostInitialize(_player, &_waypoints);

	CreateParticles();

	_swapScene = CurrentScene::GAME;
	BindInput();
}

void Game::LoadContent(const std::vector<int>& ids, int level)
{
	_textureHolder[TextureID::TilemapSpriteSheet] = LoadTexture("Media/Textures/SpriteSheetFull.png");

	_textureHolder[TextureID::Radar] = LoadTexture("Media/Player/Radar.png");
	_textureHolder[TextureID::Bullet] = LoadTexture("Media/Player/Bullets.png");
	_textureHolder[TextureID::Weapon] = LoadTexture("Media/Player/Weapons.png");
	_textureHolder[TextureID::Flag] = LoadTexture("Media/Player/Flag.png");
	_textureHolder[TextureID::Player] = LoadTexture("Media/Player/player.png");
	_textureHolder[TextureID::Checkpoint] = LoadTexture("Media/Textures/Checkpoint.png");
	_textureHolder[TextureID::PowerUp] = LoadTexture("Media/Textures/PowerUps.png");

	_textureHolder[TextureID::Particle] = LoadTexture("Media/Textures/Checkpoint.png");

	_textureHolder[TextureID::UI] = LoadTexture("Media/UI/UI.png");

	_textureHolder[TextureID::EntitySpriteSheet] = LoadTexture("Media/Textures/EntitySprite.png");
	char* levelPath;
	if (level % 2 == 1) 
		levelPath = "Media/Json/NormalMap.json";
	else
		levelPath = "Media/Json/RockMap.json";

	_levelLoader.LoadJson(levelPath, _systemManager, &_bodyFactory, &_waypoints, ids);
}


int Game::Update()
{
	unsigned int currentTime = LTimer::gameTime();		//millis since game started
	float dt = (float)(currentTime - _lastTime) / 1000.0f;	//time since last update
	NetworkHandler::Instance().gameTime += dt;
	// UPDATE HERE //
	// Use yo Update using Poll Event (Menus, single presses)
	_inputManager->ProcessInput();
	// Use to Update constantly at frame rate
	_inputManager->ConstantInput();
	_particleSystem.Process(dt);

	_systemManager.Process(dt);
	UpdateUI();

	_world.Step(1 / (float)SCREEN_FPS, 8, 3);
	//save the curent time for next frame
	_lastTime = currentTime;

	return (int)_swapScene;
}

void Game::Render()
{
	SDL_RenderClear(_renderer);	

	//RENDER HERE
	_systemManager.Render();

	_particleSystem.Render(_renderer, &_systemManager.GetCamera());

	SDL_RenderPresent(_renderer);
}

bool Game::IsRunning()
{
	if (_swapScene != CurrentScene::GAME) { _swapScene = CurrentScene::GAME; }
	return _running;
}

void Game::Start()
{
	_running = true;
	_swapScene = CurrentScene::GAME;

	_lastTime = LTimer::gameTime();		
	if (!_audioManager->IsMusicPlaying())
	{
		int random = std::rand() % 3;
		if (random == 0) { _audioManager->PlayMusic("Music1"); }
		else if (random == 1) { _audioManager->PlayMusic("Music2"); }
		else if (random == 2) { _audioManager->PlayMusic("Music3"); }
	}
}

void Game::Stop()
{
	_running = false;
	CleanUp();
	_audioManager->StopMusic();
}

void Game::OnEvent(Event evt, Type typ)
{
	if (_running)
	{
		switch (typ)
		{
		case Type::Press:
			switch (evt)
			{
			case Event::ESCAPE:
				//_inputManager->saveFile();
				_running = false;
				break;
			}
			break;

		case Type::Down:
			switch (evt)
			{
			case Event::w:
				_audioManager->PlayFX("Hum");
				break;
			case Event::a:
				_audioManager->PlayFX("Hum");
				break;
			case Event::s:
				_audioManager->PlayFX("Hum");
				break;
			case Event::d:
				_audioManager->PlayFX("Hum");
				break;
			}
			break;

		default:
			break;
		}
	}
}


void Game::BindInput()
{
	// In
	Command* wIn = new InputCommand([&]()
	{
		StatusEffectComponent* effects = static_cast<StatusEffectComponent*>(_player->GetComponent(Component::Type::StatusEffect));
		if (!effects->staggered)
		{
			PhysicsComponent* physics = static_cast<PhysicsComponent*>(_player->GetComponent(Component::Type::Physics));

			if (physics->yVelocity > 0)
			{
				physics->yVelocity *= 0.975f;
			}

			physics->yDir = -1;
		}
	} , Type::Down);
	_inputManager->AddKey(Event::w, wIn, this);

	Command* aIn = new InputCommand([&]()
	{
		StatusEffectComponent* effects = static_cast<StatusEffectComponent*>(_player->GetComponent(Component::Type::StatusEffect));
		if (!effects->staggered) 
		{
			PhysicsComponent* physics = static_cast<PhysicsComponent*>(_player->GetComponent(Component::Type::Physics));

			if (physics->xVelocity > 0)
			{
				physics->xVelocity *= 0.975f;
			}

			physics->xDir = -1;
		}
	} , Type::Down);
	_inputManager->AddKey(Event::a, aIn, this);

	Command* sIn = new InputCommand([&]()
	{
		StatusEffectComponent* effects = static_cast<StatusEffectComponent*>(_player->GetComponent(Component::Type::StatusEffect));
		if (!effects->staggered) 
		{
			PhysicsComponent* physics = static_cast<PhysicsComponent*>(_player->GetComponent(Component::Type::Physics));

			if (physics->yVelocity < 0)
			{
				physics->yVelocity *= 0.975f;
			}

			physics->yDir = 1;
		}
	} , Type::Down);
	_inputManager->AddKey(Event::s, sIn, this);

	Command* dIn = new InputCommand([&]()
	{
		StatusEffectComponent* effects = static_cast<StatusEffectComponent*>(_player->GetComponent(Component::Type::StatusEffect));
		if (!effects->staggered) 
		{
			PhysicsComponent* physics = static_cast<PhysicsComponent*>(_player->GetComponent(Component::Type::Physics));

			if (physics->xVelocity < 0)
			{
				physics->xVelocity *= 0.975f;
			}

			physics->xDir = 1;
		}
	} , Type::Down);
	_inputManager->AddKey(Event::d, dIn, this);


	// Up
	Command* wUp = new InputCommand([&]()
	{
		PhysicsComponent* physics = static_cast<PhysicsComponent*>(_player->GetComponent(Component::Type::Physics));
		physics->yDir = 0;
	}, Type::Release);
	_inputManager->AddKey(Event::w, wUp, this);

	Command* aUp = new InputCommand([&]()
	{
		PhysicsComponent* physics = static_cast<PhysicsComponent*>(_player->GetComponent(Component::Type::Physics));
		physics->xDir = 0;
	}, Type::Release);
	_inputManager->AddKey(Event::a, aUp, this);

	Command* sUp = new InputCommand([&]()
	{
		PhysicsComponent* physics = static_cast<PhysicsComponent*>(_player->GetComponent(Component::Type::Physics));
		physics->yDir = 0;
	}, Type::Release);
	_inputManager->AddKey(Event::s, sUp, this);

	Command* dUp = new InputCommand([&]()
	{
		PhysicsComponent* physics = static_cast<PhysicsComponent*>(_player->GetComponent(Component::Type::Physics));
		physics->xDir = 0;
	}, Type::Release);
	_inputManager->AddKey(Event::d, dUp, this);


	// Back to Main Menu
	Command* backIn = new InputCommand([&]() {
		NetworkHandler::Instance().Disconnect(); 
		_swapScene = Scene::CurrentScene::MAIN_MENU; }, Type::Press);
	_inputManager->AddKey(Event::BACKSPACE, backIn, this);

	// Exit Game
	_inputManager->AddListener(Event::ESCAPE, this);

	Command* spaceIn = new InputCommand([&]()
	{
		WeaponComponent* weapon = static_cast<WeaponComponent*>(_player->GetComponent(Component::Type::Weapon));
		
		if ( weapon->hasWeapon)
		{
			if (!static_cast<StatusEffectComponent*>(_player->GetComponent(Component::Type::StatusEffect))->staggered)
			{
				if (NetworkHandler::Instance().GetPlayerID() != -1)
				{
					FireData data;
					data.remoteID = NetworkHandler::Instance().GetPlayerID();
					NetworkHandler::Instance().Send(&data);
					std::cout << "sending fire data" << std::endl;
				}
				weapon->fired = true;
				_audioManager->PlayFX("Weapon");
			}
		}
	}, Type::Hold);
	_inputManager->AddKey(Event::SPACE, spaceIn, this);

}

void Game::CleanUp()
{
	//DESTROY HERE
	_inputManager->EmptyKeys();

	_world.SetAllowSleeping(true);
}

void Game::DebugBox2D()
{
	//DEBUG ASTAR
	
	//SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

	_waypoints.drawNodes(_renderer, &_systemManager.GetCamera());

	_waypoints.drawArcs(_renderer, &_systemManager.GetCamera());
	//DEBUG BOX2D
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);

	///////////////////use this code for testing purpose///////////////////////////////////////////////////////////////
	for (b2Body* BodyIterator = _world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
	{
		if (BodyIterator->IsActive())
		{
			if (BodyIterator->GetUserData())
			{
				SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
			}
			else
			{
				Entity* e = static_cast<Entity*>(BodyIterator->GetUserData());
				EntityType t = e->GetType();
				if (t == EntityType::Player)
				{
					SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
				}
				else if (t == EntityType::Checkpoint)
				{
					SDL_SetRenderDrawColor(_renderer, 127, 255, 212, 255);
				}
				else if (t == EntityType::Point)
				{
					SDL_SetRenderDrawColor(_renderer, 0, 0, 50, 255);
				}
				else if (t == EntityType::Flag)
				{
					SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 255);
				}
				else
				{
					SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
				}
			}

			for (b2Fixture* b2Fixture = BodyIterator->GetFixtureList(); b2Fixture != 0; b2Fixture = b2Fixture->GetNext())
			{
				b2Shape::Type shapeType = b2Fixture->GetType();
				if (shapeType == b2Shape::e_circle)
				{
					
				}
				else if (shapeType == b2Shape::e_polygon)
				{

					b2PolygonShape* polygonShape = (b2PolygonShape*)b2Fixture->GetShape();

					int lenght = (int)polygonShape->GetVertexCount();

					SDL_Point* points = new SDL_Point[lenght + 1];


					for (int i = 0; i < lenght; i++)
					{
						Point worldPoint;
						float verticesPosX = polygonShape->GetVertex(i).x; b2Fixture->GetBody()->GetPosition().x;
						float verticesPosY = polygonShape->GetVertex(i).y; b2Fixture->GetBody()->GetPosition().y;
						/*
						float mag = sqrt(fixturePosX * fixturePosX + fixturePosY * fixturePosY);
						if (mag != 0)
						{
						fixturePosX /= mag;
						fixturePosY /= mag;
						}*/
						float angle = b2Fixture->GetBody()->GetAngle();
						float s = sin(angle);
						float c = cos(angle);

						// translate point back to origin:
						verticesPosX -= 0;
						verticesPosY -= 0;

						// rotate point
						float xnew = verticesPosX* c - verticesPosY * s;
						float ynew = verticesPosX * s + verticesPosY * c;

						// translate point back:
						verticesPosX = xnew + 0;
						verticesPosY = ynew + 0;

						worldPoint.x = metersToPixels(verticesPosX + b2Fixture->GetBody()->GetPosition().x);
						worldPoint.y = metersToPixels(verticesPosY + b2Fixture->GetBody()->GetPosition().y);
						worldPoint = _systemManager.GetCamera().worldToScreen(worldPoint);
						points[i].x = worldPoint.x;
						points[i].y = worldPoint.y;
					}

					points[lenght].y = points[0].y;
					points[lenght].x = points[0].x;
					
					SDL_RenderDrawLines(_renderer, points, lenght + 1);
					delete points;
				}
			}
		}
	}

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderPresent(_renderer);
}

void Game::CreateUI()
{
	// Poll Boxes
	std::vector<float> pollPosition1 = std::vector<float>();
	pollPosition1.push_back(0); //id
	pollPosition1.push_back(UI_BOX_X); //xPosition
	pollPosition1.push_back(UI_BOX_Y); //yPosition
	pollPosition1.push_back(UI_BOX_SIZE); //width
	pollPosition1.push_back(UI_BOX_SIZE); //height
	_systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::UI, pollPosition1));

	std::vector<float> pollPosition2 = std::vector<float>();
	pollPosition2.push_back(2); //id
	pollPosition2.push_back(UI_BOX_X); //xPosition
	pollPosition2.push_back(UI_BOX_Y * 2); //yPosition
	pollPosition2.push_back(UI_BOX_SIZE); //width
	pollPosition2.push_back(UI_BOX_SIZE); //height
	_systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::UI, pollPosition2));

	std::vector<float> pollPosition3 = std::vector<float>();
	pollPosition3.push_back(3); //id
	pollPosition3.push_back(UI_BOX_X); //xPosition
	pollPosition3.push_back(UI_BOX_X * 3); //yPosition
	pollPosition3.push_back(UI_BOX_SIZE); //width
	pollPosition3.push_back(UI_BOX_SIZE); //height
	_systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::UI, pollPosition3));

	// Weapon Box
	std::vector<float> weapon = std::vector<float>();
	weapon.push_back(4); //id
	weapon.push_back(SCREEN_WIDTH * 0.925f); //xPosition
	weapon.push_back(UI_BOX_Y); //yPosition
	weapon.push_back(UI_BOX_SIZE); //width
	weapon.push_back(UI_BOX_SIZE); //height
	_systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::UI, weapon));

	// Lap Text
	_systemManager.GetUISystem()->CreateDisplayText("Lap : " + std::to_string(_systemManager.GetUISystem()->currentLapLocal),
		SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.05f);

	// Poll Text
	_systemManager.GetUISystem()->CreateDisplayText("1", UI_POLL_TEXT_X, UI_POLL_TEXT_Y);
	_systemManager.GetUISystem()->CreateDisplayText("2", UI_POLL_TEXT_X, UI_POLL_TEXT_Y * 2);
	_systemManager.GetUISystem()->CreateDisplayText("3", UI_POLL_TEXT_X, UI_POLL_TEXT_Y * 3);

	// Next Checkpoint Text
	_systemManager.GetUISystem()->CreateTextAtCenter("1", UI_NEXT_TEXT_X, UI_NEXT_TEXT_Y);
	_systemManager.GetUISystem()->CreateTextAtCenter("1", UI_NEXT_TEXT_X, UI_NEXT_TEXT_Y * 2);
	_systemManager.GetUISystem()->CreateTextAtCenter("1", UI_NEXT_TEXT_X, UI_NEXT_TEXT_Y * 3);

	// Ammo Text
	_systemManager.GetUISystem()->CreateTextAtCenter("0", SCREEN_WIDTH * 0.945f, UI_BOX_Y);

	// Radar
	std::vector<float> data = std::vector<float>();
	data.push_back(20000); //id
	_systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::Radar, data));
}

void Game::UpdateUI()
{
	_systemManager.GetUISystem()->HUD();
	
	_systemManager.GetUISystem()->UpdateDisplayText("Lap : " + std::to_string(_systemManager.GetUISystem()->currentLapLocal), 0);
	_systemManager.GetUISystem()->UpdateTextAtCenter(std::to_string(_systemManager.GetUISystem()->nextCheckpoint[0]), 0);
	_systemManager.GetUISystem()->UpdateTextAtCenter(std::to_string(_systemManager.GetUISystem()->nextCheckpoint[1]), 1);
	_systemManager.GetUISystem()->UpdateTextAtCenter(std::to_string(_systemManager.GetUISystem()->nextCheckpoint[2]), 2);
	_systemManager.GetUISystem()->UpdateTextAtCenter(std::to_string(_systemManager.GetUISystem()->currentAmmoLocal), 3);

	
	if (_systemManager.GetUISystem()->isTop < 3)
	{
		int top = _systemManager.GetUISystem()->isTop;
		_systemManager.GetUISystem()->UpdateTextColoured(std::to_string(_systemManager.GetUISystem()->nextCheckpoint[top]), top, 255, 0, 255, 255);
	}

	if (_systemManager.GetUISystem()->gameOver == true)
	{
		if (_systemManager.GetUISystem()->win == true)
		{
			_win = true;
			_swapScene = Scene::CurrentScene::GAMEOVER;
		}
		else
		{
			_win = false;
			_swapScene = Scene::CurrentScene::GAMEOVER;
		}
	}
}

void Game::CreateParticles()
{
	// Particles
	vector<ParticleManager::ParticleSettings> settingsVec = vector<ParticleManager::ParticleSettings>();
	ParticleManager::ParticleSettings settings = ParticleManager::ParticleSettings();
	settings._minTTL = 5;
	settings._maxTTL = 10;
	settings._startingVelocity = 100;
	settings._endingVelocity = 1;
	settings._emissionRate = 0.25;
	settings._particleSize = 30;
	settings._texture = _textureHolder[TextureID::Particle];
	settings._offsetFromParent = Vector2b(-1, -1);
	settings._positionToParentTo = new Vector2b(-1, -1);
	settings._startingDirection = new Vector2b(-1, -1);
	settings._shapeType = Shape::NULL_SHAPE;

	settingsVec.push_back(settings);

	vector<Vector2b *> pos = vector<Vector2b*>();
	TransformComponent* transform = static_cast<TransformComponent*>(_player->GetComponent(Component::Type::Transform));
	Vector2b trans = Vector2b(transform->origin.x, transform->origin.y);
	pos.push_back(&trans);

	ParticleComponent* partcle = new ParticleComponent(pos, settingsVec, _renderer);
	_player->AddComponent(partcle);

	_particleSystem.AddEntity(_player);
}
