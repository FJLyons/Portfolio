#include "Manager_Collision.h"

CollisionManager* CollisionManager::collisionManagerInstance = nullptr;

CollisionManager::CollisionManager()
{
	InitVariables();
}

CollisionManager::~CollisionManager()
{
}

CollisionManager * CollisionManager::GetInstance()
{
	if (collisionManagerInstance == nullptr)
	{
		collisionManagerInstance = new CollisionManager();
	}
	return collisionManagerInstance;
}


//// MATHS FUNCTIONS ////


void CollisionManager::CalculateValues(const Team& side, const float gameTime)
{
	std::map<int, Unit*> homeUnits;
	std::vector<Structure*> homeStructures;
	Structure* castle;

	float valueTotal = 0;
	float valueAttack = 0;
	float valueDefence = 0;

	int builders = 0;
	int soldiers = 0;
	int archers = 0;
	int walls = 0;

	int totalBuilders = 0;
	int totalSoldiers = 0;
	int totalArchers = 0;

	if (side == Team::LEFT)
	{
		homeUnits = m_leftUnits;
		homeStructures = m_leftWalls;
		castle = m_leftCastle;
		valueTotal += m_leftGold;
	}

	else
	{
		homeUnits = m_rightUnits;
		homeStructures = m_rightWalls;
		castle = m_rightCastle;
		valueTotal += m_rightGold;
	}


	for (const auto& friendly : homeUnits)
	{
		if (friendly.second->GetAlive())
		{
			valueTotal += (friendly.second->GetValue() + friendly.second->GetAttackValue() + friendly.second->GetDefenceValue());
			valueAttack += friendly.second->GetAttackValue();
			valueDefence += friendly.second->GetDefenceValue();

			if (friendly.second->GetType() == UnitType::BUILDER)
			{
				builders++;
			}
			if (friendly.second->GetType() == UnitType::SOLDIER)
			{
				soldiers++;
			}
			if (friendly.second->GetType() == UnitType::ARCHER)
			{
				archers++;
			}
		}

		if (friendly.second->GetType() == UnitType::BUILDER)
		{
			totalBuilders++;
		}
		if (friendly.second->GetType() == UnitType::SOLDIER)
		{
			totalSoldiers++;
		}
		if (friendly.second->GetType() == UnitType::ARCHER)
		{
			totalArchers++;
		}
	}

	for (const auto& friendly : homeStructures)
	{
		if (friendly->GetAlive())
		{
			valueTotal += (friendly->GetValue() + friendly->GetDefenceValue());
			valueDefence += friendly->GetDefenceValue();
			walls++;
		}
	}

	if (castle->GetAlive())
	{
		valueTotal += castle->GetValue() + castle->GetDefenceValue();
		valueDefence += castle->GetDefenceValue();
	}

	// Results
	if (side == Team::LEFT)
	{
		m_leftSideValue = valueTotal;
		m_leftAttackValue = valueAttack;
		m_leftDefenceValue = valueDefence;

		m_leftBuilders = builders;
		m_leftSoldiers = soldiers;
		m_leftArchers = archers;
		m_leftWallsBuilt = walls;

		m_leftBuildersTotal = totalBuilders;
		m_leftSoldiersTotal = totalSoldiers;
		m_leftArchersTotal = totalArchers;

		if (m_leftSideValue > m_leftSideValueHighest)
		{
			m_leftSideValueHighest = m_leftSideValue;
			m_leftSideValueHighestSeconds = gameTime / 1000.0f;
		}

		if (m_leftAttackValue > m_leftAttackValueHighest)
		{
			m_leftAttackValueHighest = m_leftAttackValue;
		}

		if (m_leftDefenceValue > m_leftDefenceValueHighest)
		{
			m_leftDefenceValueHighest = m_leftDefenceValue;
		}
	}

	else
	{
		m_rightSideValue = valueTotal;
		m_rightAttackValue = valueAttack;
		m_rightDefenceValue = valueDefence;

		m_rightBuilders = builders;
		m_rightSoldiers = soldiers;
		m_rightArchers = archers;
		m_rightWallsBuilt = walls;

		m_rightBuildersTotal = totalBuilders;
		m_rightSoldiersTotal = totalSoldiers;
		m_rightArchersTotal = totalArchers;

		if (m_rightSideValue > m_rightSideValueHighest)
		{
			m_rightSideValueHighest = m_rightSideValue;
			m_rightSideValueHighestSeconds = gameTime / 1000.0f;
		}

		if (m_rightAttackValue > m_rightAttackValueHighest)
		{
			m_rightAttackValueHighest = m_rightAttackValue;
		}

		if (m_rightDefenceValue > m_rightDefenceValueHighest)
		{
			m_rightDefenceValueHighest = m_rightDefenceValue;
		}
	}
}

sf::Vector2f CollisionManager::Normalise(const sf::Vector2f& vec) const
{
	float length(sqrt((vec.x * vec.x) + (vec.y * vec.y)));

	if (length != 0)
	{
		sf::Vector2f normalised((vec.x / length), (vec.y / length));
		return normalised;
	}

	return sf::Vector2f(0, 0);
}

void CollisionManager::ApplyDamage()
{
	for (const auto& unit : m_targetUnitDamage)
	{
		unit.first->ReduceHealthBy(unit.second);
	}
	CompareKills();
	m_targetUnitDamage.erase(m_targetUnitDamage.begin(), m_targetUnitDamage.end());
}

void CollisionManager::Reset()
{
	InitVariables();

	//// OBJECTS
	m_leftUnits.clear();
	m_rightUnits.clear();

	m_leftWalls.clear();
	m_rightWalls.clear();

	m_leftResources.clear();
	m_rightResources.clear();

	m_targetUnit.clear();
	m_targetWall.clear();

	m_targetResource.clear();
	m_targetSite.clear();

	m_targetUnitDamage.clear();

	m_targetPosition.clear();
}

void CollisionManager::InitVariables()
{
	//// VARIABLES
	m_leftSideValue = 0;
	m_rightSideValue = 0;

	m_leftAttackValue = 0;
	m_rightAttackValue = 0;

	m_leftDefenceValue = 0;
	m_rightDefenceValue = 0;

	m_leftGold = 0;
	m_rightGold = 0;

	m_leftBuilders = 0;
	m_leftSoldiers = 0;
	m_leftArchers = 0;

	m_rightBuilders = 0;
	m_rightSoldiers = 0;
	m_rightArchers = 0;

	m_leftSideValueHighest = 0;
	m_rightSideValueHighest = 0;
	m_leftSideValueHighestSeconds = 0;
	m_rightSideValueHighestSeconds = 0;

	m_leftAttackValueHighest = 0;
	m_rightAttackValueHighest = 0;

	m_leftDefenceValueHighest = 0;
	m_rightDefenceValueHighest = 0;

	m_leftBuildersTotal = 0;
	m_leftSoldiersTotal = 0;
	m_leftArchersTotal = 0;
	m_leftResourcesTotal = 0;
	m_leftWallsTotal = 0;

	m_rightBuildersTotal = 0;
	m_rightSoldiersTotal = 0;
	m_rightArchersTotal = 0;
	m_rightResourcesTotal = 0;
	m_rightWallsTotal = 0;

	m_leftBuildingQueue = 0;
	m_rightBuildingQueue = 0;

	w_engageCastle = 0;
	w_engageEnemy = 0;
	w_engageStructure = 0;
	
	w_defendCastle = 0;
	w_buildStructure = 0;
	
	w_needResources = 0;
}

void CollisionManager::CompareKills()
{
	for (const auto& unit : m_targetUnitDamage)
	{
		if (unit.first->GetAlive() && unit.first->GetDefenceValue() <= 0)
		{
			unit.first->SetAlive(false);
		}
	}
}

