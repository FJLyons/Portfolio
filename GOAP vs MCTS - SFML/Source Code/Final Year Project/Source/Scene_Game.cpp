#include "Scene_Game.h"

Game::Game()
{
	Init();


	for (int i = 0; i < UIS; i++)
	{
		m_textGOAP[i].setString("");
		m_textGOAP[i].setFont(ResourceManager::GetInstance()->FONT);
		m_textGOAP[i].setPosition(SCREEN_WIDTH * 0.33f, SCREEN_HEIGHT * (0.025f * (i + 1)));
		m_textGOAP[i].setCharacterSize(18);
		m_textGOAP[i].setOutlineColor(sf::Color::Blue);
		m_textGOAP[i].setOutlineThickness(1);

		m_textMCTS[i].setString("");
		m_textMCTS[i].setFont(ResourceManager::GetInstance()->FONT);
		m_textMCTS[i].setPosition(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * (0.025f * (i + 1)));
		m_textMCTS[i].setCharacterSize(18);
		m_textMCTS[i].setOutlineColor(sf::Color::Red);
		m_textMCTS[i].setOutlineThickness(1);
	}

	m_time.setString("0 : 0");
	m_time.setFont(ResourceManager::GetInstance()->FONT);
	m_time.setPosition(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.08f);
	m_time.setCharacterSize(32);
	m_time.setOutlineColor(Grey);
	m_time.setOutlineThickness(2);
	m_time.setOrigin(m_time.getLocalBounds().width / 2.0f, m_time.getLocalBounds().height / 2.0f);

	m_GOAPWins = 0;
	m_MCTSWins = 0;

	m_gameNumber = 1;
	m_gameCounter.setString(std::to_string(m_gameNumber));
	m_gameCounter.setFont(ResourceManager::GetInstance()->FONT);
	m_gameCounter.setPosition(SCREEN_WIDTH * 0.5175f, SCREEN_HEIGHT * 0.125f);
	m_gameCounter.setCharacterSize(24);
	m_gameCounter.setOutlineColor(Grey);
	m_gameCounter.setOutlineThickness(1);
	m_gameCounter.setOrigin(m_time.getLocalBounds().width / 2.0f, m_time.getLocalBounds().height / 2.0f);
}

Game::~Game()
{
	Reset();
}

void Game::Init()
{
	m_factory = new UnitFactory();

	InitVariables();

	m_clock.restart();

	SetUpCastles();
	SetUpUnits();
	SetUpResources();
	SetUpWalls();

	InitGOAP();
	InitMCTS();

	InitUI();

	m_currentScene = GameScene::GAME;
}

GameScene Game::Update()
{
	sf::Time elapsed = m_clock.restart();
	float dt = elapsed.asMilliseconds();

	if (!m_pause)
	{
		UpdateGOAP(dt);
		UpdateMCTS(dt);

		CollisionManager::GetInstance()->ApplyDamage();

		TeamBuilder(dt);
		// std::cout << CollisionManager::GetInstance()->leftSideValue << " - ";
		// std::cout << CollisionManager::GetInstance()->rightSideValue << std::endl;

		GameOver(dt);
	}

	return m_currentScene;
}

void Game::Draw(sf::RenderWindow &window)
{
	m_map.Draw(window);

	// Castles
	if (m_leftCastle->GetAlive())
	{
		m_leftCastle->Draw(window, m_debug);
	}
	if (m_rightCastle->GetAlive())
	{
		m_rightCastle->Draw(window, m_debug);
	}

	// Resources
	for (const auto& resource : m_leftResources)
	{
		if (resource->GetAlive())
		{
			resource->Draw(window, m_debug);
		}
	}
	for (const auto& resource : m_rightResources)
	{
		if (resource->GetAlive())
		{
			resource->Draw(window, m_debug);
		}
	}

	// Walls
	for (const auto& wall : m_leftWalls)
	{
		if (wall->GetAlive())
		{
			wall->Draw(window, m_debug);
		}
	}
	for (const auto& wall : m_rightWalls)
	{
		if (wall->GetAlive())
		{
			wall->Draw(window, m_debug);
		}
	}

	// Agents
	for (const auto& unit : m_leftUnits)
	{
		if (unit->GetAlive())
		{
			unit->Draw(window, m_debug);
		}
	}
	for (const auto& unit : m_rightUnits)
	{
		if (unit->GetAlive())
		{
			unit->Draw(window, m_debug);
		}
	}

	// UI
	if (m_UI)
	{
		window.draw(HUD);

		for (int i = 0; i < UIS; i++)
		{
			window.draw(m_textGOAP[i]);
			window.draw(m_textMCTS[i]);
		}

		window.draw(GOAPLine);
		window.draw(MCTSLine);

		window.draw(m_time);
		window.draw(m_gameCounter);
	}
}

void Game::Input(sf::Event Event)
{
	if (m_inputManager->KeyPressed(sf::Keyboard::BackSpace))
	{
		Logger::GetInstance()->SaveFile();
		m_currentScene = GameScene::MAINMENU;
		ResourceManager::GetInstance()->s_enter.play();
	}

	if (m_inputManager->KeyPressed(sf::Keyboard::P))
	{
		m_pause = !m_pause;
		ResourceManager::GetInstance()->s_enter.play();
	}

	if (m_inputManager->KeyPressed(sf::Keyboard::U))
	{
		m_UI = !m_UI;
		ResourceManager::GetInstance()->s_enter.play();
	}

	if (m_inputManager->KeyPressed(sf::Keyboard::M))
	{
		if (ResourceManager::GetInstance()->music.getStatus() == sf::Sound::Paused)
		{
			ResourceManager::GetInstance()->music.play();
		}
		else
		{
			ResourceManager::GetInstance()->music.pause();
		}
	}

	// Debug
	if (m_inputManager->KeyPressed(sf::Keyboard::Num1))
	{
		m_debug = DEBUG_CIRCLES;
		ResourceManager::GetInstance()->s_enter.play();
	}
	if (m_inputManager->KeyPressed(sf::Keyboard::Num2))
	{
		m_debug = DEBUG_TEXT;
		ResourceManager::GetInstance()->s_enter.play();
	}
	if (m_inputManager->KeyPressed(sf::Keyboard::Num3))
	{
		m_debug = DEBUG_NONE;
		ResourceManager::GetInstance()->s_enter.play();
	}
}

