

#include "Abductor.h"

Abductor::Abductor()
{
}
Abductor::~Abductor()
{

}
Abductor::Abductor(sf::Vector2f pos)
{

	mTexture = ResourceLoader::instance()->getabductorTexture();
	bulletTexture = ResourceLoader::instance()->getbulletTexture();
	mPositon = pos;
	velocity = sf::Vector2f(10, 0.5);
	mSprite.setTexture(mTexture);
	mSprite.setPosition(mPositon);
	mSprite.setOrigin(sf::Vector2f(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2));
	currentBehaviour = Behaviour::Wander;
	abducting = false;
	range = 700;
	alive = true;
	seekPoint = FindNewPoint();
	///seekPoint = sf::Vector2f(300, 550);

	canShoot = true;
	shotTimer = rand() % (5) + 1;
	shotdelay = 2.;

	mSprite.setScale(0.5, 0.5);


	collisionRect.setOrigin(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
	collisionRect.setSize(sf::Vector2f(mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height));
	collisionRect.setOutlineColor(sf::Color::Red);
	collisionRect.setFillColor(sf::Color::Transparent);
	collisionRect.setOutlineThickness(2);
	collisionRect.setPosition(mPositon);

	FeildofView.setOrigin(0, 150);
	FeildofView.setSize(sf::Vector2f(300, 300));
	FeildofView.setOutlineColor(sf::Color::Yellow);
	FeildofView.setFillColor(sf::Color::Transparent);
	FeildofView.setOutlineThickness(2);
	FeildofView.setPosition(mPositon);



	alignment = sf::Vector2f(0, 0);
	cohesion = sf::Vector2f(0, 0);
	seperation = sf::Vector2f(0, 0);
}

sf::Vector2f Abductor::FindNewPoint()
{
	sf::Vector2f newPoint = sf::Vector2f(rand() % (1920 * 9) + 1, 500);//rand() % (500) + 1);
	return  newPoint;
}
void Abductor::Wandering(std::vector<Abductor*>& abductors, std::vector<Obstacles*>& obstacles, std::vector<Human*>& humans)
{
	CheckForHuman(humans);
	alignment = ComputeAlignment(abductors);
	cohesion = ComputeCohesion(abductors);
	seperation = ComputeSeperation(abductors);
	obsticleseperation = ComputeObsticleSeperation(obstacles);

	Direction = seekPoint - mPositon;
	Direction = CollisionManager::instance()->NormaliseVector(Direction);

	if (mPositon.y < 100)//stop form going to high
	{
		obsticleseperation.y += +1;
	}
	if (mPositon.y > 700)//stop from going to low
	{
		obsticleseperation.y += -1;
	}



	velocity.x += alignment.x + cohesion.x + seperation.x * 2 + obsticleseperation.x * 2 + Direction.x;
	velocity.y += alignment.y + cohesion.y + seperation.y * 2 + obsticleseperation.y * 2 + Direction.y;

	velocity = CollisionManager::instance()->NormaliseVector(velocity);
	velocity.x = velocity.x * 10;
	velocity.y = velocity.y * 10;

	mPositon += velocity;
	mSprite.setPosition(mPositon);

	if (CollisionManager::instance()->CheckRange(mSprite.getGlobalBounds().width, seekPoint, mPositon) == true)
	{
		seekPoint = FindNewPoint();
		for (int i = 0; i < abductors.size(); i++)
		{
			if (abductors[i]->getseekPoint() != seekPoint)
			{
				abductors[i]->setseekPoint(seekPoint);
			}
		}
	}

}
sf::Vector2f Abductor::ComputeAlignment(std::vector<Abductor*>& abductors)
{
	sf::Vector2f m_alignment;
	int neighbourcount = 0;
	for (int i = 0; i < abductors.size(); i++)
	{
		if (i != myIndex)
		{
			if (CollisionManager::instance()->RectangleCollision(FeildofView, abductors[i]->getFOVRect()) == true && CollisionManager::instance()->RectangleCollision(collisionRect, abductors[i]->getRect()) == false)//CollisionManager::CheckRange(neighbourRange, abductors[i]->getPosition(), mPositon) == true)
			{
				if (abductors[i]->getseekPoint() != seekPoint)
				{
					abductors[i]->setseekPoint(seekPoint);
				}

				m_alignment += abductors[i]->getVelocity();
				neighbourcount++;
			}
		}
	}

	if (neighbourcount == 0)
	{

		return  sf::Vector2f(0, 0);
	}
	else
	{
		m_alignment.x = m_alignment.x / neighbourcount;
		m_alignment.y = m_alignment.y / neighbourcount;
		m_alignment = CollisionManager::instance()->NormaliseVector(m_alignment);
		return m_alignment;
	}
}

sf::Vector2f Abductor::ComputeCohesion(std::vector<Abductor*>& abductors)
{
	sf::Vector2f m_Cohesion;
	int neighbourcount = 0;

	for (int i = 0; i < abductors.size(); i++)
	{
		if (i != myIndex)
		{
			if (CollisionManager::instance()->RectangleCollision(FeildofView, abductors[i]->getRect()) == true && CollisionManager::instance()->RectangleCollision(collisionRect, abductors[i]->getRect()) == false)//CollisionManager::CheckRange(neighbourRange, abductors[i]->getPosition(), mPositon) == true && CollisionManager::RectangleCollision(collisionRect, abductors[i]->getRect()) == false)
			{
				m_Cohesion += abductors[i]->getPosition();
				neighbourcount++;
			}

		}
	}
	if (neighbourcount == 0)
	{
		return sf::Vector2f(0, 0);
	}
	else
	{
		m_Cohesion.x = m_Cohesion.x / neighbourcount;
		m_Cohesion.y = m_Cohesion.y / neighbourcount;
		m_Cohesion = m_Cohesion - mPositon;
		m_Cohesion = CollisionManager::instance()->NormaliseVector(m_Cohesion);
		return m_Cohesion;

	}

}
sf::Vector2f Abductor::ComputeSeperation(std::vector<Abductor*>& abductors)
{
	sf::Vector2f m_Seperation;
	int neighbourcount = 0;

	for (int i = 0; i < abductors.size(); i++)
	{
		if (i != myIndex)
		{
			if (CollisionManager::instance()->RectangleCollision(collisionRect, abductors[i]->getRect()) == true)//CollisionManager::CheckRange(mSprite.getGlobalBounds().width, abductors[i]->getPosition(), mPositon) == true)
			{
				m_Seperation += abductors[i]->getPosition() - mPositon;
				neighbourcount++;
			}

		}
	}
	if (neighbourcount == 0)
	{
		return sf::Vector2f(0, 0);
	}
	else
	{
		m_Seperation.x = m_Seperation.x / neighbourcount;
		m_Seperation.y = m_Seperation.y / neighbourcount;
		m_Seperation.x *= -1;
		m_Seperation.y *= -1;

		m_Seperation = CollisionManager::instance()->NormaliseVector(m_Seperation);
		return m_Seperation;
	}
}
sf::Vector2f Abductor::ComputeObsticleSeperation(std::vector<Obstacles*>& obstacles)
{
	sf::Vector2f m_Seperation;
	int neighbourcount = 0;

	for (int i = 0; i < obstacles.size(); i++)
	{

		if (CollisionManager::instance()->RectangleCollision(FeildofView, obstacles[i]->getRect()) == true)//CollisionManager::CheckRange(mSprite.getGlobalBounds().width, abductors[i]->getPosition(), mPositon) == true)
		{
			m_Seperation += obstacles[i]->getPosition() - mPositon;
			neighbourcount++;
		}


	}
	if (neighbourcount == 0)
	{
		return sf::Vector2f(0, 0);
	}
	else
	{
		m_Seperation.x = m_Seperation.x / neighbourcount;
		m_Seperation.y = m_Seperation.y / neighbourcount;
		m_Seperation.x *= -1;
		m_Seperation.y *= -1;

		m_Seperation = CollisionManager::instance()->NormaliseVector(m_Seperation);
		return m_Seperation;
	}
}
void Abductor::CheckForHuman(std::vector<Human*>& humans)
{
	for (int i = 0; i < humans.size(); i++)
	{
		if (CollisionManager::instance()->CheckRange(300, humans[i]->getPosition(), mPositon) == true)
		{
			if (humans[i]->getTargeted() == false && humans[i]->getFalling() == false)
			{
				currentBehaviour = Behaviour::Abduct;
				humans[i]->setTargeted(true);
				humanindex = i;
			}
		}
	}
}

void Abductor::Abducting(std::vector<Human*>& humans)
{
	if (mPositon.y < -30)
	{
		alive = false;
	}

	if (abducting == false)
	{
		if (humanindex < humans.size())
		{
			Direction = humans[humanindex]->getPosition() - mPositon;
			Direction = CollisionManager::instance()->NormaliseVector(Direction);
			velocity.x += Direction.x;
			velocity.y += Direction.y;

			velocity = CollisionManager::instance()->NormaliseVector(velocity);
			velocity.x = velocity.x * 2;
			velocity.y = velocity.y * 2;

			mPositon += velocity;

			if (CollisionManager::instance()->CheckRange(2, humans[humanindex]->getPosition(), mPositon) == true)//RectangleCollision(humans[humanindex]->getRext(),collisionRect) == true)
			{
				abducting = true;
				humans[humanindex]->setAbducted(true);
			}
		}
		else
		{
			CheckForHuman(humans);
		}
	}

	else
	{
		velocity = sf::Vector2f(0, -2);
		mPositon += velocity;
	}

	mSprite.setPosition(mPositon);
}
void Abductor::Update(std::vector<Abductor*>& abductors, int indexofCurrentAbductor, std::vector<Obstacles*>& obstacles, std::vector<Human*>& humans, sf::Vector2f playerpos)
{
	myIndex = indexofCurrentAbductor;

	if (currentBehaviour == Behaviour::Wander)
	{
		Wandering(abductors, obstacles, humans);
	}

	else if (currentBehaviour == Behaviour::Abduct)
	{
		Abducting(humans);
	}

	angle = atan2(velocity.y, velocity.x);
	angle = angle * (180 / 3.14);


	if (CollisionManager::instance()->CheckRange(500, mPositon, playerpos) == true)
	{
		angle = atan2(playerpos.y - mPositon.y, playerpos.x - mPositon.x);
		angle = angle * (180 / 3.14);
		Shoot(playerpos);
	}

	else
	{
		angle = atan2(velocity.y, velocity.x);
		angle = angle * (180 / 3.14);
	}

	mSprite.setRotation(angle);
	FeildofView.setRotation(angle);
	collisionRect.setPosition(mPositon);
	FeildofView.setPosition(mPositon);

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->Update() == false)
		{
			bullets.erase(bullets.begin() + i);
		}
	}
}
void Abductor::Shoot(sf::Vector2f playerpos)
{
	shotTimer += shotClock.getElapsedTime().asSeconds();
	sf::Time dt = shotClock.restart();

	if (shotTimer > shotdelay)
	{
		bullets.push_back(new AbductorBullet(mPositon, playerpos));
		shotTimer = 0;
	}

}
void Abductor::Draw(sf::RenderWindow & window)
{
	window.draw(mSprite);
	if (myGlobalOptions->drawCollisionBox)
	{
		window.draw(collisionRect);
		window.draw(FeildofView);
	}
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->Draw(window);
	}

}
void Abductor::setPosition(sf::Vector2f vec)
{
	mPositon = vec;
}
void Abductor::DropHuman(std::vector<Human*>& humans)
{
	if (currentBehaviour == Behaviour::Abduct)
	{
		if (humanindex < humans.size())
		{
			humans[humanindex]->setFalling(true);
			humans[humanindex]->setAbducted(false);
		}
		else
		{
			CheckForHuman(humans);
		}
	}
}
sf::Vector2f Abductor::getVelocity()
{
	return velocity;
}
sf::Vector2f Abductor::getPosition()
{
	return mPositon;
}

sf::RectangleShape Abductor::getRect()
{
	return collisionRect;
}

sf::RectangleShape Abductor::getFOVRect()
{
	return FeildofView;
}

sf::Vector2f Abductor::getseekPoint()
{
	return seekPoint;
}

bool Abductor::getAlive()
{
	return alive;
}



void Abductor::setseekPoint(sf::Vector2f point)
{
	seekPoint = point;
}




