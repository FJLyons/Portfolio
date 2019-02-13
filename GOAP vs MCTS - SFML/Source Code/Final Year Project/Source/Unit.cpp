#include "Unit.h"

Unit::Unit(const Team& side, const UnitType& type) : m_side(side), m_id(0), m_type(type)
{
	Init();

	int lines = 2;
	for (int i = 0; i < lines; i++)
	{
		m_text[i].setString("");
		m_text[i].setFont(ResourceManager::GetInstance()->FONT);
		m_text[i].setPosition(m_position);
		m_text[i].setCharacterSize(18);
		if (m_side == Team::LEFT)
		{
			m_text[i].setOutlineColor(sf::Color::Blue);
		}
		else if (m_side == Team::RIGHT)
		{
			m_text[i].setOutlineColor(sf::Color::Red);
		}
		m_text[i].setOutlineThickness(1);
	}
}


Unit::~Unit()
{
	if (m_GOAPActions != nullptr)
	{
		for (const auto& action : *m_GOAPActions)
		{
			delete action;
		}
		m_GOAPActions->clear();
		delete m_GOAPActions;
	}

	delete m_MCTSAction;

	delete init_action;
}

void Unit::Init()
{
	m_objective = WorldState::Wait;

	m_radius = UNIT_BODY_SIZE;
	BODY = sf::CircleShape();

	if (m_side == Team::LEFT)
	{
		m_position = sf::Vector2f(SCREEN_WIDTH * 0.1f, rand() % UNIT_POS_LOWER + UNIT_POS_UPPER);
		//BODY.setFillColor(sf::Color::Blue);
		CreateGOAPActions();
	}
	else if (m_side == Team::RIGHT)
	{
		m_position = sf::Vector2f(SCREEN_WIDTH * 0.9f, rand() % UNIT_POS_LOWER + UNIT_POS_UPPER);
		//BODY.setFillColor(sf::Color::Red);
		CreateMCTSActions();
	}

	SetTarget(Target::NO_T);
	alive = true;
	m_attackTimer = 0;

	if (m_type == UnitType::SOLDIER) { InitSoldier(); }
	else if (m_type == UnitType::ARCHER) { InitArcher(); }
	else if (m_type == UnitType::BUILDER) { InitBuilder(); }

	InitCircles();
}

int Unit::Update(const ActionState& currentState, const float dt)
{
	BODY.setPosition(m_position);
	FOV.setPosition(m_position);
	RANGE.setPosition(m_position);

	m_text[0].setString(std::to_string((int)m_valueDefense));
	m_text[0].setOrigin(m_text[0].getLocalBounds().width / 2.0f, m_text[0].getLocalBounds().height / 2.0f);
	m_text[0].setPosition(m_position - sf::Vector2f(0, m_radius * 2));

	m_text[1].setString(ResourceManager::GetInstance()->stateNames[currentState]);
	m_text[1].setOrigin(m_text[1].getLocalBounds().width / 2.0f, m_text[1].getLocalBounds().height / 2.0f);
	m_text[1].setPosition(m_position + sf::Vector2f(0, m_radius * 2));

	m_working = 0;

	switch (currentState)
	{
	case ActionState::Idle:						m_working = IsIdle();					break;

	case ActionState::Move_To_Castle:			m_working = MoveToCastle();				break;
	case ActionState::Attack_Castle:			m_working = AttackCastle(dt);			break;
	case ActionState::Castle_Dead:				m_working = Done();						break;

	case ActionState::Move_To_Enemy:			m_working = MoveToEnemy();				break;
	case ActionState::Attack_Enemy:				m_working = AttackEnemy(dt);			break;
	case ActionState::Enemy_Dead:				m_working = Done();						break;

	case ActionState::Move_To_Structure:		m_working = MoveToStructure();			break;
	case ActionState::Attack_Structure:			m_working = AttackStructure(dt);		break;
	case ActionState::Structure_Dead:			m_working = Done();						break;

	case ActionState::Move_To_Base:				m_working = MoveToBase();				break;
	case ActionState::Base_Reached:				m_working = Done();						break;
		
	case ActionState::Idle_Builder:				m_working = IsIdle();					break;

	case ActionState::Move_To_Resource:			m_working = MoveToResource();			break;
	case ActionState::Gather_Resources:			m_working = GatherResources(dt);		break;
	case ActionState::Resources_Collected:		m_working = Done();						break;

	case ActionState::Move_To_Site:				m_working = MoveToSite();				break;
	case ActionState::Build_Structure:			m_working = BuildStructure(dt);			break;
	case ActionState::Structure_Built:			m_working = Done();						break;
	}

	return m_working;
}

