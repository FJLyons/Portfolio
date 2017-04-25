//Frankie Lyons
//C00177077
//SAT Assignment

#pragma once

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>

class Objects
{
public:
	Objects();
	~Objects();

	const sf::ConvexShape getShape();
	void setShape(int pointcount);

	const sf::Vector2f getPosition();
	void setPosition(sf::Vector2f position);

	const sf::Vector2f getVelocity();
	void setVelocity(sf::Vector2f velocity);

	const std::vector<sf::Vector2f> getAxis();
	const std::vector<sf::Vector2f> getVerts();

	const float getSize();
	void setSize(float size);

	void update();

	void rotate();

private:
	// define a shape
	sf::ConvexShape  m_shape;

	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_size;

	float m_pointAngle;
	sf::Vector2f m_point;

	float m_rotationAngle;

	std::vector<sf::Vector2f> m_vertices;
};