void Game::TeamBuilder(const float dt)
{
	CollisionManager::GetInstance()->CalculateValues(Team::LEFT, m_gameTime);
	CollisionManager::GetInstance()->CalculateValues(Team::RIGHT, m_gameTime);

	float leftValue = CollisionManager::GetInstance()->GetSideValue(Team::LEFT);
	float rightValue = CollisionManager::GetInstance()->GetSideValue(Team::RIGHT);

	float leftAttackValue = CollisionManager::GetInstance()->GetAttackValue(Team::LEFT);
	float rightAttackValue = CollisionManager::GetInstance()->GetAttackValue(Team::RIGHT);

	float leftDefenceValue = CollisionManager::GetInstance()->GetDefenceValue(Team::LEFT);
	float rightDefenceValue = CollisionManager::GetInstance()->GetDefenceValue(Team::RIGHT);

	int leftBuilders = CollisionManager::GetInstance()->GetUnitCount(Team::LEFT, UnitType::BUILDER);
	int leftSoldiers = CollisionManager::GetInstance()->GetUnitCount(Team::LEFT, UnitType::SOLDIER);
	int leftArchers = CollisionManager::GetInstance()->GetUnitCount(Team::LEFT, UnitType::ARCHER);
	int leftWalls = CollisionManager::GetInstance()->GetWallCount(Team::LEFT);

	int rightBuilders = CollisionManager::GetInstance()->GetUnitCount(Team::RIGHT, UnitType::BUILDER);
	int rightSoldiers = CollisionManager::GetInstance()->GetUnitCount(Team::RIGHT, UnitType::SOLDIER);
	int rightArchers = CollisionManager::GetInstance()->GetUnitCount(Team::RIGHT, UnitType::ARCHER);;
	int rightWalls = CollisionManager::GetInstance()->GetWallCount(Team::RIGHT);

	// UI
	// GOAP
	m_UI_G[0] = "GOAP : ";
	m_textGOAP[0].setString(m_UI_G[0]);
	m_UI_G[1] = "Gold = " + std::to_string(CollisionManager::GetInstance()->GetGold(Team::LEFT));
	m_textGOAP[1].setString(m_UI_G[1]);
	m_UI_G[2] = "Builders = " + std::to_string(leftBuilders);
	m_textGOAP[2].setString(m_UI_G[2]);
	m_UI_G[3] = "Soldiers = " + std::to_string(leftSoldiers);
	m_textGOAP[3].setString(m_UI_G[3]);
	m_UI_G[4] = "Archers = " + std::to_string(leftArchers);
	m_textGOAP[4].setString(m_UI_G[4]);
	m_UI_G[5] = "Wins = " + std::to_string(m_GOAPWins);
	m_textGOAP[5].setString(m_UI_G[5]);

	// MCTS
	m_UI_M[0] = "MCTS : ";
	m_textMCTS[0].setString(m_UI_M[0]);
	m_UI_M[1] = "Gold = " + std::to_string(CollisionManager::GetInstance()->GetGold(Team::RIGHT));
	m_textMCTS[1].setString(m_UI_M[1]);
	m_UI_M[2] = "Builders = " + std::to_string(rightBuilders);
	m_textMCTS[2].setString(m_UI_M[2]);
	m_UI_M[3] = "Soldiers = " + std::to_string(rightSoldiers);
	m_textMCTS[3].setString(m_UI_M[3]);
	m_UI_M[4] = "Archers = " + std::to_string(rightArchers);
	m_textMCTS[4].setString(m_UI_M[4]);
	m_UI_M[5] = "Wins = " + std::to_string(m_MCTSWins);
	m_textMCTS[5].setString(m_UI_M[5]);

	float total = CollisionManager::GetInstance()->GetSideValue(Team::LEFT) + CollisionManager::GetInstance()->GetSideValue(Team::RIGHT);
	float left = CollisionManager::GetInstance()->GetSideValue(Team::LEFT) / total;
	float right = CollisionManager::GetInstance()->GetSideValue(Team::RIGHT) / total;

	float newleft = UI_BAR * left;
	float newright = -UI_BAR * right;

	GOAPLine.setSize(sf::Vector2f(newleft, 18));

	MCTSLine.setSize(sf::Vector2f(newright, 18));

	
	// Build Left for attack
	if (leftAttackValue <= rightAttackValue * 1.05f)
	{
		if (CollisionManager::GetInstance()->GetGold(Team::LEFT) >= ARCHER_VALUE
			&& !CollisionManager::GetInstance()->CheckWorld_WallsUp(Team::LEFT)
			&& (m_leftQueueArchers + leftArchers) < MAX_ARCHERS )
		{
			m_leftQueue.push_back(BuildQueue::Offence_Archer);
			CollisionManager::GetInstance()->SpendGold(Team::LEFT, ARCHER_VALUE);
			m_leftQueueArchers++;
		}

		else if (CollisionManager::GetInstance()->GetGold(Team::LEFT) > SOLDIER_VALUE
			&& (m_leftQueueSoldiers + leftSoldiers) < MAX_SOLDIERS)
		{
			m_leftQueue.push_back(BuildQueue::Offence_Soldier);
			CollisionManager::GetInstance()->SpendGold(Team::LEFT, SOLDIER_VALUE);
			m_leftQueueSoldiers++;
		}
	}

	// Build Left for defence
	if (leftDefenceValue <= rightDefenceValue * 1.05f)
	{
		if (CollisionManager::GetInstance()->GetGold(Team::LEFT) >= WALL_VALUE
			&& !CollisionManager::GetInstance()->CheckWorld_HomeWallsUp(Team::LEFT)
			&& (m_leftQueueWalls + leftWalls) < MAX_WALLS)
		{
			CollisionManager::GetInstance()->BuildStructureRequest(Team::LEFT);
			CollisionManager::GetInstance()->SpendGold(Team::LEFT, WALL_VALUE);
			m_leftQueueWalls++;
		}

		else if (CollisionManager::GetInstance()->GetGold(Team::LEFT) >= ARCHER_VALUE
			&& !CollisionManager::GetInstance()->CheckWorld_WallsUp(Team::LEFT)
			&& (m_leftQueueArchers + leftArchers) < MAX_ARCHERS)
		{
			m_leftQueue.push_back(BuildQueue::Defence_Archer);
			CollisionManager::GetInstance()->SpendGold(Team::LEFT, ARCHER_VALUE);
			m_leftQueueArchers++;
		}

		else if (CollisionManager::GetInstance()->GetGold(Team::LEFT) > SOLDIER_VALUE
			&& (m_leftQueueSoldiers + leftSoldiers) < MAX_SOLDIERS)
		{
			m_leftQueue.push_back(BuildQueue::Defence_Soldier);
			CollisionManager::GetInstance()->SpendGold(Team::LEFT, SOLDIER_VALUE);
			m_leftQueueSoldiers++;
		}
	}

	// Build for more resources
	if (CollisionManager::GetInstance()->GetGold(Team::LEFT) < CollisionManager::GetInstance()->GetGold(Team::RIGHT) * 1.05f)
	{
		if (CollisionManager::GetInstance()->GetGold(Team::LEFT) >= BUILDER_VALUE
			&& !CollisionManager::GetInstance()->CheckWorld_ResourcesGone(Team::LEFT)
			&& (m_leftQueueBuilders + leftBuilders) < MAX_BUILDERS)
		{
			m_leftQueue.push_back(BuildQueue::Builder);
			CollisionManager::GetInstance()->SpendGold(Team::LEFT, BUILDER_VALUE);
			m_leftQueueBuilders++;
		}
	}

	BuildLeft(dt);


	/////////////////////////////////
	// Build Right for attack
	if (rightAttackValue <= leftAttackValue * 1.05f)
	{
		if (CollisionManager::GetInstance()->GetGold(Team::RIGHT) >= ARCHER_VALUE
			&& !CollisionManager::GetInstance()->CheckWorld_WallsUp(Team::RIGHT)
			&& (m_rightQueueArchers + rightArchers) < MAX_ARCHERS)
		{
			m_rightQueue.push_back(BuildQueue::Offence_Archer);
			CollisionManager::GetInstance()->SpendGold(Team::RIGHT, ARCHER_VALUE);
			m_rightQueueArchers++;
		}

		else if (CollisionManager::GetInstance()->GetGold(Team::RIGHT) > SOLDIER_VALUE
			&& (m_rightQueueSoldiers + rightSoldiers) < MAX_SOLDIERS)
		{
			m_rightQueue.push_back(BuildQueue::Offence_Soldier);
			CollisionManager::GetInstance()->SpendGold(Team::RIGHT, SOLDIER_VALUE);
			m_rightQueueSoldiers++;
		}
	}

	// Build Right for defence
	if (rightDefenceValue <= leftDefenceValue * 1.05f)
	{
		if (CollisionManager::GetInstance()->GetGold(Team::RIGHT) >= WALL_VALUE
			&& !CollisionManager::GetInstance()->CheckWorld_HomeWallsUp(Team::RIGHT)
			&& (m_rightQueueWalls + rightWalls) < MAX_WALLS)
		{
			CollisionManager::GetInstance()->BuildStructureRequest(Team::RIGHT);
			CollisionManager::GetInstance()->SpendGold(Team::RIGHT, WALL_VALUE);
			m_rightQueueWalls++;
		}

		else if (CollisionManager::GetInstance()->GetGold(Team::RIGHT) >= ARCHER_VALUE
			&& !CollisionManager::GetInstance()->CheckWorld_WallsUp(Team::RIGHT)
			&& (m_rightQueueArchers + rightArchers) < MAX_ARCHERS)
		{
			m_rightQueue.push_back(BuildQueue::Defence_Archer);
			CollisionManager::GetInstance()->SpendGold(Team::RIGHT, ARCHER_VALUE);
			m_rightQueueArchers++;
		}

		else if (CollisionManager::GetInstance()->GetGold(Team::RIGHT) > SOLDIER_VALUE
			&& (m_rightQueueSoldiers + rightSoldiers) < MAX_SOLDIERS)
		{
			m_rightQueue.push_back(BuildQueue::Defence_Soldier);
			CollisionManager::GetInstance()->SpendGold(Team::RIGHT, SOLDIER_VALUE);
			m_rightQueueSoldiers++;
		}
	}

	// Build for more resources
	if (CollisionManager::GetInstance()->GetGold(Team::RIGHT) < CollisionManager::GetInstance()->GetGold(Team::LEFT) * 1.05f)
	{
		if (CollisionManager::GetInstance()->GetGold(Team::RIGHT) >= BUILDER_VALUE
			&& !CollisionManager::GetInstance()->CheckWorld_ResourcesGone(Team::RIGHT)
			&& (m_rightQueueBuilders + rightBuilders) < MAX_BUILDERS)
		{
			m_rightQueue.push_back(BuildQueue::Builder);
			CollisionManager::GetInstance()->SpendGold(Team::RIGHT, BUILDER_VALUE);
			m_rightQueueBuilders++;
		}
	}

	BuildRight(dt);
}

