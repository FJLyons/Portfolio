//Frankie Lyons
//C00177077
//SAT Assignment


#include "SATCollisions.h"


SATCollisions::SATCollisions()
{
}


SATCollisions::~SATCollisions()
{
}

float SATCollisions::dotProduct(sf::Vector2f point, sf::Vector2f normalAxis)
{
	float dot = (point.x * normalAxis.x) + (point.y * normalAxis.y);

	return dot;
}


sf::Vector2f SATCollisions::projectAxis(Objects* shape, sf::Vector2f axis)
{
	std::vector<float> projections;
	sf::Vector2f minMax;

	for (int i = 0; i < shape->getVerts().size(); i++)
	{
		projections.push_back(dotProduct(shape->getVerts()[i], axis));
	}

	minMax = sf::Vector2f(projections.at(0), projections.at(0));

	for (int i = 0; i < projections.size(); i++)
	{
		if (projections.at(i) < minMax.x)
		{
			minMax.x = projections.at(i);
		}

		else if (projections.at(i) > minMax.y)
		{
			minMax.y = projections.at(i);
		}
	}

	return minMax;
}

bool SATCollisions::gapDetection(Objects* shape1, Objects* shape2)
{
	float mtd;
	sf::Vector2f mtv;

	sf::Vector2f minMax1, minMax2;

	mtd = std::numeric_limits<float>::max();		// mtd start

	//float distance;

	float temp;

	std::vector<sf::Vector2f> axes;

	axes = shape1->getAxis();

	for (int i = 0; i < shape2->getAxis().size(); i++)
	{
		axes.push_back(shape2->getAxis()[i]);
	}

	for (int i = 0; i < axes.size(); i++)
	{
		sf::Vector2f axis = axes[i];

		// normalize the axis
		temp = sqrt(axis.x * axis.x + axis.y * axis.y);
		axis /= temp;

		minMax1 = projectAxis(shape1, axis);

		minMax2 = projectAxis(shape2, axis);

		// test for gap
		if (minMax1.y < minMax2.x || minMax1.x > minMax2.y)
		{
			return false;
		}

		float overlap;
		if (minMax1.y < minMax2.y)
		{
			overlap = minMax1.y - minMax2.x;
		}
		else { overlap = minMax2.y - minMax1.x; }

		if (overlap < mtd)
		{
			mtd = overlap;
			mtv = axes.at(i);
		}
	}

	SATResolve(shape1, shape2, mtv, mtd);

	return true;
}

void SATCollisions::SATResolve(Objects* shape1, Objects* shape2, sf::Vector2f mtv, float mtd){
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

	mtv = mtv / sqrt(mtv.x * mtv.x + mtv.y * mtv.y);

	sf::Vector2f move = (mtv * mtd);

	if (dotProduct(shape2->getPosition() - shape1->getPosition(), mtv) > 0.0f)
	{
		shape1->setPosition(shape1->getPosition() - move);
		shape2->setPosition(shape2->getPosition() + move);
	}

	else 
	{
		shape1->setPosition(shape1->getPosition() + move);
		shape2->setPosition(shape2->getPosition() - move);
	}
}