//Frankie Lyons
//C00177077
//SAT Assignment

#include "Triangles.h"


Triangles::Triangles()
{
}


Triangles::~Triangles()
{
}

const sf::CircleShape Triangles::getTriangle()
{
	return m_triangle;
}

void Triangles::setTriangle(float radius)
{
	m_triangle = sf::CircleShape(radius, 3);
	m_triangle.setRadius(radius);
}

const sf::Vector2f Triangles::getPosition()
{
	return m_position;
}

void Triangles::setPosition(sf::Vector2f position)
{
	m_position = position;
}

const sf::Vector2f Triangles::getVelocity()
{
	return m_velocity;
}

void Triangles::setVelocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
}

const float Triangles::getSize()
{
	return m_size;
}

const float Triangles::getRadius()
{
	return m_radius;
}

void Triangles::setSize(float size)
{
	m_size = size;
	m_radius = size / 2;
}

void Triangles::update(){
	m_position += m_velocity;
	m_triangle.setPosition(sf::Vector2f(m_position.x - m_radius, m_position.y - m_radius));

	if (m_position.x >= 1280 - m_radius || m_position.x <= 0 + m_radius) {
		m_velocity.x *= -1;
	}
	if (m_position.y >= 720 - m_radius || m_position.y <= 0 + m_radius) {
		m_velocity.y *= -1;
	}

	if (m_position.x > 1280 - m_radius) { m_position.x = 1280 - m_radius; }
	if (m_position.x < 0 + m_radius) { m_position.x = m_radius; }

	if (m_position.y > 720 - m_radius) { m_position.y = 720 - m_radius; }
}