void Game::BuildLeft(const float dt)
{
	if (m_leftQueue.size() > 0)
	{
		m_leftTimer += dt;

		BuildQueue nextUnit = m_leftQueue[0];

		if (nextUnit == BuildQueue::Offence_Archer)
		{
			if (m_leftTimer >= ARCHER_TIME)
			{
				m_leftTimer = 0;
				m_leftQueueArchers--;

				m_leftUnits.push_back(m_factory->CreateGOAPArcher());

				int id = m_leftUnits.size() - 1;
				m_leftUnits.back()->SetID(id);
				CollisionManager::GetInstance()->m_leftUnits[id] = m_leftUnits.back();

				goap_current_state[id] = goap_state_engage_castle;
				goap_current_goal[id] = goap_goal_kill_castle;

				m_leftUnits.back()->SetObjective(WorldState::Engage_Castle);

				GOAP(id);
			}
		}

		if (nextUnit == BuildQueue::Offence_Soldier)
		{
			if (m_leftTimer >= SOLDIER_TIME)
			{
				m_leftTimer = 0;
				m_leftQueueSoldiers--;

				m_leftUnits.push_back(m_factory->CreateGOAPSoldier());

				int id = m_leftUnits.size() - 1;
				m_leftUnits.back()->SetID(id);
				CollisionManager::GetInstance()->m_leftUnits[id] = m_leftUnits.back();

				goap_current_state[id] = goap_state_engage_castle;
				goap_current_goal[id] = goap_goal_kill_castle;

				m_leftUnits.back()->SetObjective(WorldState::Engage_Castle);

				GOAP(id);
			}
		}

		if (nextUnit == BuildQueue::Defence_Archer)
		{
			if (m_leftTimer >= ARCHER_TIME)
			{
				m_leftTimer = 0;
				m_leftQueueArchers--;

				m_leftUnits.push_back(m_factory->CreateGOAPArcher());

				int id = m_leftUnits.size() - 1;
				m_leftUnits.back()->SetID(id);
				CollisionManager::GetInstance()->m_leftUnits[id] = m_leftUnits.back();

				goap_current_state[id] = goap_state_engage_enemy;
				goap_current_goal[id] = goap_goal_kill_enemy;

				m_leftUnits.back()->SetObjective(WorldState::Engage_Enemies);

				GOAP(id);
			}
		}

		if (nextUnit == BuildQueue::Defence_Soldier)
		{
			if (m_leftTimer >= SOLDIER_TIME)
			{
				m_leftTimer = 0;
				m_leftQueueSoldiers--;

				m_leftUnits.push_back(m_factory->CreateGOAPSoldier());

				int id = m_leftUnits.size() - 1;
				m_leftUnits.back()->SetID(id);
				CollisionManager::GetInstance()->m_leftUnits[id] = m_leftUnits.back();

				goap_current_state[id] = goap_state_engage_enemy;
				goap_current_goal[id] = goap_goal_kill_enemy;

				m_leftUnits.back()->SetObjective(WorldState::Engage_Enemies);

				GOAP(id);
			}
		}

		if (nextUnit == BuildQueue::Builder)
		{
			if (m_leftTimer >= BUILDER_TIME)
			{
				m_leftTimer = 0;
				m_leftQueueBuilders--;

				m_leftUnits.push_back(m_factory->CreateGOAPBuilder());

				int id = m_leftUnits.size() - 1;
				m_leftUnits.back()->SetID(id);
				CollisionManager::GetInstance()->m_leftUnits[id] = m_leftUnits.back();

				goap_current_state[id] = goap_state_gather;
				goap_current_goal[id] = goap_goal_gather;

				m_leftUnits.back()->SetObjective(WorldState::Need_Resources);

				GOAP(id);
			}
		}

		if (m_leftTimer == 0)
		{
			m_leftQueue.erase(m_leftQueue.begin());
		}
	}

	else if (m_leftQueue.size() == 0)
	{
		m_leftTimer = 0;
	}
}

