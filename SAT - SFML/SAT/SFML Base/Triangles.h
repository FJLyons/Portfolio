//Frankie Lyons
//C00177077
//SAT Assignment

#pragma once

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>

class Triangles
{
public:
	Triangles();
	~Triangles();

	const sf::CircleShape getTriangle();
	void setTriangle(float radius);

	const sf::Vector2f getPosition();
	void setPosition(sf::Vector2f position);

	const sf::Vector2f getVelocity();
	void setVelocity(sf::Vector2f velocity);

	const float getSize();
	void setSize(float size);

	const float getRadius();

	void update();

private:
	// define a triangle
	sf::CircleShape m_triangle;

	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_size;
	float m_radius;
};