void Unit::Draw(sf::RenderWindow& window, const int debugLevel)
{
	window.draw(BODY);	

	if (debugLevel > DEBUG_NONE)
	{
		window.draw(m_text[0]);
		window.draw(m_text[1]);

		if (debugLevel > DEBUG_TEXT)
		{
			window.draw(FOV);
			window.draw(RANGE);
		}
	}
}

void Unit::InitCircles()
{
	BODY.setRadius(m_radius);
	BODY.setOrigin(sf::Vector2f(m_radius, m_radius));
	BODY.setPosition(m_position);

	if (m_type == UnitType::SOLDIER)
	{ 
		//BODY.setOutlineColor(sf::Color::White); 
		if (m_side == Team::LEFT)
		{
			BODY.setTexture(&ResourceManager::GetInstance()->t_blue_soldier);
		}
		else if (m_side == Team::RIGHT)
		{
			BODY.setTexture(&ResourceManager::GetInstance()->t_red_soldier);
		}
	}
	else if (m_type == UnitType::ARCHER) 
	{ 
		//BODY.setOutlineColor(sf::Color::Cyan); 
		if (m_side == Team::LEFT)
		{
			BODY.setTexture(&ResourceManager::GetInstance()->t_blue_archer);
		}
		else if (m_side == Team::RIGHT)
		{
			BODY.setTexture(&ResourceManager::GetInstance()->t_red_archer);
		}
	}
	else if (m_type == UnitType::BUILDER) 
	{ 
		//BODY.setOutlineColor(sf::Color::Magenta); 
		if (m_side == Team::LEFT)
		{
			BODY.setTexture(&ResourceManager::GetInstance()->t_blue_builder);
		}
		else if (m_side == Team::RIGHT)
		{
			BODY.setTexture(&ResourceManager::GetInstance()->t_red_builder);
		}
	}
	//BODY.setOutlineThickness(m_radius / 10.0f);

	FOV = sf::CircleShape();
	FOV.setRadius(m_radius * m_rangeSight);
	FOV.setOrigin(sf::Vector2f(FOV.getRadius(), FOV.getRadius()));
	FOV.setPosition(m_position);
	FOV.setFillColor(sf::Color::Transparent);
	FOV.setOutlineColor(sf::Color::Yellow);
	FOV.setOutlineThickness(2.0f);

	RANGE = sf::CircleShape();
	RANGE.setRadius(m_radius * m_rangeAttack);
	RANGE.setOrigin(sf::Vector2f(RANGE.getRadius(), RANGE.getRadius()));
	RANGE.setPosition(m_position);
	RANGE.setFillColor(sf::Color::Transparent);
	RANGE.setOutlineColor(sf::Color::Black);
	RANGE.setOutlineThickness(2.0f);
}

void Unit::InitBuilder()
{
	m_value = BUILDER_VALUE;
	m_valueAttack = 2;
	m_valueDefense = 20;
	m_speedMovement = 4;
	m_speedAttack = 2.5f;
	m_rangeSight = 5;
	m_rangeAttack = 2;

	s_attack = ResourceManager::GetInstance()->s_builder;
	if (!b_attack.loadFromFile("content\\audio\\builder.ogg")) {}

	s_attack.setBuffer(b_attack);
	s_attack.setVolume(5);
}


void Unit::InitSoldier()
{
	m_value = SOLDIER_VALUE;
	m_valueAttack = 10;
	m_valueDefense = 35;
	m_speedMovement = 5;
	m_speedAttack = 2.0f;
	m_rangeSight = 8;
	m_rangeAttack = 2; 

	s_attack = ResourceManager::GetInstance()->s_soldier;
	if (!b_attack.loadFromFile("content\\audio\\soldier.ogg")) {}

	s_attack.setBuffer(b_attack);
	s_attack.setVolume(20);
}

