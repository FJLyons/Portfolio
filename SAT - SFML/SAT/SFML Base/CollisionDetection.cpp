//Frankie Lyons
//C00177077
//SAT Assignment

#include "CollisionDetection.h"


CollisionDetection::CollisionDetection()
{
}


CollisionDetection::~CollisionDetection()
{
}

bool CollisionDetection::borderCollision(Objects* shape)
{
	if (shape->getPosition().x >= 1280 - shape->getSize()|| shape->getPosition().x <= 0 + shape->getSize()) {
		shape->setVelocity((sf::Vector2f(-(shape->getVelocity().x), shape->getVelocity().y)));
	}
	if (shape->getPosition().y >= 720 - shape->getSize()|| shape->getPosition().y <= 0 + shape->getSize()) {
		shape->setVelocity((sf::Vector2f(shape->getVelocity().x, -(shape->getVelocity().y))));
	}

	if (shape->getPosition().x > 1280 - shape->getSize()) { shape->setPosition(sf::Vector2f(1280 - shape->getSize(), shape->getPosition().y)); }
	if (shape->getPosition().x < 0 + shape->getSize()) { shape->setPosition(sf::Vector2f(shape->getSize(), shape->getPosition().y)); }

	if (shape->getPosition().y > 720 - shape->getSize()) { shape->setPosition(sf::Vector2f(shape->getPosition().x, 720 - shape->getSize())); }
	if (shape->getPosition().y < 0 + shape->getSize()) { shape->setPosition(sf::Vector2f(shape->getPosition().x, shape->getSize())); }

	return false;
}

bool CollisionDetection::circleCollision(Objects* shape1, Objects* shape2){
	float xd = shape1->getPosition().x - shape2->getPosition().x;
	float yd = shape1->getPosition().y - shape2->getPosition().y;

	float sumRadius = (shape1->getSize()) + (shape2->getSize());
	float sqrRadius = sumRadius * sumRadius;

	float distSqr = (xd * xd) + (yd * yd);

	if (distSqr <= sqrRadius)
	{
		return true;
	}

	return false;
}

void CollisionDetection::circleResolve(Objects* shape1, Objects* shape2){
	// get the minimum translation distance
	sf::Vector2f collisionVector = shape2->getPosition() - shape1->getPosition();

	float distance = sqrt(collisionVector.x * collisionVector.x + collisionVector.y * collisionVector.y);

	float dotA = (shape1->getVelocity().x * collisionVector.x) + (shape1->getVelocity().y * collisionVector.y);
	float dotB = (shape2->getVelocity().x * collisionVector.x) + (shape2->getVelocity().y * collisionVector.y);

	float scalerA = dotA / distance;
	float scalerB = dotB / distance;

	sf::Vector2f velAlongColA = scalerA * (collisionVector / distance);
	sf::Vector2f velAlongColB = scalerB * (collisionVector / distance);

	shape1->setVelocity(shape1->getVelocity() - velAlongColA + velAlongColB);
	shape2->setVelocity(shape2->getVelocity() - velAlongColB + velAlongColA);

	sf::Vector2f move = ((shape1->getSize()) + (shape2->getSize()) - distance) * (collisionVector / distance);
	shape1->setPosition(shape1->getPosition() - move);
	shape2->setPosition(shape2->getPosition() + move);
}