void Game::BuildRight(const float dt)
{
	if (m_rightQueue.size() > 0)
	{
		m_rightTimer += dt;

		BuildQueue nextUnit = m_rightQueue[0];

		if (nextUnit == BuildQueue::Offence_Archer)
		{
			if (m_rightTimer >= ARCHER_TIME)
			{
				m_rightTimer = 0;
				m_rightQueueArchers--;

				m_rightUnits.push_back(m_factory->CreateMCTSArcher());

				int id = m_rightUnits.size() - 1;
				m_rightUnits.back()->SetID(id);
				CollisionManager::GetInstance()->m_rightUnits[id] = m_rightUnits.back();

				MCTS_State[id] = MCTSWorldState(ActionState::Attack_Castle);
				MCTS_State[id].SetID(id);
				MCTS_Action[id] = *m_rightUnits.back()->GetMCTSAction();

				m_rightUnits.back()->SetObjective(WorldState::Engage_Castle);

				MCTS(id);
			}
		}

		if (nextUnit == BuildQueue::Offence_Soldier)
		{
			if (m_rightTimer >= SOLDIER_TIME)
			{
				m_rightTimer = 0;
				m_rightQueueSoldiers--;

				m_rightUnits.push_back(m_factory->CreateMCTSSoldier());

				int id = m_rightUnits.size() - 1;
				m_rightUnits.back()->SetID(id);
				CollisionManager::GetInstance()->m_rightUnits[id] = m_rightUnits.back();

				MCTS_State[id] = MCTSWorldState(ActionState::Attack_Castle);
				MCTS_State[id].SetID(id);
				MCTS_Action[id] = *m_rightUnits.back()->GetMCTSAction();

				m_rightUnits.back()->SetObjective(WorldState::Engage_Castle);

				MCTS(id);
			}
		}

		if (nextUnit == BuildQueue::Defence_Archer)
		{
			if (m_rightTimer >= ARCHER_TIME)
			{
				m_rightTimer = 0; 
				m_rightQueueArchers--;

				m_rightUnits.push_back(m_factory->CreateMCTSArcher());

				int id = m_rightUnits.size() - 1;
				m_rightUnits.back()->SetID(id);
				CollisionManager::GetInstance()->m_rightUnits[id] = m_rightUnits.back();

				MCTS_State[id] = MCTSWorldState(ActionState::Attack_Enemy);
				MCTS_State[id].SetID(id);
				MCTS_Action[id] = *m_rightUnits.back()->GetMCTSAction();

				m_rightUnits.back()->SetObjective(WorldState::Engage_Enemies);

				MCTS(id);
			}
		}

		if (nextUnit == BuildQueue::Defence_Soldier)
		{
			if (m_rightTimer >= SOLDIER_TIME)
			{
				m_rightTimer = 0;
				m_rightQueueSoldiers--;

				m_rightUnits.push_back(m_factory->CreateMCTSSoldier());

				int id = m_rightUnits.size() - 1;
				m_rightUnits.back()->SetID(id);
				CollisionManager::GetInstance()->m_rightUnits[id] = m_rightUnits.back();

				MCTS_State[id] = MCTSWorldState(ActionState::Attack_Enemy);
				MCTS_State[id].SetID(id);
				MCTS_Action[id] = *m_rightUnits.back()->GetMCTSAction();

				m_rightUnits.back()->SetObjective(WorldState::Engage_Enemies);

				MCTS(id);
			}
		}

		if (nextUnit == BuildQueue::Builder)
		{
			if (m_rightTimer >= BUILDER_TIME)
			{
				m_rightTimer = 0;
				m_rightQueueBuilders--;

				m_rightUnits.push_back(m_factory->CreateMCTSBuilder());

				int id = m_rightUnits.size() - 1;
				m_rightUnits.back()->SetID(id);
				CollisionManager::GetInstance()->m_rightUnits[id] = m_rightUnits.back();

				MCTS_State[id] = MCTSWorldState(ActionState::Idle_Builder);
				MCTS_State[id].SetID(id);
				MCTS_Action[id] = *m_rightUnits.back()->GetMCTSAction();

				m_rightUnits.back()->SetObjective(WorldState::Need_Resources);

				MCTS(id);
			}
		}

		if (m_rightTimer == 0)
		{
			m_rightQueue.erase(m_rightQueue.begin());
		}
	}

	else if (m_rightQueue.size() == 0)
	{
		m_rightTimer = 0;
	}
}

