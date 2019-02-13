#include "CollisionManager.h"

CollisionManager *CollisionManager::_instance = 0;

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
	delete _instance;
}

CollisionManager * CollisionManager::instance()
{

	if (_instance == nullptr)
	{//if our instance hasn't been initialized
	 //initialize it
		_instance = new CollisionManager();
	}
	//return the instance.
	return _instance;
}

bool CollisionManager::CircleDetectCollision(sf::CircleShape &s1, sf::CircleShape &s2) {


	float distanceX = (s1.getPosition().x) - (s2.getPosition().x);
	float distanceY = (s1.getPosition().y) - (s2.getPosition().y);
	float distance = sqrt((distanceX * distanceX) + (distanceY * distanceY));

	if (distance < (s1.getRadius() + s2.getRadius())) {
		return true;
	}
	else {
		return false;
	}
}

bool CollisionManager::CheckRange(float range, sf::Vector2f object1, sf::Vector2f object2)
{
	float distanceX = object1.x - object2.x;
	float distanceY = object1.y - object2.y;
	float distance = sqrt((distanceX * distanceX) + (distanceY * distanceY));

	if (distance <= range)
	{
		return true;
	}
	else
		return false;
}

bool CollisionManager::RectangleCollision(sf::RectangleShape  s1, sf::RectangleShape  s2)
{
	float s1X = s1.getPosition().x - s1.getGlobalBounds().width / 2;
	float s1Y = s1.getPosition().y - s1.getGlobalBounds().height / 2;

	float s2X = s2.getPosition().x - s2.getGlobalBounds().width / 2;
	float s2Y = s2.getPosition().y - s2.getGlobalBounds().height / 2;

	if (s1X < s2X + s2.getGlobalBounds().width &&
		s1X + s1.getGlobalBounds().width > s2X &&
		s1Y < s2Y + s2.getGlobalBounds().height &&
		s1.getGlobalBounds().height + s1Y >s2Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}


float CollisionManager::dotProduct(sf::Vector2f v1, sf::Vector2f v2) {

	float answer((v1.x * v2.x) + (v1.y * v2.y));
	return answer;
}
sf::Vector2f CollisionManager::NormaliseVector(sf::Vector2f v1) {

	float TempLength(sqrt((v1.x * v1.x) + (v1.y * v1.y)));
	if (TempLength != 0)
	{
		sf::Vector2f normalised((v1.x / TempLength), (v1.y / TempLength));
		return normalised;
	}

	return sf::Vector2f(0, 0);

}