void CollisionManager::PlanRouteLeft(const int id, const Team& side)
{
	Unit* unit;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
	}

	else
	{
		unit = m_rightUnits[id];
	}

	float closestWall = MAX_LENGHT;

	sf::Vector2f closestPosition = sf::Vector2f(0, 0);
	sf::Vector2f offset = sf::Vector2f(75, 0);

	m_targetPosition[unit].clear();

	if (unit->GetPosition().x - unit->GetBody().getRadius() > RIGHTSITEBACK)
	{
		for (int i = 0; i < 3; i++)
		{
			if (!m_rightWalls[i]->GetAlive())
			{
				float xd, yd;
				xd = unit->GetPosition().x - m_rightWalls[i]->GetPosition().x;
				yd = unit->GetPosition().y - m_rightWalls[i]->GetPosition().y;

				float length = sqrt(xd * xd + yd * yd);

				if (length < closestWall)
				{
					closestWall = length;
					closestPosition = m_rightWalls[i]->GetPosition();
				}
			}
		}
	}

	else if (unit->GetPosition().x - unit->GetBody().getRadius() > RIGHTSITEMID)
	{
		for (int i = 3; i < 6; i++)
		{
			if (!m_rightWalls[i]->GetAlive())
			{
				float xd, yd;
				xd = unit->GetPosition().x - m_rightWalls[i]->GetPosition().x;
				yd = unit->GetPosition().y - m_rightWalls[i]->GetPosition().y;

				float length = sqrt(xd * xd + yd * yd);

				if (length < closestWall)
				{
					closestWall = length;
					closestPosition = m_rightWalls[i]->GetPosition();
				}
			}
		}
	}

	else if (unit->GetPosition().x - unit->GetBody().getRadius() > RIGHTSITEFRONT)
	{
		for (int i = 6; i < 9; i++)
		{
			if (!m_rightWalls[i]->GetAlive())
			{
				float xd, yd;
				xd = unit->GetPosition().x - m_rightWalls[i]->GetPosition().x;
				yd = unit->GetPosition().y - m_rightWalls[i]->GetPosition().y;

				float length = sqrt(xd * xd + yd * yd);

				if (length < closestWall)
				{
					closestWall = length;
					closestPosition = m_rightWalls[i]->GetPosition();
				}
			}
		}
	}


	if (unit->GetPosition().x - unit->GetBody().getRadius() < RIGHTSITEFRONT)
	{
		if (unit->GetPosition().x - unit->GetBody().getRadius() > LEFTSITEFRONT)
		{
			for (int i = 6; i < 9; i++)
			{
				if (!m_leftWalls[i]->GetAlive())
				{
					float xd, yd;
					xd = unit->GetPosition().x - m_leftWalls[i]->GetPosition().x;
					yd = unit->GetPosition().y - m_leftWalls[i]->GetPosition().y;

					float length = sqrt(xd * xd + yd * yd);

					if (length < closestWall)
					{
						closestWall = length;
						closestPosition = m_leftWalls[i]->GetPosition();
					}
				}
			}
		}

		else if (unit->GetPosition().x - unit->GetBody().getRadius() > LEFTSITEMID)
		{
			for (int i = 3; i < 6; i++)
			{
				if (!m_leftWalls[i]->GetAlive())
				{
					float xd, yd;
					xd = unit->GetPosition().x - m_leftWalls[i]->GetPosition().x;
					yd = unit->GetPosition().y - m_leftWalls[i]->GetPosition().y;

					float length = sqrt(xd * xd + yd * yd);

					if (length < closestWall)
					{
						closestWall = length;
						closestPosition = m_leftWalls[i]->GetPosition();
					}
				}
			}
		}

		else if (unit->GetPosition().x - unit->GetBody().getRadius() > LEFTSITEBACK)
		{
			for (int i = 0; i < 3; i++)
			{
				if (!m_leftWalls[i]->GetAlive())
				{
					float xd, yd;
					xd = unit->GetPosition().x - m_leftWalls[i]->GetPosition().x;
					yd = unit->GetPosition().y - m_leftWalls[i]->GetPosition().y;

					float length = sqrt(xd * xd + yd * yd);

					if (length < closestWall)
					{
						closestWall = length;
						closestPosition = m_leftWalls[i]->GetPosition();
					}
				}
			}
		}
	}

	if (closestWall != MAX_LENGHT)
	{
		m_targetPosition[unit].push_back(closestPosition + offset);
		m_targetPosition[unit].push_back(closestPosition - offset);
	}

	// Reverse so Castle is last destination from back
	std::reverse(m_targetPosition[unit].begin(), m_targetPosition[unit].end());
}

void CollisionManager::PlanRouteRight(const int id, const Team& side)
{
	Unit* unit;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
	}

	else
	{
		unit = m_rightUnits[id];
	}

	float closestWall = MAX_LENGHT;

	sf::Vector2f closestPosition = sf::Vector2f(0, 0);
	sf::Vector2f offset = sf::Vector2f(75, 0);

	m_targetPosition[unit].clear();

	if (unit->GetPosition().x + unit->GetBody().getRadius() < LEFTSITEBACK)
	{
		for (int i = 0; i < 3; i++)
		{
			if (!m_leftWalls[i]->GetAlive())
			{
				float xd, yd;
				xd = unit->GetPosition().x - m_leftWalls[i]->GetPosition().x;
				yd = unit->GetPosition().y - m_leftWalls[i]->GetPosition().y;

				float length = sqrt(xd * xd + yd * yd);

				if (length < closestWall)
				{
					closestWall = length;
					closestPosition = m_leftWalls[i]->GetPosition();
				}
			}
		}
	}

	else if (unit->GetPosition().x + unit->GetBody().getRadius() < LEFTSITEMID)
	{
		for (int i = 3; i < 6; i++)
		{
			if (!m_leftWalls[i]->GetAlive())
			{
				float xd, yd;
				xd = unit->GetPosition().x - m_leftWalls[i]->GetPosition().x;
				yd = unit->GetPosition().y - m_leftWalls[i]->GetPosition().y;

				float length = sqrt(xd * xd + yd * yd);

				if (length < closestWall)
				{
					closestWall = length;
					closestPosition = m_leftWalls[i]->GetPosition();
				}
			}
		}
	}

	else if (unit->GetPosition().x + unit->GetBody().getRadius() < LEFTSITEFRONT)
	{
		for (int i = 6; i < 9; i++)
		{
			if (!m_leftWalls[i]->GetAlive())
			{
				float xd, yd;
				xd = unit->GetPosition().x - m_leftWalls[i]->GetPosition().x;
				yd = unit->GetPosition().y - m_leftWalls[i]->GetPosition().y;

				float length = sqrt(xd * xd + yd * yd);

				if (length < closestWall)
				{
					closestWall = length;
					closestPosition = m_leftWalls[i]->GetPosition();
				}
			}
		}
	}


	if (unit->GetPosition().x + unit->GetBody().getRadius() > LEFTSITEFRONT)
	{
		if (unit->GetPosition().x + unit->GetBody().getRadius() < RIGHTSITEFRONT)
		{
			for (int i = 6; i < 9; i++)
			{
				if (!m_rightWalls[i]->GetAlive())
				{
					float xd, yd;
					xd = unit->GetPosition().x - m_rightWalls[i]->GetPosition().x;
					yd = unit->GetPosition().y - m_rightWalls[i]->GetPosition().y;

					float length = sqrt(xd * xd + yd * yd);

					if (length < closestWall)
					{
						closestWall = length;
						closestPosition = m_rightWalls[i]->GetPosition();
					}
				}
			}
		}

		else if (unit->GetPosition().x + unit->GetBody().getRadius() < RIGHTSITEMID)
		{
			for (int i = 3; i < 6; i++)
			{
				if (!m_rightWalls[i]->GetAlive())
				{
					float xd, yd;
					xd = unit->GetPosition().x - m_rightWalls[i]->GetPosition().x;
					yd = unit->GetPosition().y - m_rightWalls[i]->GetPosition().y;

					float length = sqrt(xd * xd + yd * yd);

					if (length < closestWall)
					{
						closestWall = length;
						closestPosition = m_rightWalls[i]->GetPosition();
					}
				}
			}
		}

		else if (unit->GetPosition().x + unit->GetBody().getRadius() < RIGHTSITEBACK)
		{
			for (int i = 0; i < 3; i++)
			{
				if (!m_rightWalls[i]->GetAlive())
				{
					float xd, yd;
					xd = unit->GetPosition().x - m_rightWalls[i]->GetPosition().x;
					yd = unit->GetPosition().y - m_rightWalls[i]->GetPosition().y;

					float length = sqrt(xd * xd + yd * yd);

					if (length < closestWall)
					{
						closestWall = length;
						closestPosition = m_rightWalls[i]->GetPosition();
					}
				}
			}
		}
	}

	if (closestWall != MAX_LENGHT)
	{
		m_targetPosition[unit].push_back(closestPosition - offset);
		m_targetPosition[unit].push_back(closestPosition + offset);
	}

	// Reverse so Castle is last destination from back
	std::reverse(m_targetPosition[unit].begin(), m_targetPosition[unit].end());
}

