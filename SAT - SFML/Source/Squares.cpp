//Frankie Lyons
//C00177077
//SAT Assignment

#include "Squares.h"


Squares::Squares()
{
}


Squares::~Squares()
{
}

const sf::CircleShape Squares::getSquare()
{
	return m_square;
}

void Squares::setSquare(float radius)
{
	m_square = sf::CircleShape(radius, 4);
	m_square.setRadius(radius);
}

const sf::Vector2f Squares::getPosition()
{
	return m_position;
}

void Squares::setPosition(sf::Vector2f position)
{
	m_position = position;
}

const sf::Vector2f Squares::getVelocity()
{
	return m_velocity;
}

void Squares::setVelocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
}

const float Squares::getSize()
{
	return m_size;
}

const float Squares::getRadius()
{
	return m_radius;
}

void Squares::setSize(float size)
{
	m_size = size;
	m_radius = size / 2;
	m_square.setOrigin(m_radius, m_radius);
}

void Squares::update(){
	m_position += m_velocity;
	m_square.setPosition(sf::Vector2f(m_position.x - m_radius, m_position.y - m_radius));

	if (m_position.x >= 1280 - m_radius || m_position.x <= 0 + m_radius) {
		m_velocity.x *= -1;
	}
	if (m_position.y >= 720 - m_radius || m_position.y <= 0 + m_radius) {
		m_velocity.y *= -1;
	}

	if (m_position.x > 1280 - m_radius) { m_position.x = 1280 - m_radius; }
	if (m_position.x < 0 + m_radius) { m_position.x = m_radius; }

	if (m_position.y > 720 - m_radius) { m_position.y = 720 - m_radius; }


	m_square.rotate(1.0f);
}