void Game::WorldEvaluation(Unit* unit)
{
	std::map<int, WorldState> priority;
	std::map<WorldState, int> states;

	WorldState current_state = WorldState::Wait;

	if (unit->GetType() != UnitType::BUILDER)
	{
		// Offense - is enemy castle open to attack?
		if (CollisionManager::GetInstance()->CheckWorld_EngageCastle(unit->GetSide())) 
		{
			states[WorldState::Engage_Castle] = CollisionManager::GetInstance()->GetWeight(WorldState::Engage_Castle);
		}

		if (CollisionManager::GetInstance()->CheckWorld_EngageEnemy(unit->GetSide()))
		{
			states[WorldState::Engage_Enemies] = CollisionManager::GetInstance()->GetWeight(WorldState::Engage_Enemies);
		}

		if (CollisionManager::GetInstance()->CheckWorld_EngageStructure(unit->GetID(), unit->GetSide()))
		{
			states[WorldState::Engage_Structure] = CollisionManager::GetInstance()->GetWeight(WorldState::Engage_Structure);
		}
	}

	// Defense
	if (CollisionManager::GetInstance()->CheckWorld_DefendCastle(unit->GetSide()))
	{
		states[WorldState::Defend_Castle] = CollisionManager::GetInstance()->GetWeight(WorldState::Defend_Castle);
	}

	if (unit->GetType() == UnitType::BUILDER)
	{
		// Other
		if (CollisionManager::GetInstance()->CheckWorld_NeedResources(unit->GetSide()))
		{
			states[WorldState::Need_Resources] = CollisionManager::GetInstance()->GetWeight(WorldState::Need_Resources);
		}
	}


	// Find Best Option
	if (!states.empty())
	{
		for (auto &state : states)
		{
			priority[state.second] = state.first;
		}

		// Get most urgent state
		current_state = (*priority.begin()).second;
	}

	// Overwrite
	if (CollisionManager::GetInstance()->CheckWorld_EnemiesDead(unit->GetSide())) // are all enemies dead?
	{
		current_state = WorldState::Engage_Castle;
	}

	if (CollisionManager::GetInstance()->CheckWorld_ResourcesGone(unit->GetSide())) // are all resources gone?
	{
		current_state = WorldState::Engage_Castle;
	}

	if (CollisionManager::GetInstance()->DetectedEnemy(unit->GetID(), unit->GetSide()))
	{
		if (CollisionManager::GetInstance()->CheckWorld_EngageEnemy(unit->GetSide()))
		{
			current_state = WorldState::Engage_Enemies;
		}

		else if (CollisionManager::GetInstance()->CheckWorld_DefendCastle(unit->GetSide()))
		{
			current_state = WorldState::Defend_Castle;
		}
	}

	if (unit->GetType() == UnitType::BUILDER)
	{
		if (CollisionManager::GetInstance()->BuildStructureRequested(unit->GetSide()))
		{
			current_state = WorldState::Build_Structures;
		}

		else if (CollisionManager::GetInstance()->CheckWorld_NeedResources(unit->GetSide())) // are all enemies dead?
		{
			current_state = WorldState::Need_Resources;
		}
	}

	if (CollisionManager::GetInstance()->DetectedWall(unit->GetID(), unit->GetSide()))
	{
		if (CollisionManager::GetInstance()->CheckWorld_EngageStructure(unit->GetID(), unit->GetSide()))
		{
			current_state = WorldState::Engage_Structure;
		}

		else if (CollisionManager::GetInstance()->CheckWorld_TouchingWall(unit->GetID(), unit->GetSide()))
		{
			current_state = WorldState::Engage_Structure;
		}
	}

	// Operate based on state
	GoalAllocation(unit, current_state);
}

void Game::GoalAllocation(Unit* unit, const WorldState& currentState)
{
	unit->SetObjective( currentState);

	if (currentState == WorldState::Engage_Castle)
	{
		if (unit->GetSide() ==  Team::LEFT)
		{
			goap_current_state[unit->GetID()] = goap_state_engage_castle;
			goap_current_goal[unit->GetID()] = goap_goal_kill_castle;
		}

		else
		{
			MCTS_Action[unit->GetID()].AddChild(ActionState::Move_To_Castle);
		}
	}

	if (currentState == WorldState::Engage_Enemies)
	{
		if (unit->GetSide() ==  Team::LEFT)
		{
			goap_current_state[unit->GetID()] = goap_state_engage_enemy;
			goap_current_goal[unit->GetID()] = goap_goal_kill_enemy;
		}

		else
		{
			MCTS_Action[unit->GetID()].AddChild(ActionState::Move_To_Enemy);
		}
	}

	if (currentState == WorldState::Engage_Structure)
	{
		if (unit->GetSide() ==  Team::LEFT)
		{
			goap_current_state[unit->GetID()] = goap_state_engage_wall;
			goap_current_goal[unit->GetID()] = goap_goal_kill_wall;
		}

		else
		{
			MCTS_Action[unit->GetID()].AddChild(ActionState::Move_To_Structure);
		}
	}

	if (currentState == WorldState::Defend_Castle)
	{
		if (unit->GetSide() ==  Team::LEFT)
		{
			goap_current_state[unit->GetID()] = goap_state_defend;
			goap_current_goal[unit->GetID()] = goap_goal_defend;
		}

		else
		{
			MCTS_Action[unit->GetID()].AddChild(ActionState::Move_To_Base);
		}
	}

	if (currentState == WorldState::Need_Resources)
	{
		if (unit->GetSide() ==  Team::LEFT)
		{
			goap_current_state[unit->GetID()] = goap_state_gather;
			goap_current_goal[unit->GetID()] = goap_goal_gather;
		}

		else
		{
			MCTS_Action[unit->GetID()].AddChild(ActionState::Move_To_Resource);
		}
	}

	if (currentState == WorldState::Build_Structures)
	{
		if (unit->GetSide() ==  Team::LEFT)
		{
			goap_current_state[unit->GetID()] = goap_state_build;
			goap_current_goal[unit->GetID()] = goap_goal_build;
			m_leftQueueWalls--;
		}

		else
		{
			MCTS_Action[unit->GetID()].AddChild(ActionState::Move_To_Site);
			m_rightQueueWalls--;
		}
	}

	if (currentState == WorldState::Wait)
	{
		if (unit->GetSide() ==  Team::LEFT)
		{
			if (unit->GetType() == UnitType::BUILDER)
			{
				goap_current_state[unit->GetID()] = goap_state_gather;
				goap_current_goal[unit->GetID()] = goap_goal_gather;
			}
			else
			{
				goap_current_state[unit->GetID()] = goap_state_engage_castle;
				goap_current_goal[unit->GetID()] = goap_goal_kill_castle;
			}
		}
		else
		{
			if (unit->GetType() == UnitType::BUILDER)
			{
				MCTS_Action[unit->GetID()].AddChild(ActionState::Idle_Builder);
			}
			else
			{
				MCTS_Action[unit->GetID()].AddChild(ActionState::Idle);
			}
		}
	}

	if (unit->GetSide() ==  Team::LEFT)
	{
		GOAP(unit->GetID()); // send in i from update?
	}
	else
	{
		MCTS_State[unit->GetID()] = MCTSWorldState(MCTS_Action[unit->GetID()]);
		MCTS_State[unit->GetID()].SetID(unit->GetID());
	}
}

void Game::UpdateGOAP(const float dt)
{
	for (int i = 0; i < m_leftUnits.size(); i++)
	{
		int id = m_leftUnits[i]->GetID();

		if (GOAP_Plan[m_leftUnits[i]].size() > 0 && m_leftUnits[i]->GetAlive())
		{
			int action = m_leftUnits[i]->Update(GOAP_Plan[m_leftUnits[i]].back().GetAction(), dt);

			if (action == SUCCESS)
			{
				std::cout << "GOAP [" << id << "] - Succeeded - " << GOAP_Plan[m_leftUnits[i]].back().GetName() << "\n";

				//for (const auto& effect : GOAP_Plan[m_leftUnits[i]].back().GetEffects())
				//{
				//	goap_current_state[id]->SetVariable(effect.first, effect.second); // update state
				//}

				GOAP_Plan[m_leftUnits[i]].pop_back();

				if (GOAP_Plan[m_leftUnits[i]].size() == 0)
				{
					WorldEvaluation(m_leftUnits[i]);
				}
			}

			else if (action == FAIL)
			{
				std::cout << "GOAP [" << id << "] - Failed - " << GOAP_Plan[m_leftUnits[i]].back().GetName() << "\n";
				WorldEvaluation(m_leftUnits[i]);
			}
		}
	}
}