void CollisionManager::ReachedTarget(const int id, const Team& side)
{
	Unit* unit;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
	}

	else
	{
		unit = m_rightUnits[id];
	}

	float radius1 = unit->GetBody().getRadius() / 2;
	float radius2 = 2;

	float xd, yd;
	xd = m_targetPosition[unit].back().x - unit->GetPosition().x;
	yd = m_targetPosition[unit].back().y - unit->GetPosition().y;

	if (sqrt(xd * xd + yd * yd) <= radius1 + radius2)
	{
		m_targetPosition[unit].pop_back();
	}
}


//// CHECK WORLD //// 


bool CollisionManager::CheckWorld_EngageCastle(const Team& side)
{
	std::map<int, Unit*> homeUnits;
	std::map<int, Unit*> enemyUnits;

	int allies = 0;
	int enemies = 0;

	// Get unit side
	if (side == Team::LEFT)
	{
		homeUnits = m_leftUnits;
		enemyUnits = m_rightUnits;
	}
	else
	{
		homeUnits = m_rightUnits;
		enemyUnits = m_leftUnits;
	}

	// Check Enemies
	for (const auto& enemy : enemyUnits)
	{
		if (enemy.second->GetAlive())
		{
			if(enemy.second->GetObjective() == WorldState::Defend_Castle ||
				enemy.second->GetObjective() == WorldState::Engage_Castle ||
				enemy.second->GetObjective() == WorldState::Engage_Enemies)
				enemies++;
		}
	}

	// Check Friendlies
	for (const auto& friendly : homeUnits)
	{
		if (friendly.second->GetAlive() && friendly.second->GetObjective() == WorldState::Engage_Castle)
		{
			allies++;
		}
	}

	w_engageCastle = enemies - allies;

	// If defenders < attackers
	if (enemies < allies * 0.75f || enemies == 0)
	{
		return true; // castle is open to attack
	}

	return false; // castle is defended
}

bool CollisionManager::CheckWorld_EngageEnemy(const Team& side)
{
	std::map<int, Unit*> homeUnits;
	std::map<int, Unit*> enemyUnits;

	int allies = 0;
	int enemies = 0;

	// Get unit side
	if (side == Team::LEFT)
	{
		homeUnits = m_leftUnits;
		enemyUnits = m_rightUnits;
	}
	else
	{
		homeUnits = m_rightUnits;
		enemyUnits = m_leftUnits;
	}

	// Check Enemies
	for (const auto& enemy : enemyUnits)
	{
		if (enemy.second->GetAlive())
		{
			if (enemy.second->GetObjective() == WorldState::Engage_Castle ||
				enemy.second->GetObjective() == WorldState::Engage_Enemies)
				enemies++; // Enemy within FOV of its castle
		}
	}

	// Check Friendlies
	for (const auto& friendly : homeUnits)
	{
		if (friendly.second->GetAlive() && friendly.second->GetObjective() == WorldState::Engage_Enemies)
		{
			allies++; // Enemy within FOV of its castle
		}
	}

	w_engageEnemy = allies - enemies;

	// If attackers > home attackers
	if (enemies >= allies * 0.75f)
	{
		return true; // Should Engage
	}

	return false; // Don't Engage
}

bool CollisionManager::CheckWorld_EngageStructure(const int id, const Team& side)
{
	// If home castle under attack - run
	if (DetectedEnemy(id, side))
	{
		return false; // Do Not Engage
	}

	if (CheckWorld_WallsUp(side)) // front
	{
		return true; // Path blocked - engage
	}

	return false;
}

bool CollisionManager::CheckWorld_DefendCastle(const Team& side)
{
	std::map<int, Unit*> homeUnits;
	std::map<int, Unit*> enemyUnits;
	Structure* homeCastle;

	int allies = 0;
	int enemies = 0;

	// Get unit side
	if (side == Team::LEFT)
	{
		homeUnits = m_leftUnits;
		homeCastle = m_leftCastle;
		enemyUnits = m_rightUnits;
	}
	else
	{
		homeUnits = m_rightUnits;
		homeCastle = m_rightCastle;
		enemyUnits = m_leftUnits;
	}

	// Check Enemies
	for (const auto& enemy : enemyUnits)
	{
		if (enemy.second->GetAlive() && enemy.second->GetObjective() == WorldState::Engage_Castle)
		{
			float radius1 = enemy.second->GetFOV().getRadius();
			float radius2 = homeCastle->GetRadius();
			float xd, yd;

			xd = homeCastle->GetPosition().x - enemy.second->GetPosition().x;
			yd = homeCastle->GetPosition().y - enemy.second->GetPosition().y;

			if (sqrt(xd * xd + yd * yd) <= radius1 + radius2)
			{
				enemies++; // Enemy within FOV of its castle
			}

			//if (homeCastle->GetBody().getGlobalBounds().intersects(enemy.second->GetFOV().getGlobalBounds()))
			//{
			//	enemies++; // Enemy within FOV of its castle
			//}
		}
	}

	// Check Friendlies
	for (const auto& friendly : homeUnits)
	{
		if (friendly.second->GetAlive() && friendly.second->GetObjective() == WorldState::Defend_Castle)
		{
			float radius1 = friendly.second->GetFOV().getRadius();
			float radius2 = homeCastle->GetRadius();
			float xd, yd;

			xd = homeCastle->GetPosition().x - friendly.second->GetPosition().x;
			yd = homeCastle->GetPosition().y - friendly.second->GetPosition().y;

			if (sqrt(xd * xd + yd * yd) <= radius1 + radius2)
			{
				allies++; // Allies within FOV of its castle
			}

			//if (homeCastle->GetBody().getGlobalBounds().intersects(friendly.second->GetFOV().getGlobalBounds()))
			//{
			//	allies++; // Allies within FOV of its castle
			//}
		}
	}

	w_defendCastle = allies - enemies;

	// If defenders < attackers AND someone is attacking
	if (allies <= enemies && enemies != 0)
	{
		return true; // castle is undefended - need to defend
	}

	return false; // castle is defended - no need to send unit
}