void Unit::InitArcher()
{
	m_value = ARCHER_VALUE;
	m_valueAttack = 4;
	m_valueDefense = 20;
	m_speedMovement = 5;
	m_speedAttack = 1.4f;
	m_rangeSight = 12;
	m_rangeAttack = 8;

	s_attack = ResourceManager::GetInstance()->s_archer;
	if (!b_attack.loadFromFile("content\\audio\\archer.ogg")) {}

	s_attack.setBuffer(b_attack);
	s_attack.setVolume(40);
}

void Unit::CreateGOAPActions()
{
	m_GOAPActions = new std::vector<GOAPAction*>();
	
	if (m_type == UnitType::BUILDER)
	{
		init_action = new GOAPAction(ActionState::Idle_Builder, 5);
	}
	else
	{
		init_action = new GOAPAction(ActionState::Idle, 5);
	}

	m_GOAPActions = init_action->SetUpAction();
}

void Unit::CreateMCTSActions()
{	
	if (m_type == UnitType::BUILDER)
	{
		m_MCTSAction = new MCTSAction(ActionState::Idle_Builder);
	}
	else
	{
		m_MCTSAction = new MCTSAction(ActionState::Idle);
	}
}

// Actions
int Unit::IsIdle()
{
	// Unit is Idle so they should move to new Action
	return SUCCESS;
}

int Unit::Done()
{
	// Goal complete
	return SUCCESS;
}

// Moves
int Unit::MoveToCastle()
{
	// Fail - Unit seen enemy
	if (CollisionManager::GetInstance()->DetectedEnemy(m_id, m_side))
	{
		if (CollisionManager::GetInstance()->CheckWorld_EngageEnemy(m_side))
		{
			return FAIL;
		}
	}

	// Fail - Unit seen structure
	if (CollisionManager::GetInstance()->DetectedWall(m_id, m_side))
	{
		if (CollisionManager::GetInstance()->CheckWorld_EngageStructure(m_id, m_side))
		{
			return FAIL;
		}
	}

	// Win - Unit is in range of Castle
	if (CollisionManager::GetInstance()->ReachedCastle(m_id, m_side))
	{
		return SUCCESS;
	}

	// Move towards Castle
	return CollisionManager::GetInstance()->MoveToCastle(m_id, m_side);
}

int Unit::AttackCastle(const float dt)
{
	// Fail - Unit seen enemy
	if (CollisionManager::GetInstance()->DetectedEnemy(m_id, m_side))
	{
		if (CollisionManager::GetInstance()->CheckWorld_EngageEnemy(m_side))
		{
			return FAIL;
		}
	}

	// Fail - Unit seen structure
	if (CollisionManager::GetInstance()->DetectedWall(m_id, m_side))
	{
		if (CollisionManager::GetInstance()->CheckWorld_EngageStructure(m_id, m_side))
		{
			return FAIL;
		}
	}

	// Decrease time to attack
	m_attackTimer += dt;

	// Unit is in range of Castle
	if (CollisionManager::GetInstance()->ReachedCastle(m_id, m_side))
	{
		// Attack allowed
		if (m_attackTimer >= m_speedAttack * CLOCK_TICK)
		{
			// Reset timer
			m_attackTimer = 0;

			// Sound
			s_attack.play();

			// Return if attack successful
			return CollisionManager::GetInstance()->AttackCastle(m_id, m_side);
		}

		// Continue attack
		return RUNNING;
	}

	// Unable to attack, reset timer, move
	m_attackTimer = 0;

	return MoveToCastle();
}

// Enemy
int Unit::MoveToEnemy()
{
	// Get target
	if (m_target != Target::ENEMY_T)
	{
		if (!CollisionManager::GetInstance()->TargetEnemy(m_id, m_side))
		{
			return FAIL;
		}
	}

	// Fail - Unit seen structure
	if (CollisionManager::GetInstance()->DetectedWall(m_id, m_side))
	{
		if (CollisionManager::GetInstance()->CheckWorld_EngageStructure(m_id, m_side))
		{
			return FAIL;
		}

		else if (CollisionManager::GetInstance()->CheckWorld_TouchingWall(m_id, m_side))
		{
			return FAIL;
		}
	}

	// Win - Unit in range of enemy
	if (CollisionManager::GetInstance()->ReachedEnemy(m_id, m_side))
	{
		return SUCCESS;
	}

	// Move towards Enemy
	return CollisionManager::GetInstance()->MoveToEnemy(m_id, m_side);
}

