#include "Structure.h"



Structure::Structure(const Team& side, const StructureType& type) : m_side(side), m_type(type)
{
	InitCastle();

	m_text.setString("");
	m_text.setFont(ResourceManager::GetInstance()->FONT);
	m_text.setPosition(m_position);
	m_text.setCharacterSize(18);
	if (m_side == Team::LEFT)
	{
		m_text.setOutlineColor(sf::Color::Blue);
	}
	else if (m_side == Team::RIGHT)
	{
		m_text.setOutlineColor(sf::Color::Red);
	}
	m_text.setOutlineThickness(1);
}

Structure::Structure(const Team& side, const StructureType& type, const sf::Vector2f& position) : m_side(side), m_type(type), m_position(position)
{
	if (m_type == StructureType::WALL) 
	{ 
		InitWall(); 

		m_text.setString("");
		m_text.setFont(ResourceManager::GetInstance()->FONT);
		m_text.setPosition(m_position);
		m_text.setCharacterSize(18);
		if (m_side == Team::LEFT)
		{
			m_text.setOutlineColor(sf::Color::Blue);
		}
		else if (m_side == Team::RIGHT)
		{
			m_text.setOutlineColor(sf::Color::Red);
		}
		m_text.setOutlineThickness(1);
	}

	else if (m_type == StructureType::RESOURCE) 
	{ 
		InitResource(); 
	}
}


Structure::~Structure()
{
}

void Structure::Init()
{

}

void Structure::Update()
{
}

void Structure::Draw(sf::RenderWindow &window, int debugLevel)
{
	window.draw(m_rectangleShape);

	if (debugLevel > DEBUG_NONE)
	{
		m_text.setString(std::to_string((int)m_valueDefense));
		m_text.setOrigin(m_text.getLocalBounds().width / 2.0f, m_text.getLocalBounds().height / 2.0f);
		window.draw(m_text);
	}
}

void Structure::InitCastle()
{
	alive = true;

	m_size = sf::Vector2f(CASTLE_BODY_SIZE, CASTLE_BODY_SIZE);
	m_sizeHalf = sf::Vector2f(m_size.x / 2, m_size.y / 2);

	m_rectangleShape = sf::RectangleShape();

	if (m_side == Team::LEFT)
	{
		m_position = LEFTCASTLE;
		m_rectangleShape.setTexture(&ResourceManager::GetInstance()->t_blue_castle);
		//m_rectangleShape.setOutlineColor(sf::Color::Blue);
	}

	else
	{
		m_position = RIGHTCASTLE;
		m_rectangleShape.setTexture(&ResourceManager::GetInstance()->t_red_castle);
		//m_rectangleShape.setOutlineColor(sf::Color::Red);
	}

	m_rectangleShape.setSize(m_size);
	m_rectangleShape.setOrigin(m_sizeHalf);
	m_rectangleShape.setPosition(m_position);
	//m_rectangleShape.setFillColor(Grey);
	//m_rectangleShape.setOutlineThickness(2.0f);

	m_valueDefense = 250;
	m_value = 250;
}

void Structure::InitWall()
{
	alive = false;
	m_building = false;

	m_size = sf::Vector2f(WALL_WIDTH, WALL_LENGTH);
	m_sizeHalf = sf::Vector2f(m_size.x / 2, m_size.y / 2);

	m_rectangleShape = sf::RectangleShape();

	if (m_side == Team::LEFT)
	{
		m_rectangleShape.setTexture(&ResourceManager::GetInstance()->t_blue_wall);
		//m_rectangleShape.setOutlineColor(sf::Color::Blue);
	}

	else
	{
		m_rectangleShape.setTexture(&ResourceManager::GetInstance()->t_red_wall);
		//m_rectangleShape.setOutlineColor(sf::Color::Red);
	}

	m_rectangleShape.setSize(m_size);
	m_rectangleShape.setOrigin(m_sizeHalf);
	m_rectangleShape.setPosition(m_position);
	//m_rectangleShape.setFillColor(Grey);
	//m_rectangleShape.setOutlineThickness(2.0f);

	m_valueDefense = 0;
	m_value = WALL_VALUE;
}

void Structure::InitResource()
{
	alive = true;

	m_size = sf::Vector2f(19.5, 19.5);
	m_sizeHalf = sf::Vector2f(m_size.x / 2, m_size.y / 2);

	m_rectangleShape = sf::RectangleShape();

	if (m_side == Team::LEFT)
	{
		m_rectangleShape.setTexture(&ResourceManager::GetInstance()->t_blue_resource);
	}

	else
	{
		m_rectangleShape.setTexture(&ResourceManager::GetInstance()->t_red_resource);
	}

	m_rectangleShape.setSize(m_size);
	m_rectangleShape.setOrigin(m_sizeHalf);
	m_rectangleShape.setPosition(m_position);
	//m_rectangleShape.setFillColor(Grey);
	//m_rectangleShape.setOutlineColor(sf::Color::Yellow);
	//m_rectangleShape.setOutlineThickness(2.0f);

	m_valueDefense = 50;
	m_value = 5;
}

bool Structure::GetAlive() const
{
	return alive;
}

void Structure::SetAlive(const bool isAlive)
{
	alive = isAlive;
}

bool Structure::GetBuilding() const
{
	return m_building;
}

void Structure::SetBuilding(const bool isBuilding)
{
	m_building = isBuilding;
}

sf::Vector2f Structure::GetPosition() const
{
	return m_position;
}

void Structure::SetPosition(const sf::Vector2f & newPosition)
{
	m_position = newPosition;
}

float Structure::GetValue() const
{
	return m_value;
}

float Structure::GetDefenceValue() const
{
	return m_valueDefense;
}

void Structure::SetDefenceValue(const float newHealth)
{
	m_valueDefense = newHealth;
}

void Structure::ReduceHealthBy(const float damage)
{
	m_valueDefense -= damage;
}

void Structure::IncreaseHealthBy(const float work)
{
	m_valueDefense += work;
}

sf::Vector2f Structure::GetSize() const
{
	return m_size;
}

sf::RectangleShape Structure::GetBody() const
{
	return m_rectangleShape;
}

float Structure::GetRadius() const
{
	return (m_size.x + m_size.y) / 4;
}