bool CollisionManager::CheckWorld_BuildStructure(const int id, const Team& side)
{
	// If home castle under attack - run
	if (DetectedEnemy(id, side))
	{
		return false; // Do Not Engage
	}

	if (CheckWorld_HomeWallsUp(side)) // front
	{
		return false; // Path blocked - engage
	}

	return true;
}

bool CollisionManager::CheckWorld_NeedResources(const Team& side)
{
	if (CheckWorld_ResourcesGone(side))
	{
		return false;
	}

	float homeValue = 0;
	float enemyValue = 0;

	// Get unit side
	if (side == Team::LEFT)
	{
		homeValue = m_leftSideValue;
		enemyValue = m_rightSideValue;
	}
	else
	{
		homeValue = m_rightSideValue;
		enemyValue = m_leftSideValue;
	}

	w_needResources = (homeValue / 100) - (enemyValue / 100);

	// If value < enemy value
	if (homeValue <= enemyValue)
	{
		return true; // gather
	}

	return false; // no need to gather
}


//// CHECK WORLD OVERWRITERS ////


bool CollisionManager::CheckWorld_EnemiesDead(const Team& side) const
{
	std::map<int, Unit*> enemyUnits;

	// Get unit side
	if (side == Team::LEFT)
	{
		enemyUnits = m_rightUnits;
	}
	else
	{
		enemyUnits = m_leftUnits;
	}

	for (const auto& enemy : enemyUnits)
	{
		if (enemy.second->GetAlive())
		{
			return false; // An enemy is alive
		}
	}

	return true;
}

bool CollisionManager::CheckWorld_ResourcesGone(const Team& side) const
{
	std::vector<Structure*> resources;

	// Get unit side
	if (side == Team::LEFT)
	{
		resources = m_leftResources;
	}
	else
	{
		resources = m_rightResources;
	}

	for (const auto& piece : resources)
	{
		if (piece->GetAlive())
		{
			return false; // A resource is available
		}
	}

	return true;
}

bool CollisionManager::CheckWorld_WallsUp(const Team& side) const
{
	std::vector<Structure*> enemyWalls;

	// Get unit side
	if (side == Team::LEFT)
	{
		enemyWalls = m_rightWalls;
	}
	else
	{
		enemyWalls = m_leftWalls;
	}

	if ((enemyWalls[0]->GetAlive() && enemyWalls[1]->GetAlive() && enemyWalls[2]->GetAlive()) // back
		|| (enemyWalls[3]->GetAlive() && enemyWalls[4]->GetAlive() && enemyWalls[5]->GetAlive()) // mid
		|| (enemyWalls[6]->GetAlive() && enemyWalls[7]->GetAlive() && enemyWalls[8]->GetAlive())) // front
	{
		return true; // Path blocked - engage
	}

	return false;
}

bool CollisionManager::CheckWorld_HomeWallsUp(const Team& side) const
{
	std::vector<Structure*> homeWalls;
	int wallsUp = 0;

	// Get unit side
	if (side == Team::LEFT)
	{
		homeWalls = m_leftWalls;
	}
	else
	{
		homeWalls = m_rightWalls;
	}

	for(const auto& wall : homeWalls)
	{
		if (wall->GetAlive())
		{
			wallsUp++;
		}
	}

	if (wallsUp == 9)
	{
		return true; // all walls up
	}

	return false;
}

bool CollisionManager::CheckWorld_TouchingWall(const int id, const Team& side)
{
	Unit* unit;
	std::vector<Structure*> enemyWalls;


	// Get unit side
	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
		enemyWalls = m_rightWalls;
	}
	else
	{
		unit = m_rightUnits[id];
		enemyWalls = m_leftWalls;
	}

	// search through enemies
	for (const auto& wall : enemyWalls)
	{
		if (wall->GetAlive())
		{
			if (wall->GetBody().getGlobalBounds().intersects(unit->GetBody().getGlobalBounds()))
			{
				return true; // Wall spotted
			}
		}
	}

	return false;
}


void CollisionManager::BuildStructureRequest(const Team& side)
{
	if (side == Team::LEFT)
	{
		if (m_leftBuildingQueue < 0)
		{
			m_leftBuildingQueue = 0;
		}
		m_leftBuildingQueue++;
	}
	else
	{
		if (m_rightBuildingQueue < 0)
		{
			m_rightBuildingQueue = 0;
		}
		m_rightBuildingQueue++;
	}
}

bool CollisionManager::BuildStructureRequested(const Team& side)
{
	int queue = 0;

	// Get unit side
	if (side == Team::LEFT)
	{
		if (m_leftBuildingQueue > 0)
		{
			queue = m_leftBuildingQueue;
			m_leftBuildingQueue--;
		}
	}
	else
	{
		if (m_rightBuildingQueue > 0)
		{
			queue = m_rightBuildingQueue;
			m_rightBuildingQueue--;
		}
	}

	if (queue > 0) // front
	{
		return true; // build
	}

	return false;
}


//// TARGETS //// 


bool CollisionManager::TargetEnemy(const int id, const Team& side)
{
	Unit* unit;
	std::map<int, Unit*> enemyUnits;

	float closestUnit = MAX_LENGHT;

	// Get unit side
	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];

		enemyUnits = m_rightUnits;
	}
	else
	{
		unit = m_rightUnits[id];

		enemyUnits = m_leftUnits;
	}

	// search through enemies
	for (const auto& enemy : enemyUnits)
	{
		if (enemy.second->GetAlive())
		{
			float xd, yd;
			xd = unit->GetPosition().x - enemy.second->GetPosition().x;
			yd = unit->GetPosition().y - enemy.second->GetPosition().y;

			float length = sqrt(xd * xd + yd * yd);

			if (length < closestUnit)
			{
				closestUnit = length;
				m_targetUnit[unit] = enemy.second;
				unit->SetTarget(Target::ENEMY_T);
			}
		}
	}

	if (closestUnit != MAX_LENGHT)
	{
		return true; // enemy found
	}

	m_targetUnit.erase(unit);
	unit->SetTarget(Target::NO_T);

	return false;
}

bool CollisionManager::TargetStructure(const int id, const Team& side)
{
	Unit* unit;
	std::vector<Structure*> enemyWalls;

	float closestStructure = MAX_LENGHT;

	// Get unit side
	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];

		enemyWalls = m_rightWalls;
	}
	else
	{
		unit = m_rightUnits[id];

		enemyWalls = m_leftWalls;
	}

	for (const auto& wall : enemyWalls)
	{
		if (wall->GetAlive())
		{
			float xd, yd;
			xd = unit->GetPosition().x - wall->GetPosition().x;
			yd = unit->GetPosition().y - wall->GetPosition().y;

			float length = sqrt(xd * xd + yd * yd);

			if (length < closestStructure)
			{
				closestStructure = length;
				m_targetWall[unit] = wall;
				unit->SetTarget(Target::WALL_T);
			}
		}
	}

	if (closestStructure != MAX_LENGHT)
	{
		return true;
	}

	m_targetWall.erase(unit);
	unit->SetTarget(Target::NO_T);

	return false;
}

bool CollisionManager::TargetResource(const int id, const Team& side)
{
	Unit* unit;
	std::vector<Structure*> resources;

	float closestResource = MAX_LENGHT;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];

		resources = m_leftResources;
	}
	else
	{
		unit = m_rightUnits[id];

		resources = m_rightResources;
	}

	for (const auto& piece : resources)
	{
		if (piece->GetAlive())
		{
			float xd, yd;
			xd = unit->GetPosition().x - piece->GetPosition().x;
			yd = unit->GetPosition().y - piece->GetPosition().y;

			float length = sqrt(xd * xd + yd * yd);

			if (length < closestResource)
			{
				closestResource = length;
				m_targetResource[unit] = piece;
				unit->SetTarget(Target::RESOURCE_T);
			}
		}
	}

	if (closestResource != MAX_LENGHT)
	{
		return true;
	}

	m_targetResource.erase(unit);
	unit->SetTarget(Target::NO_T);

	return false;
}