int Unit::AttackEnemy(const float dt)
{
	if (m_target != Target::ENEMY_T)
	{
		if (!CollisionManager::GetInstance()->TargetEnemy(m_id, m_side))
		{
			return FAIL;
		}
	}

	// Decrease time to attack
	m_attackTimer += dt;

	// Unit is in range of Castle
	if (CollisionManager::GetInstance()->ReachedEnemy(m_id, m_side))
	{
		// Attack allowed
		if (m_attackTimer >= m_speedAttack * CLOCK_TICK)
		{
			// Reset timer
			m_attackTimer = 0;

			// Sound
			s_attack.play();

			// Return if attack successful
			return CollisionManager::GetInstance()->AttackEnemy(m_id, m_side);
		}

		return RUNNING;
	}

	// Unable to attack, reset timer, move
	m_attackTimer = 0;

	// Move towards Enemy
	return MoveToEnemy();
}

int Unit::MoveToStructure()
{
	// Get target
	if (m_target != Target::WALL_T)
	{
		if (!CollisionManager::GetInstance()->TargetStructure(m_id, m_side))
		{
			return FAIL;
		}
	}

	// Win - Unit in range of enemy
	if (CollisionManager::GetInstance()->ReachedStructure(m_id, m_side))
	{
		return SUCCESS;
	}

	// Move towards Enemy
	return CollisionManager::GetInstance()->MoveToStructure(m_id, m_side);
}

int Unit::AttackStructure(const float dt)
{
	// Get target
	if (m_target != Target::WALL_T)
	{
		if (!CollisionManager::GetInstance()->TargetStructure(m_id, m_side))
		{
			return FAIL;
		}
	}

	// Decrease time to attack
	m_attackTimer += dt;

	// Unit is in range of Castle
	if (CollisionManager::GetInstance()->ReachedStructure(m_id, m_side))
	{
		// Attack allowed
		if (m_attackTimer >= m_speedAttack * CLOCK_TICK)
		{
			// Reset timer
			m_attackTimer = 0;

			// Sound
			s_attack.play();

			// Return if attack successful
			return CollisionManager::GetInstance()->AttackStructure(m_id, m_side);
		}

		return RUNNING;
	}

	// Unable to attack, reset timer, move
	m_attackTimer = 0;

	// Move towards Enemy
	return MoveToStructure();
}

int Unit::MoveToBase()
{
	// Fail - Unit seen enemy
	if (CollisionManager::GetInstance()->DetectedEnemy(m_id, m_side))
	{
		if (CollisionManager::GetInstance()->CheckWorld_EngageEnemy(m_side))
		{
			return FAIL;
		}
	}

	// Win - Base reached
	if (CollisionManager::GetInstance()->ReachedBase(m_id, m_side))
	{
		return SUCCESS;
	}

	// Move towards base
	return CollisionManager::GetInstance()->MoveToBase(m_id, m_side);
}

int Unit::MoveToResource()
{
	// Get target
	if (m_target != Target::RESOURCE_T)
	{
		if (!CollisionManager::GetInstance()->TargetResource(m_id, m_side))
		{
			return FAIL;
		}
	}

	// Win - Resources reached
	if (CollisionManager::GetInstance()->ReachedResource(m_id, m_side))
	{
		return SUCCESS;
	}

	// Move towards base
	return CollisionManager::GetInstance()->MoveToResource(m_id, m_side);
}

int Unit::GatherResources(const float dt)
{
	// Get target
	if (m_target != Target::RESOURCE_T)
	{
		if (!CollisionManager::GetInstance()->TargetResource(m_id, m_side))
		{
			return FAIL;
		}
	}

	// Decrease time to gather 
	m_attackTimer += dt;

	// Unit is in range of Castle
	if (CollisionManager::GetInstance()->ReachedResource(m_id, m_side))
	{
		// Attack allowed
		if (m_attackTimer >= m_speedAttack * CLOCK_TICK)
		{
			// Reset timer
			m_attackTimer = 0;

			// Sound
			s_attack.play();

			// Return if attack successful
			return CollisionManager::GetInstance()->GatherResource(m_id, m_side);
		}

		// Continue attack
		return RUNNING;
	}

	// Unable to attack, reset timer, move
	m_attackTimer = 0;
	
	return MoveToResource();;
}