void Game::UpdateMCTS(const float dt)
{
	for (int i = 0; i < m_rightUnits.size(); i++)
	{
		int id = m_rightUnits[i]->GetID();

		if (m_rightUnits[i]->GetAlive())
		{
			int action = m_rightUnits[i]->Update(MCTS_Action[id].GetAction(), dt);

			if (action == SUCCESS)
			{
				std::cout << "MCTS [" << id << "] - Succeeded - " << MCTS_Action[id].GetName() << "\n";

				if (MCTS_Action[id].GetAction() == ActionState::Castle_Dead || MCTS_Action[id].GetAction() == ActionState::Enemy_Dead || 
					MCTS_Action[id].GetAction() == ActionState::Structure_Dead || MCTS_Action[id].GetAction() == ActionState::Base_Reached || 
					MCTS_Action[id].GetAction() == ActionState::Resources_Collected || MCTS_Action[id].GetAction() == ActionState::Structure_Built || 
					MCTS_Action[id].GetAction() == ActionState::Idle || MCTS_Action[id].GetAction() == ActionState::Idle_Builder)
				{
					WorldEvaluation(m_rightUnits[i]);
				}


				MCTS(i);
			}

			else if (action == FAIL)
			{
				std::cout << "MCTS [" << id << "] - Failed - " << MCTS_Action[id].GetName() << "\n";
				WorldEvaluation(m_rightUnits[i]);
				MCTS(i);
			}
		}
	}
}

void Game::InitGOAP()
{
	GOAPSearch = new GOAPPlanner();

	// Start states
	goap_state_engage_castle = new GOAPWorldState();
	goap_state_engage_castle->SetVariable(ActionState::Move_To_Castle, true);
	goap_state_engage_castle->SetVariable(ActionState::Attack_Castle, false);
	goap_state_engage_castle->SetVariable(ActionState::Castle_Dead, false);

	goap_state_engage_enemy = new GOAPWorldState();
	goap_state_engage_enemy->SetVariable(ActionState::Move_To_Enemy, true);
	goap_state_engage_enemy->SetVariable(ActionState::Attack_Enemy, false);
	goap_state_engage_enemy->SetVariable(ActionState::Enemy_Dead, false);

	goap_state_engage_wall = new GOAPWorldState();
	goap_state_engage_wall->SetVariable(ActionState::Move_To_Structure, true);
	goap_state_engage_wall->SetVariable(ActionState::Attack_Structure, false);
	goap_state_engage_wall->SetVariable(ActionState::Structure_Dead, false);

	goap_state_defend = new GOAPWorldState();
	goap_state_defend->SetVariable(ActionState::Move_To_Base, true);
	goap_state_defend->SetVariable(ActionState::Base_Reached, false);

	goap_state_gather = new GOAPWorldState();
	goap_state_gather->SetVariable(ActionState::Move_To_Resource, true);
	goap_state_gather->SetVariable(ActionState::Gather_Resources, false);
	goap_state_gather->SetVariable(ActionState::Resources_Collected, false);

	goap_state_build = new GOAPWorldState();
	goap_state_build->SetVariable(ActionState::Move_To_Site, true);
	goap_state_build->SetVariable(ActionState::Build_Structure, false);
	goap_state_build->SetVariable(ActionState::Structure_Built, false);

	// Goals
	goap_goal_kill_castle = new GOAPWorldState();
	goap_goal_kill_castle->SetVariable(ActionState::Castle_Dead, true);

	goap_goal_kill_enemy = new GOAPWorldState();
	goap_goal_kill_enemy->SetVariable(ActionState::Enemy_Dead, true);

	goap_goal_kill_wall = new GOAPWorldState();
	goap_goal_kill_wall->SetVariable(ActionState::Structure_Dead, true);

	goap_goal_defend = new GOAPWorldState();
	goap_goal_defend->SetVariable(ActionState::Base_Reached, true);

	goap_goal_gather = new GOAPWorldState();
	goap_goal_gather->SetVariable(ActionState::Resources_Collected, true);

	goap_goal_build = new GOAPWorldState();
	goap_goal_build->SetVariable(ActionState::Structure_Built, true);

	for (int i = 0; i < m_leftUnits.size(); i++)
	{
		int id = m_leftUnits[i]->GetID();

		if (m_leftUnits[i]->GetType() ==  UnitType::BUILDER)
		{
			goap_current_state[id] = goap_state_gather;
			goap_current_goal[id] = goap_goal_gather;

			m_leftUnits[i]->SetObjective(WorldState::Need_Resources);
		}
		else
		{
			goap_current_state[id] = goap_state_engage_castle;
			goap_current_goal[id] = goap_goal_kill_castle;

			m_leftUnits[i]->SetObjective(WorldState::Engage_Castle);
		}

		// Run GOAP
		GOAP(i);
	}
}

void Game::InitMCTS()
{
	MCTSSearch = new MCTSPlanner();

	for (int i = 0; i < m_rightUnits.size(); i++)
	{
		int id = m_rightUnits[i]->GetID();

		if (m_rightUnits[i]->GetType() ==  UnitType::BUILDER)
		{
			MCTS_State[id] = MCTSWorldState(ActionState::Idle_Builder);
		}
		else
		{
			MCTS_State[id] = MCTSWorldState(ActionState::Idle);
		}

		MCTS_State[id].SetID(id);
		MCTS_Action[id] = *m_rightUnits[i]->GetMCTSAction();

		// Run MCTS
		MCTS(i);
	}
}

void Game::GOAP(int i)
{
	int id = m_leftUnits[i]->GetID();

	try
	{
		GOAP_Plan[m_leftUnits[i]] = GOAPSearch->Plan(*goap_current_state[id], *goap_current_goal[id], *m_leftUnits[i]->GetGOAPActions());
		
		std::cout << "GOAP Path = \n";
		for (std::vector<GOAPAction>::reverse_iterator rit = GOAP_Plan[m_leftUnits[i]].rbegin(); rit != GOAP_Plan[m_leftUnits[i]].rend(); ++rit)
		{
			std::cout << rit->GetName() << "\n";
		}
		std::cout << "\n";
	}
	catch (const std::exception&)
	{
		std::cout << "Sorry, could not find a path!\n";
	}
}