bool CollisionManager::TargetSite(const int id, const Team& side)
{
	Unit* unit;
	std::vector<Structure*> sites;

	// Get unit side
	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
		sites = m_leftWalls;
	}
	else
	{
		unit = m_rightUnits[id];
		sites = m_rightWalls;
	}

	// Check if builder died
	Unit* unitToRemove;
	for (const auto& target : m_targetSite)
	{
		if (!target.first->GetAlive() && target.second->GetBuilding())
		{
			target.second->SetBuilding(false);
			unitToRemove = target.first;
		}
	}
	m_targetSite.erase(unitToRemove);

	for (const auto& site : sites)
	{
		if (!site->GetAlive())
		{
			if (!site->GetBuilding())
			{
				m_targetSite[unit] = site;
				site->SetBuilding(true);
				unit->SetTarget(Target::SITE_T);
				return true;
			}
		}
	}

	m_targetSite.erase(unit);
	unit->SetTarget(Target::NO_T);

	return false;
}


//// MOVE TO ////


int CollisionManager::MoveToCastle(const int id, const Team& side)
{
	Unit* unit;
	Structure* enemyCastle;
	sf::Vector2f target;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
		enemyCastle = m_rightCastle;

		if (m_targetPosition[unit].empty())
		{
			PlanRouteRight(id, side);
		}
	}

	else
	{
		unit = m_rightUnits[id];
		enemyCastle = m_leftCastle;

		if (m_targetPosition[unit].empty())
		{
			PlanRouteLeft(id, side);
		}
	}

	if (m_targetPosition[unit].size() > 0)
	{
		target = m_targetPosition[unit].back();
		ReachedTarget(id, side);
	}
	else
	{
		m_targetPosition[unit].clear();
		target = enemyCastle->GetPosition();
	}

	sf::Vector2f direction = Normalise(sf::Vector2f(target - unit->GetPosition()));
	unit->SetPosition(unit->GetPosition() + sf::Vector2f(direction.x * unit->GetMovementSpeed(), direction.y * unit->GetMovementSpeed()));

	return 0;
}

int CollisionManager::MoveToEnemy(const int id, const Team& side)
{
	Unit* unit;
	Unit* enemy;
	sf::Vector2f target;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
	}

	else
	{
		unit = m_rightUnits[id];
	}

	enemy = m_targetUnit[unit];

	if (m_targetPosition[unit].empty())
	{
		if (enemy->GetPosition().x + enemy->GetBody().getRadius() >= unit->GetPosition().x - unit->GetBody().getRadius())
		{
			PlanRouteRight(id, side);
		}
		else
		{
			PlanRouteLeft(id, side);
		}
	}

	if (m_targetPosition[unit].size() > 0)
	{
		target = m_targetPosition[unit].back();
		ReachedTarget(id, side);
	}
	else
	{
		m_targetPosition[unit].clear();
		target = enemy->GetPosition();
	}

	// Stops units getting stuck during path to enemy
	if ((enemy->GetPosition().x - unit->GetPosition().x) < unit->GetBody().getRadius())
	{
		target = enemy->GetPosition();                                                                       //////////////
	}

	sf::Vector2f direction = Normalise(sf::Vector2f(target - unit->GetPosition()));
	unit->SetPosition(unit->GetPosition() + sf::Vector2f(direction.x * unit->GetMovementSpeed(), direction.y * unit->GetMovementSpeed()));

	return 0;
}

int CollisionManager::MoveToStructure(const int id, const Team& side)
{
	Unit* unit;
	Structure* wall;
	sf::Vector2f target;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
	}

	else
	{
		unit = m_rightUnits[id];
	}

	wall = m_targetWall[unit];

	//if (m_targetPosition[unit].empty())
	//{
	//	if (wall->GetPosition().x > unit->GetPosition().x)
	//	{
	//		PlanRouteRight(id, side);
	//	}
	//	else
	//	{
	//		PlanRouteLeft(id, side);
	//	}
	//}

	//if (m_targetPosition[unit].size() > 0)
	//{
	//	target = m_targetPosition[unit].back();
	//	ReachedTarget(id, side);
	//}
	//else
	//{
	//	m_targetPosition[unit].clear();
	//	target = wall->GetPosition();
	//}

	sf::Vector2f direction = Normalise(sf::Vector2f(wall->GetPosition() - unit->GetPosition()));
	unit->SetPosition(unit->GetPosition() + sf::Vector2f(direction.x * unit->GetMovementSpeed(), direction.y * unit->GetMovementSpeed()));

	return 0;
}

int CollisionManager::MoveToBase(const int id, const Team& side)
{
	Unit* unit;
	Structure* base;
	sf::Vector2f target;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
		base = m_leftCastle;

		if (m_targetPosition[unit].empty())
		{
			PlanRouteLeft(id, side);
		}
	}

	else
	{
		unit = m_rightUnits[id];
		base = m_rightCastle;

		if (m_targetPosition[unit].empty())
		{
			PlanRouteRight(id, side);
		}
	}

	if (m_targetPosition[unit].size() > 0)
	{
		target = m_targetPosition[unit].back();
		ReachedTarget(id, side);
	}
	else
	{
		m_targetPosition[unit].clear();
		target = base->GetPosition();
	}

	sf::Vector2f direction = Normalise(sf::Vector2f(target - unit->GetPosition()));
	unit->GetPosition() += sf::Vector2f(direction.x * unit->GetMovementSpeed(), direction.y * unit->GetMovementSpeed());

	return 0;
}

int CollisionManager::MoveToResource(const int id, const Team& side)
{
	Unit* unit;
	Structure* resource;
	sf::Vector2f target;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
	}

	else
	{
		unit = m_rightUnits[id];
	}

	resource = m_targetResource[unit];

	//if (m_targetPosition[unit].empty())
	//{
	//	if (resource->GetPosition().x > unit->GetPosition().x)
	//	{
	//		PlanRouteRight(id, side);
	//	}
	//	else
	//	{
	//		PlanRouteLeft(id, side);
	//	}
	//}

	//if (m_targetPosition[unit].size() > 0)
	//{
	//	target = m_targetPosition[unit].back();
	//	ReachedTarget(id, side);
	//}
	//else
	//{
	//	m_targetPosition[unit].clear();
	//	target = resource->GetPosition();
	//}

	sf::Vector2f direction = Normalise(sf::Vector2f(resource->GetPosition() - unit->GetPosition()));
	unit->SetPosition(unit->GetPosition() + sf::Vector2f(direction.x * unit->GetMovementSpeed(), direction.y * unit->GetMovementSpeed()));

	return 0;
}

int CollisionManager::MoveToSite(const int id, const Team& side)
{
	Unit* unit;
	Structure* site;
	sf::Vector2f target;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
	}

	else
	{
		unit = m_rightUnits[id];
	}

	site = m_targetSite[unit];

	//if (m_targetPosition[unit].empty())
	//{
	//	if (site->GetPosition().x > unit->GetPosition().x)
	//	{
	//		PlanRouteRight(id, side);
	//	}
	//	else
	//	{
	//		PlanRouteLeft(id, side);
	//	}
	//}

	//if (m_targetPosition[unit].size() > 0)
	//{
	//	target = m_targetPosition[unit].back();
	//	ReachedTarget(id, side);
	//}
	//else
	//{
	//	m_targetPosition[unit].clear();
	//	target = site->GetPosition();
	//}

	sf::Vector2f direction = Normalise(sf::Vector2f(site->GetPosition() - unit->GetPosition()));
	unit->SetPosition(unit->GetPosition() + sf::Vector2f(direction.x * unit->GetMovementSpeed(), direction.y * unit->GetMovementSpeed()));

	return 0;
}


