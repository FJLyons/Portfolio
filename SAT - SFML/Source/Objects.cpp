//Frankie Lyons
//C00177077
//SAT Assignment

#include "Objects.h"


Objects::Objects()
{
	m_pointAngle = 0;
	m_point = sf::Vector2f(0, 0);

	m_rotationAngle = rand() % 360;

	//m_size = rand() % 25 + 25;
	//m_position = (sf::Vector2f(rand() % 1180 + 50, rand() % 620 + 50));
	//m_velocity = (sf::Vector2f((rand() % 5 - 2.5f), (rand() % 5 - 2.5f)));
	//setShape(rand() % 2 + 3);
}


Objects::~Objects()
{
}

const sf::ConvexShape Objects::getShape()
{
	return m_shape;
}

void Objects::setShape(int pointcount)
{
	m_shape.setPointCount(pointcount);

	for (int i = 0; i < pointcount; i++)
	{
		m_pointAngle = i * (M_PI * 2) / pointcount;
		m_point = m_position + sf::Vector2f(cos(m_pointAngle) * m_size, sin(m_pointAngle) * m_size);
		m_shape.setPoint(i, m_point);
		m_vertices.push_back(m_point);
	}
}

const sf::Vector2f Objects::getPosition()
{
	return m_position;
}

void Objects::setPosition(sf::Vector2f position)
{
	m_position = position;
}

const sf::Vector2f Objects::getVelocity()
{
	return m_velocity;
}

void Objects::setVelocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
}

const float Objects::getSize()
{
	return m_size;
}

void Objects::setSize(float size)
{
	m_size = size;
}

void Objects::update(){
	m_position += m_velocity;	
	rotate();
}

void Objects::rotate()
{
	m_rotationAngle += 1.0f / 60.0f;

	m_vertices.clear();

	for (int i = 0; i < m_shape.getPointCount(); i++)
	{
		m_pointAngle = i * (M_PI * 2) / m_shape.getPointCount();
		m_point = m_position + sf::Vector2f(cos(m_pointAngle + m_rotationAngle) * m_size, sin(m_pointAngle + m_rotationAngle) * m_size);
		m_shape.setPoint(i, m_point);
		m_vertices.push_back(m_point);
	}

	if (m_rotationAngle >= 360) { m_rotationAngle = 0; }
}

const std::vector<sf::Vector2f> Objects::getAxis()
{
	int pointCount = m_shape.getPointCount();
	std::vector<sf::Vector2f> m_axes;

	for (int i = 0; i < pointCount; i++)
	{
		if (i + 1 < pointCount)
		{
			sf::Vector2f axis(m_shape.getPoint(i) - m_shape.getPoint(i + 1));
			m_axes.push_back(sf::Vector2f(axis.y, -(axis.x)));
		}

		else
		{ 
			sf::Vector2f axis(m_shape.getPoint(i) - m_shape.getPoint(0));
			m_axes.push_back(sf::Vector2f(axis.y, -(axis.x)));
		}
	}

	return m_axes;
}

const std::vector<sf::Vector2f> Objects::getVerts()
{
	return  m_vertices;
}