void Game::MCTS(int i)
{
	int id = m_rightUnits[i]->GetID();

	try
	{
		//std::cout << "MCTS Path = \n";

		MCTS_Action[id] = MCTSSearch->Plan(MCTS_State[id]);
		MCTS_State[id] = MCTSWorldState(MCTS_Action[id]); // mcts_initial_state = next_action.next_state;
		MCTS_State[id].SetID(id);


		if (MCTS_Action[id].GetAction() == ActionState::Attack_Castle)
		{
			m_rightUnits[i]->SetObjective(WorldState::Engage_Castle);
		}
		else if (MCTS_Action[id].GetAction() == ActionState::Attack_Enemy)
		{
			m_rightUnits[i]->SetObjective(WorldState::Engage_Enemies);
		}
		else if (MCTS_Action[id].GetAction() == ActionState::Gather_Resources)
		{
			m_rightUnits[i]->SetObjective(WorldState::Need_Resources);
		}
		else
		{
			m_rightUnits[i]->SetObjective(WorldState::Wait);
		}


		std::cout << "MCTS [" << id << "] - " << MCTS_Action[id].GetName() << "\n";
	}
	catch (const std::exception&)
	{
		std::cout << "Sorry, could not find a path!\n";
	}
}

void Game::SetUpCastles()
{
	// Set up castles
	m_leftCastle = new Structure(Team::LEFT, StructureType::CASTLE);
	m_rightCastle = new Structure(Team::RIGHT, StructureType::CASTLE);

	CollisionManager::GetInstance()->m_leftCastle = m_leftCastle;
	CollisionManager::GetInstance()->m_rightCastle = m_rightCastle;
}

void Game::SetUpUnits()
{
	m_leftUnits.push_back(m_factory->CreateGOAPBuilder());
	int l_id = m_leftUnits.size() - 1;
	m_leftUnits.back()->SetID(l_id);

	CollisionManager::GetInstance()->m_leftUnits[l_id] = m_leftUnits.back();

	m_rightUnits.push_back(m_factory->CreateMCTSBuilder());
	int r_id = m_rightUnits.size() - 1;
	m_rightUnits.back()->SetID(r_id);

	CollisionManager::GetInstance()->m_rightUnits[r_id] = m_rightUnits.back();
}

void Game::SetUpWalls()
{
	// Left
	m_leftWalls.push_back(new Structure(Team::LEFT, StructureType::WALL, LeftBackUpper));
	m_leftWalls.push_back(new Structure(Team::LEFT, StructureType::WALL, LeftBackMiddle));
	m_leftWalls.push_back(new Structure(Team::LEFT, StructureType::WALL, LeftBackLower));

	m_leftWalls.push_back(new Structure(Team::LEFT, StructureType::WALL, LeftMidUpper));
	m_leftWalls.push_back(new Structure(Team::LEFT, StructureType::WALL, LeftMidMiddle));
	m_leftWalls.push_back(new Structure(Team::LEFT, StructureType::WALL, LeftMidLower));

	m_leftWalls.push_back(new Structure(Team::LEFT, StructureType::WALL, LeftFrontUpper));
	m_leftWalls.push_back(new Structure(Team::LEFT, StructureType::WALL, LeftFrontMiddle));
	m_leftWalls.push_back(new Structure(Team::LEFT, StructureType::WALL, LeftFrontLower));

	// Right
	m_rightWalls.push_back(new Structure(Team::RIGHT, StructureType::WALL, RightBackUpper));
	m_rightWalls.push_back(new Structure(Team::RIGHT, StructureType::WALL, RightBackMiddle));
	m_rightWalls.push_back(new Structure(Team::RIGHT, StructureType::WALL, RightBackLower));

	m_rightWalls.push_back(new Structure(Team::RIGHT, StructureType::WALL, RightMidUpper));
	m_rightWalls.push_back(new Structure(Team::RIGHT, StructureType::WALL, RightMidMiddle));
	m_rightWalls.push_back(new Structure(Team::RIGHT, StructureType::WALL, RightMidLower));

	m_rightWalls.push_back(new Structure(Team::RIGHT, StructureType::WALL, RightFrontUpper));
	m_rightWalls.push_back(new Structure(Team::RIGHT, StructureType::WALL, RightFrontMiddle));
	m_rightWalls.push_back(new Structure(Team::RIGHT, StructureType::WALL, RightFrontLower));

	//m_rightWalls[8]->SetAlive(true);
	//m_rightWalls[7]->SetAlive(true);
	//m_rightWalls[6]->SetAlive(true);
	//m_rightWalls[5]->SetAlive(true);
	//m_rightWalls[4]->SetAlive(true);
	//m_rightWalls[3]->SetAlive(true);
	//m_rightWalls[2]->SetAlive(true);
	//m_rightWalls[1]->SetAlive(true);
	//m_rightWalls[0]->SetAlive(true);


	//m_leftWalls[8]->SetAlive(true);
	//m_leftWalls[7]->SetAlive(true);
	//m_leftWalls[6]->SetAlive(true);
	//m_leftWalls[5]->SetAlive(true);
	//m_leftWalls[4]->SetAlive(true);
	//m_leftWalls[3]->SetAlive(true);
	//m_leftWalls[2]->SetAlive(true);
	//m_leftWalls[1]->SetAlive(true);
	//m_leftWalls[0]->SetAlive(true);

	// Add to Collision Manager
	CollisionManager::GetInstance()->m_leftWalls = m_leftWalls;
	CollisionManager::GetInstance()->m_rightWalls = m_rightWalls;
}

void Game::SetUpResources()
{
	int offset = 15;
	int startX = 15;
	int startY = 15;
	int row = 5;
	int col = 20;

	for (int x = 0; x < col; x++)
	{
		for (int y = 0; y < row; y++)
		{
			// Left Resources
			m_leftResources.push_back(new Structure(Team::LEFT, StructureType::RESOURCE,
				sf::Vector2f(startX + (offset * x), startY + (offset * y))));

			m_leftResources.push_back(new Structure(Team::LEFT, StructureType::RESOURCE,
				sf::Vector2f(startX + (offset * x), SCREEN_HEIGHT - startY - (offset * y))));

			// Right Resources
			m_rightResources.push_back(new Structure(Team::RIGHT, StructureType::RESOURCE,
				sf::Vector2f(SCREEN_WIDTH - startX - (offset * x), startY + (offset * y))));

			m_rightResources.push_back(new Structure(Team::RIGHT, StructureType::RESOURCE,
				sf::Vector2f(SCREEN_WIDTH - startX - (offset * x), SCREEN_HEIGHT - startY - (offset * y))));
		}
	}

	CollisionManager::GetInstance()->m_leftResources = m_leftResources;
	CollisionManager::GetInstance()->m_rightResources = m_rightResources;

}