//// ENEMY DETECTED


bool CollisionManager::DetectedEnemy(const int id, const Team& side)
{
	Unit* unit;
	std::map<int, Unit*> enemyUnits;

	// Get unit side
	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
		enemyUnits = m_rightUnits;
	}
	else
	{
		unit = m_rightUnits[id];
		enemyUnits = m_leftUnits;
	}

	if (enemyUnits.size() > 0)
	{
		float radius1 = unit->GetFOV().getRadius();
		float radius2 = enemyUnits[0]->GetBody().getRadius();

		// search through enemies
		for (const auto& enemy : enemyUnits)
		{
			if (enemy.second->GetAlive())
			{
				float xd, yd;
				xd = unit->GetPosition().x - enemy.second->GetPosition().x;
				yd = unit->GetPosition().y - enemy.second->GetPosition().y;

				float length = sqrt(xd * xd + yd * yd);

				if (length <= radius1 + radius2)
				{
					return true; // Enemy spotted
				}
			}
		}
	}

	return false;
}

bool CollisionManager::DetectedWall(const int id, const Team& side)
{
	Unit* unit;
	std::vector<Structure*> enemyWalls;

	// Get unit side
	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
		enemyWalls = m_rightWalls;
	}
	else
	{
		unit = m_rightUnits[id];
		enemyWalls = m_leftWalls;
	}

	//float radius1 = unit->GetFOV().getRadius();
	//float radius2 = enemyWalls[0]->GetRadius();

	// search through enemies
	for (const auto& wall : enemyWalls)
	{
		if (wall->GetAlive())
		{
			//float xd, yd;
			//xd = unit->GetPosition().x - wall->GetPosition().x;
			//yd = unit->GetPosition().y - wall->GetPosition().y;

			//float length = sqrt(xd * xd + yd * yd);

			//if (length <= radius1 + radius2)
			//{
			//	return true; // Wall spotted
			//}

			if (wall->GetBody().getGlobalBounds().intersects(unit->GetFOV().getGlobalBounds()))
			{
				return true;
			}
		}
	}

	return false;
}


//// TARGET REACHED


bool CollisionManager::ReachedCastle(const int id, const Team& side)
{
	Unit* unit;
	Structure* castle;

	float radius1, radius2;
	float xd, yd;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
		castle = m_rightCastle;
	}
	else
	{
		unit = m_rightUnits[id];
		castle = m_leftCastle;
	}

	radius1 = unit->GetRange().getRadius();
	radius2 = castle->GetRadius();

	xd = unit->GetPosition().x - castle->GetPosition().x;
	yd = unit->GetPosition().y - castle->GetPosition().y;

	return sqrt(xd * xd + yd * yd) <= radius1 + radius2;

	//return castle->GetBody().getGlobalBounds().intersects(unit->GetRange().getGlobalBounds());
}

bool CollisionManager::ReachedEnemy(const int id, const Team& side)
{
	Unit* unit;
	Unit* enemy;

	float radius1, radius2;
	float xd, yd;

	if (side == Team::LEFT) 
	{ 
		unit = m_leftUnits[id]; 
	}

	else 
	{ 
		unit = m_rightUnits[id]; 
	}

	enemy = m_targetUnit[unit];

	radius1 = unit->GetRange().getRadius();
	radius2 = enemy->GetBody().getRadius();

	xd = unit->GetPosition().x - enemy->GetPosition().x;
	yd = unit->GetPosition().y - enemy->GetPosition().y;

	return sqrt(xd * xd + yd * yd) <= radius1 + radius2;
}

bool CollisionManager::ReachedStructure(const int id, const Team& side)
{
	Unit* unit;
	Structure* wall;

	float radius1, radius2;
	float xd, yd;

	if (side == Team::LEFT) 
	{ 
		unit = m_leftUnits[id]; 
	}

	else 
	{ 
		unit = m_rightUnits[id]; 
	}

	wall = m_targetWall[unit];

	radius1 = unit->GetRange().getRadius();
	radius2 = wall->GetRadius() / 4;

	xd = unit->GetPosition().x - wall->GetPosition().x;
	yd = unit->GetPosition().y - wall->GetPosition().y;

	return sqrt(xd * xd + yd * yd) <= radius1 + radius2;

	//return wall->GetBody().getGlobalBounds().intersects(unit->GetRange().getGlobalBounds());
}


bool CollisionManager::ReachedBase(const int id, const Team& side)
{
	Unit* unit;
	Structure* castle;

	float radius1, radius2;
	float xd, yd;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
		castle = m_leftCastle;
	}
	else
	{
		unit = m_rightUnits[id];
		castle = m_rightCastle;
	}

	radius1 = unit->GetRange().getRadius();
	radius2 = castle->GetRadius();

	xd = unit->GetPosition().x - castle->GetPosition().x;
	yd = unit->GetPosition().y - castle->GetPosition().y;

	return sqrt(xd * xd + yd * yd) <= radius1 + radius2;

	//return castle->GetBody().getGlobalBounds().intersects(unit->GetRange().getGlobalBounds());
}

bool CollisionManager::ReachedResource(const int id, const Team& side)
{
	Unit* unit;
	Structure* resource;

	float radius1, radius2;
	float xd, yd;

	float closestDistanceResource = MAX_LENGHT;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
	}
	else
	{
		unit = m_rightUnits[id];
	}

	resource = m_targetResource[unit];

	radius1 = unit->GetRange().getRadius();
	radius2 = resource->GetRadius();

	xd = unit->GetPosition().x - resource->GetPosition().x;
	yd = unit->GetPosition().y - resource->GetPosition().y;

	return sqrt(xd * xd + yd * yd) <= radius1 + radius2;

	//return resource->GetBody().getGlobalBounds().intersects(unit->GetRange().getGlobalBounds());
}

bool CollisionManager::ReachedSite(const int id, const Team& side)
{
	Unit* unit;
	Structure* site;

	float radius1, radius2;
	float xd, yd;

	if (side == Team::LEFT)
	{
		unit = m_leftUnits[id];
	}
	else
	{
		unit = m_rightUnits[id];
	}

	site = m_targetSite[unit];

	radius1 = unit->GetRange().getRadius();
	radius2 = site->GetRadius() / 4;

	xd = unit->GetPosition().x - site->GetPosition().x;
	yd = unit->GetPosition().y - site->GetPosition().y;

	return sqrt(xd * xd + yd * yd) <= radius1 + radius2;

	//return site->GetBody().getGlobalBounds().intersects(unit->GetRange().getGlobalBounds());
}


//// ATTACK //// 


int CollisionManager::AttackCastle(const int id, const Team& side)
{
	Unit* unit;
	Structure* castle;

	if (side == Team::LEFT) 
	{ 
		unit = m_leftUnits[id]; 
		castle = m_rightCastle; 
	}

	else 
	{ 
		unit = m_rightUnits[id]; 
		castle = m_leftCastle; 
	}

	// Damage
	castle->ReduceHealthBy(unit->GetAttackValue());

	// Death
	if (castle->GetAlive() && castle->GetDefenceValue() <= 0)
	{
		castle->SetAlive(false);

		//if (side == Team::LEFT)
		//{
		//	PlanRouteRight(id, side);
		//}

		//else
		//{
		//	PlanRouteLeft(id, side);
		//}

		return SUCCESS;
	}

	return 0;
}