int Unit::MoveToSite()
{
	// Get target
	if (m_target != Target::SITE_T)
	{
		if (!CollisionManager::GetInstance()->TargetSite(m_id, m_side))
		{
			return FAIL;
		}
	}

	//// Fail - Unit should NOT build wall
	//if (!CollisionManager::GetInstance()->CheckWorld_BuildStructure(m_id, m_side)) // is home castle open to attack?
	//{
	//	return FAIL;
	//}

	//if (CollisionManager::GetInstance()->CheckWorld_HomeWallsUp(m_side))
	//{
	//	return FAIL;
	//}

	// Win - Site reached
	if (CollisionManager::GetInstance()->ReachedSite(m_id, m_side))
	{
		return SUCCESS;
	}

	// Move towards site
	return CollisionManager::GetInstance()->MoveToSite(m_id, m_side);
}

int Unit::BuildStructure(const float dt)
{
	// Get target
	if (m_target != Target::SITE_T)
	{
		if (!CollisionManager::GetInstance()->TargetSite(m_id, m_side))
		{
			return FAIL;
		}
	}

	if (CollisionManager::GetInstance()->CheckWorld_HomeWallsUp(m_side))
	{
		return FAIL;
	}

	// Decrease time to build
	m_attackTimer += dt;

	// Unit is in range of Castle
	if (CollisionManager::GetInstance()->ReachedSite(m_id, m_side))
	{
		// Attack allowed
		if (m_attackTimer >= m_speedAttack * CLOCK_TICK)
		{
			// Reset timer
			m_attackTimer = 0;

			// Sound
			s_attack.play();

			// Return if attack successful
			return CollisionManager::GetInstance()->BuildStructure(m_id, m_side);
		}

		// Continue attack
		return RUNNING;
	}

	// Unable to attack, reset timer
	m_attackTimer = 0;
	
	return MoveToResource();
}


//// GETTERS AND SETTERS

bool Unit::GetAlive() const
{
	return alive;
}

void Unit::SetAlive(const bool isAlive)
{
	alive = isAlive;
}

int Unit::GetID() const
{
	return m_id;
}

void Unit::SetID(const int newID)
{
	m_id = newID;
}

WorldState Unit::GetObjective() const
{
	return m_objective;
}

void Unit::SetObjective(const WorldState & newObjective)
{
	m_objective = newObjective;
}

Team Unit::GetSide() const
{
	return m_side;
}

void Unit::SetSide(const Team & newSide)
{
	m_side = newSide;
}

UnitType Unit::GetType() const
{
	return m_type;
}

void Unit::SetType(const UnitType & newType)
{
	m_type = newType;
}

std::vector<GOAPAction*>* Unit::GetGOAPActions() const
{
	return m_GOAPActions;
}

MCTSAction * Unit::GetMCTSAction() const
{
	return m_MCTSAction;
}

sf::Vector2f Unit::GetPosition() const
{
	return m_position;
}

void Unit::SetPosition(const sf::Vector2f & newPosition)
{
	m_position = newPosition;
}

sf::CircleShape Unit::GetBody() const
{
	return BODY;
}

sf::CircleShape Unit::GetRange() const
{
	return RANGE;
}

sf::CircleShape Unit::GetFOV() const
{
	return FOV;
}

Target Unit::GetTarget() const
{
	return m_target;
}

void Unit::SetTarget(const Target & newTarget)
{
	m_target = newTarget;
}

float Unit::GetMovementSpeed() const
{
	return m_speedMovement;
}

float Unit::GetAttackTimer() const
{
	return m_attackTimer;
}

void Unit::SetAttackTimer(const float newTime)
{
	m_attackTimer = newTime;
}

float Unit::GetValue() const
{
	return m_value;
}

float Unit::GetAttackValue() const
{
	return m_valueAttack;
}

float Unit::GetDefenceValue() const
{
	return m_valueDefense;
}

void Unit::SetDefenceValue(const float newHealth)
{
	m_valueDefense = newHealth;
}

void Unit::ReduceHealthBy(const float damage)
{
	m_valueDefense -= damage;
}