void Game::GameOver(const float dt)
{
	m_gameTime += dt;

	m_tic += dt;
	if (m_tic >= MILIS)
	{
		m_tic -= MILIS;
		m_seconds++;
		if (m_seconds >= MINUTE)
		{
			m_seconds -= MINUTE;
			m_minutes++;
		}
	}

	std::string gameTime = std::to_string(m_minutes) + " : " + std::to_string(m_seconds);
	m_time.setString(gameTime);

	if (m_gameTime >= GAME_LENGTH)
	{
		m_winner = "Draw";
		Logger::GetInstance()->LogGame(Log());
		Reset();
		Init();
	}

	else if (!m_leftCastle->GetAlive())
	{
		m_winner = "MCTS";
		m_MCTSWins++;
		m_winnerWins = m_MCTSWins;
		Logger::GetInstance()->LogGame(Log());
		Reset();
		Init();
	}

	else if (!m_rightCastle->GetAlive())
	{
		m_winner = "GOAP";
		m_GOAPWins++;
		m_winnerWins = m_GOAPWins;
		Logger::GetInstance()->LogGame(Log());
		Reset();
		Init();
	}


}

std::string Game::Log()
{
	std::string game =
		std::to_string(m_gameNumber)
		+ "," + m_winner
		+ "," + std::to_string(m_winnerWins)
		+ "," + std::to_string(m_gameTime / MILIS)

		+ "," + std::to_string(CollisionManager::GetInstance()->m_leftSideValueHighest)
		+ "," + std::to_string(CollisionManager::GetInstance()->m_leftAttackValueHighest)
		+ "," + std::to_string(CollisionManager::GetInstance()->m_leftDefenceValueHighest)
		+ "," + std::to_string(CollisionManager::GetInstance()->m_leftSideValueHighestSeconds)

		+ "," + std::to_string(CollisionManager::GetInstance()->m_leftBuildersTotal)
		+ "," + std::to_string(CollisionManager::GetInstance()->m_leftSoldiersTotal)
		+ "," + std::to_string(CollisionManager::GetInstance()->m_leftArchersTotal)
		+ "," + std::to_string(CollisionManager::GetInstance()->m_leftWallsTotal)

		+ "," + std::to_string(CollisionManager::GetInstance()->m_rightSideValueHighest)
		+ "," + std::to_string(CollisionManager::GetInstance()->m_rightAttackValueHighest)
		+ "," + std::to_string(CollisionManager::GetInstance()->m_rightDefenceValueHighest)
		+ "," + std::to_string(CollisionManager::GetInstance()->m_rightSideValueHighestSeconds)

		+ "," + std::to_string(CollisionManager::GetInstance()->m_rightBuildersTotal)
		+ "," + std::to_string(CollisionManager::GetInstance()->m_rightSoldiersTotal)
		+ "," + std::to_string(CollisionManager::GetInstance()->m_rightArchersTotal)
		+ "," + std::to_string(CollisionManager::GetInstance()->m_rightWallsTotal)

		+ "," + Logger::GetInstance()->GetTimeStamp(false);

	return game;
}

void Game::Reset()
{
	//// OBJECTS

	delete m_leftCastle;
	delete m_rightCastle;

	delete m_factory;

	// Units
	for (const auto& unit : m_leftUnits)
	{
		delete unit;
	}
	m_leftUnits.clear();

	for (const auto& unit : m_rightUnits)
	{
		delete unit;
	}
	m_rightUnits.clear();

	// Walls
	for (const auto& wall : m_leftWalls)
	{
		delete wall;
	}
	m_leftWalls.clear();

	for (const auto& wall : m_rightWalls)
	{
		delete wall;
	}
	m_rightWalls.clear();

	// Resources
	for (const auto& resource : m_leftResources)
	{
		delete resource;
	}
	m_leftResources.clear();

	for (const auto& resource : m_rightResources)
	{
		delete resource;
	}
	m_rightResources.clear();

	// Queue
	m_leftQueue.clear();
	m_rightQueue.clear();

	//// AI
	delete goap_state_engage_castle;
	delete goap_state_engage_enemy;
	delete goap_state_engage_wall;
	delete goap_state_defend;
	delete goap_state_gather;
	delete goap_state_build;

	goap_current_state.clear();

	delete goap_goal_kill_castle;
	delete goap_goal_kill_enemy;
	delete goap_goal_kill_wall;
	delete goap_goal_defend;
	delete goap_goal_gather;
	delete goap_goal_build;

	goap_current_goal.clear();

	GOAP_Plan.clear();
	delete GOAPSearch;

	MCTS_Action.clear();
	MCTS_State.clear();
	delete MCTSSearch;

	CollisionManager::GetInstance()->Reset();
}

void Game::InitVariables()
{
	// VARIABLES
	m_debug = DEBUG_CIRCLES;

	m_minutes = 0;
	m_seconds = 0;
	m_tic = 0;

	m_pause = false;
	m_UI = true;
	m_gameTime = 0;
	m_winnerWins = 0;

	m_leftTimer = 0;
	m_rightTimer = 0;

	m_leftQueueBuilders = 0;
	m_leftQueueSoldiers = 0;
	m_leftQueueArchers = 0;

	m_rightQueueBuilders = 0;
	m_rightQueueSoldiers = 0;
	m_rightQueueArchers = 0;

	m_gameNumber++;
	m_gameOver = false;
	m_winner = "";

	m_gameCounter.setString(std::to_string(m_gameNumber));
}

void Game::InitUI()
{
	GOAPLine.setSize(sf::Vector2f(1, 15));
	GOAPLine.setOrigin(sf::Vector2f(0, 9));
	GOAPLine.setPosition(SCREEN_WIDTH * 0.42f, SCREEN_HEIGHT * 0.035f);
	GOAPLine.setFillColor(sf::Color::Blue);
	GOAPLine.setOutlineColor(Grey);
	GOAPLine.setOutlineThickness(2.0f);

	MCTSLine.setSize(sf::Vector2f(1, 15));
	MCTSLine.setOrigin(sf::Vector2f(0, 9));
	MCTSLine.setPosition(SCREEN_WIDTH * 0.58f, SCREEN_HEIGHT * 0.035f);
	MCTSLine.setFillColor(sf::Color::Red);
	MCTSLine.setOutlineColor(Grey);
	MCTSLine.setOutlineThickness(2.0f);

	HUD.setSize(sf::Vector2f(720, 180));
	HUD.setOrigin(sf::Vector2f(360, 90));
	HUD.setPosition(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.1f);
	HUD.setFillColor(sf::Color(0, 0, 0, 155));
	HUD.setOutlineColor(sf::Color::Black);
	HUD.setOutlineThickness(4.0f);
}