int CollisionManager::AttackEnemy(const int id, const Team& side)
{
	Unit* unit;
	Unit* enemyUnit;

	if (side == Team::LEFT) 
	{ 
		unit = m_leftUnits[id]; 
	}

	else 
	{ 
		unit = m_rightUnits[id];
	}

	enemyUnit = m_targetUnit[unit];

	// Death
	if (!enemyUnit->GetAlive())
	{
		unit->SetTarget(Target::NO_T);
		unit->SetAttackTimer(0);

		//if (side == Team::LEFT)
		//{
		//	PlanRouteRight(id, side);
		//}

		//else
		//{
		//	PlanRouteLeft(id, side);
		//}
	
		return SUCCESS;
	}

	// Damage for queue
	m_targetUnitDamage[enemyUnit] += unit->GetAttackValue();

	return 0;
}

int CollisionManager::AttackStructure(const int id, const Team& side)
{
	Unit* unit;
	Structure* enemyWall;

	if (side == Team::LEFT) 
	{ 
		unit = m_leftUnits[id];
	}

	else 
	{ 
		unit = m_rightUnits[id]; 
	}

	enemyWall = m_targetWall[unit];

	// Damage
	if (unit->GetType() == UnitType::ARCHER)
	{
		enemyWall->ReduceHealthBy(unit->GetAttackValue() / 2); // Archers do half damage
	}
	else
	{
		enemyWall->ReduceHealthBy(unit->GetAttackValue());
	}

	// Death
	if (enemyWall->GetAlive() && enemyWall->GetDefenceValue() <= 0)
	{
		enemyWall->SetAlive(false);
	}

	// Win
	if (!enemyWall->GetAlive())
	{
		unit->SetTarget(Target::NO_T);
		unit->SetAttackTimer(0);

		//if (side == Team::LEFT)
		//{
		//	PlanRouteRight(id, side);
		//}

		//else
		//{
		//	PlanRouteLeft(id, side);
		//}

		return SUCCESS;
	}

	return 0;
}


//// WORK ////


int CollisionManager::GatherResource(const int id, const Team& side)
{
	Unit* unit;
	Structure* resource;

	if (side == Team::LEFT) 
	{ 
		unit = m_leftUnits[id]; 
	}

	else 
	{ 
		unit = m_rightUnits[id]; 
	}

	resource = m_targetResource[unit];

	// Damage
	if (resource->GetAlive() && resource->GetDefenceValue() > 0)
	{
		if (side == Team::LEFT)
		{
			m_leftGold += resource->GetValue();
		}
		else
		{
			m_rightGold += resource->GetValue();
		}

		resource->ReduceHealthBy(unit->GetAttackValue() * 2.5); // damage against resource increased
	}

	// Death
	if (resource->GetAlive() && resource->GetDefenceValue() <= 0)
	{
		resource->SetAlive(false);
	}

	// Win
	if (!resource->GetAlive())
	{
		unit->SetTarget(Target::NO_T);
		unit->SetAttackTimer(0);

		//if (side == Team::LEFT)
		//{
		//	PlanRouteRight(id, side);
		//	m_leftResourcesTotal++;
		//}

		//else
		//{
		//	PlanRouteLeft(id, side);
		//	m_rightResourcesTotal++;
		//}

		return SUCCESS;
	}

	return 0;
}

int CollisionManager::BuildStructure(const int id, const Team& side)
{
	Unit* unit;
	Structure* site;

	if (side == Team::LEFT) 
	{ 
		unit = m_leftUnits[id]; 
	}
	else 
	{ 
		unit = m_rightUnits[id]; 
	}

	site = m_targetSite[unit];

	if (!site->GetAlive())
	{
		site->IncreaseHealthBy(unit->GetAttackValue() * 4); // Build faster
	}

	if (site->GetDefenceValue() >= WALL_HP)
	{
		site->SetDefenceValue(WALL_HP);
		site->SetAlive(true);
		site->SetBuilding(false);

		if (side == Team::LEFT)
		{
			//PlanRouteRight(id, side);
			m_leftWallsTotal++;
		}

		else
		{
			//PlanRouteLeft(id, side);
			m_rightWallsTotal++;
		}

		return SUCCESS;
	}

	return 0;
}


//////////////
//// MCTS ////
//////////////


int CollisionManager::MCTS_Idle()
{
	if (m_leftCastle->GetAlive() || m_leftUnits.size() > 0)
	{
		return FAIL; // should not stay idle
	}

	return 0; // No success state
}

int CollisionManager::MCTS_MoveToCastle(const int id)
{
	float radius1 = m_rightUnits[id]->GetFOV().getRadius();
	float radius2 = (m_rightCastle->GetSize().x + m_rightCastle->GetSize().y) / 4;
	float xd, yd;

	xd = m_leftCastle->GetPosition().x - m_rightUnits[id]->GetPosition().x;
	yd = m_leftCastle->GetPosition().y - m_rightUnits[id]->GetPosition().y;

	if (sqrt(xd * xd + yd * yd) <= radius1 + radius2)
	{
		return SUCCESS; // should move to spotted castle
	}

	return FAIL; // Can't see castle fail
}

int CollisionManager::MCTS_AttackCastle(const int id)
{
	if (m_leftCastle->GetAlive())
	{
		return SUCCESS; // castle is alive
	}

	return FAIL; // castle dead
}

int CollisionManager::MCTS_CastleDead()
{
	if (m_leftCastle->GetAlive())
	{
		return FAIL; // castle is alive
	}

	return SUCCESS; // success state - castle dead
}

int CollisionManager::MCTS_MoveToEnemy(const int id)
{
	if (m_targetUnit[m_rightUnits[id]] == nullptr)
	{
		return FAIL; // has no target
	}

	Unit* unit2 = m_targetUnit[m_rightUnits[id]];

	float radius1 = m_rightUnits[id]->GetFOV().getRadius();
	float radius2 = unit2->GetBody().getRadius();
	float xd, yd;

	xd = unit2->GetPosition().x - m_rightUnits[id]->GetPosition().x;
	yd = unit2->GetPosition().y - m_rightUnits[id]->GetPosition().y;

	if (sqrt(xd * xd + yd * yd) <= radius1 + radius2)
	{
		return SUCCESS; // should move to spotted enemy
	}

	return FAIL; // Can't see enemy fail
}

int CollisionManager::MCTS_AttackEnemy(const int id)
{
	if (m_rightUnits[id]->GetTarget() == Target::ENEMY_T)
	{
		if (m_targetUnit[m_rightUnits[id]]->GetAlive())
		{
			return SUCCESS; // enemy is alive
		}
	}

	return FAIL; // enemy dead
}

int CollisionManager::MCTS_EnemyDead(const int id)
{
	if (m_rightUnits[id]->GetTarget() == Target::ENEMY_T)
	{
		if (m_targetUnit[m_rightUnits[id]]->GetAlive())
		{
			return FAIL; // enemy is alive
		}
	}

	return SUCCESS; // enemy dead
}

int CollisionManager::MCTS_MoveToStructure(const int id)
{
	if (m_targetWall[m_rightUnits[id]] == nullptr)
	{
		return FAIL; // has no target
	}

	Structure* wall = m_targetWall[m_rightUnits[id]];

	float radius1 = m_rightUnits[id]->GetFOV().getRadius();
	float radius2 = wall->GetRadius();
	float xd, yd;

	xd = wall->GetPosition().x - m_rightUnits[id]->GetPosition().x;
	yd = wall->GetPosition().y - m_rightUnits[id]->GetPosition().y;

	if (sqrt(xd * xd + yd * yd) <= radius1 + radius2)
	{
		return SUCCESS; // should move to spotted enemy
	}

	//if (m_leftCastle->GetBody().getGlobalBounds().intersects(m_rightUnits[id]->GetFOV().getGlobalBounds()))
	//{
	//	return SUCCESS;
	//}

	return FAIL; // Can't see enemy fail
}

int CollisionManager::MCTS_AttackStructure(const int id)
{
	if (m_rightUnits[id]->GetTarget() == Target::WALL_T)
	{
		if (m_targetWall[m_rightUnits[id]]->GetAlive())
		{
			return SUCCESS; // wall is alive
		}
	}

	return FAIL; // wall dead
}

int CollisionManager::MCTS_StructureDead(const int id)
{
	if (m_rightUnits[id]->GetTarget() == Target::WALL_T)
	{
		if (m_targetWall[m_rightUnits[id]]->GetAlive())
		{
			return FAIL; // enemy is alive
		}
	}

	return SUCCESS; // enemy dead
}

int CollisionManager::MCTS_MoveToBase()
{
	float radius1 = (m_rightCastle->GetSize().x + m_rightCastle->GetSize().y) / 4;

	for (const auto& enemy : m_leftUnits)
	{
		if (enemy.second->GetAlive())
		{
			float radius2 = enemy.second->GetRange().getRadius() * 2;
			float xd, yd;

			xd = m_rightCastle->GetPosition().x - enemy.second->GetPosition().x;
			yd = m_rightCastle->GetPosition().y - enemy.second->GetPosition().y;

			if (sqrt(xd * xd + yd * yd) <= radius1 + radius2)
			{
				return SUCCESS; // base is under attack
			}
		}
	}

	return FAIL; // base is safe
}

int CollisionManager::MCTS_BaseReached(const int id)
{
	float radius1 = (m_rightCastle->GetSize().x + m_rightCastle->GetSize().y) / 4;
	float radius2 = m_rightUnits[id]->GetRange().getRadius() * 2;

	float xd, yd;

	xd = m_rightCastle->GetPosition().x - m_rightUnits[id]->GetPosition().x;
	yd = m_rightCastle->GetPosition().y - m_rightUnits[id]->GetPosition().y;

	if (sqrt(xd * xd + yd * yd) <= radius1 + radius2)
	{
		return SUCCESS; // base is under attack
	}

	return FAIL; // base is safe
}

int CollisionManager::MCTS_MoveToResource(const int id)
{
	float radius1 = m_rightUnits[id]->GetFOV().getRadius() * 2;
	float radius2 = (m_rightResources[0]->GetSize().x + m_rightResources[0]->GetSize().y) / 4;

	for (const auto& piece : m_rightResources)
	{
		if (piece->GetAlive())
		{
			float xd, yd;

			xd = piece->GetPosition().x - m_rightUnits[id]->GetPosition().x;
			yd = piece->GetPosition().y - m_rightUnits[id]->GetPosition().y;

			if (sqrt(xd * xd + yd * yd) <= radius1 + radius2)
			{
				return SUCCESS; // base is under attack
			}
		}
	}

	return FAIL; // base is safe
}

int CollisionManager::MCTS_GatherResources(const int id)
{
	if (m_rightUnits[id]->GetTarget() == Target::RESOURCE_T)
	{
		if (m_targetResource[m_rightUnits[id]]->GetAlive())
		{
			return SUCCESS; // resource available to gather
		}
	}

	return FAIL; // resource gone
}

int CollisionManager::MCTS_ResourcesCollected(const int id)
{
	if (m_rightUnits[id]->GetTarget() == Target::RESOURCE_T)
	{
		if (m_targetResource[m_rightUnits[id]]->GetAlive())
		{
			return FAIL; // resource available
		}
	}

	return SUCCESS; // resource gone
}

int CollisionManager::MCTS_MoveToSite(const int id)
{
	float radius1 = m_rightUnits[id]->GetRange().getRadius() * 2;
	float radius2 = m_rightWalls[0]->GetRadius() / 4;

	for (const auto& wall : m_rightWalls)
	{
		if (!wall->GetAlive())
		{
			float xd, yd;

			xd = wall->GetPosition().x - wall->GetPosition().x;
			yd = wall->GetPosition().y - wall->GetPosition().y;

			if (sqrt(xd * xd + yd * yd) <= radius1 + radius2)
			{
				return SUCCESS; // base is under attack
			}

			//if (wall->GetBody().getGlobalBounds().intersects(m_rightUnits[id]->GetRange().getGlobalBounds()))
			//{
			//	return SUCCESS;
			//}
		}
	}

	return FAIL; // base is safe
}

int CollisionManager::MCTS_BuildStructure(const int id)
{
	if (m_rightUnits[id]->GetTarget() == Target::SITE_T)
	{
		if (m_targetSite[m_rightUnits[id]]->GetAlive())
		{
			return FAIL; // wall is alive
		}
	}

	return SUCCESS; // wall dead - should build
}

int CollisionManager::MCTS_StuctureBuilt(const int id)
{
	if (m_rightUnits[id]->GetTarget() == Target::SITE_T)
	{
		if (m_targetSite[m_rightUnits[id]]->GetAlive())
		{
			return SUCCESS; // wall is alive
		}
	}

	return FAIL; // wall dead
}


/////////////////////////////
//// GETTERS AND SETTERS ////
/////////////////////////////


int CollisionManager::GetGold(const Team& side) const
{
	if (side == Team::LEFT)
	{
		return m_leftGold;
	}
	else
	{
		return m_rightGold;
	}
}

void CollisionManager::SpendGold(const Team& side, const int cost)
{
	if (side == Team::LEFT)
	{
		m_leftGold -= cost;
	}
	else
	{
		m_rightGold -= cost;
	}
}

float CollisionManager::GetSideValue(const Team& side) const
{
	if (side == Team::LEFT)
	{
		return m_leftSideValue;
	}
	else
	{
		return m_rightSideValue;
	}
}

float CollisionManager::GetAttackValue(const Team& side) const
{
	if (side == Team::LEFT)
	{
		return m_leftAttackValue;
	}
	else
	{
		return m_rightAttackValue;
	}
}

float CollisionManager::GetDefenceValue(const Team& side) const
{
	if (side == Team::LEFT)
	{
		return m_leftDefenceValue;
	}
	else
	{
		return m_rightDefenceValue;
	}
}

float CollisionManager::GetUnitCount(const Team& side, const UnitType& type) const
{
	if (side == Team::LEFT)
	{
		if (type == UnitType::BUILDER)
		{
			return m_leftBuilders;
		}
		else if (type == UnitType::SOLDIER)
		{
			return m_leftSoldiers;
		}
		else
		{
			return m_leftArchers;
		}
	}
	else
	{
		if (type == UnitType::BUILDER)
		{
			return m_rightBuilders;
		}
		else if (type == UnitType::SOLDIER)
		{
			return m_rightSoldiers;
		}
		else
		{
			return m_rightArchers;
		}
	}
}

float CollisionManager::GetWallCount(const Team& side) const
{
	if (side == Team::LEFT)
	{
		return m_leftWallsBuilt;
	}
	else
	{
		return m_rightWallsBuilt;
	}
}

float CollisionManager::GetWeight(const WorldState& worldState) const
{
	if (worldState == WorldState::Engage_Castle)
	{
		return w_engageCastle;
	}

	else if (worldState == WorldState::Engage_Enemies)
	{
		return w_engageEnemy;
	}

	else if (worldState == WorldState::Engage_Structure)
	{
		return w_engageStructure;
	}

	else if (worldState == WorldState::Defend_Castle)
	{
		return w_defendCastle;
	}

	else if (worldState == WorldState::Need_Resources)
	{
		return w_needResources;
	}
}


