#include "Mutant.h"

Mutant::Mutant()
{
}
Mutant::~Mutant()
{

}
Mutant::Mutant(sf::Vector2f pos)
{

	mTexture = ResourceLoader::instance()->getmutantTexture();
	bulletTexture = ResourceLoader::instance()->getbulletTexture();
	mPositon = pos;
	velocity = sf::Vector2f(10, 0.5);
	mSprite.setTexture(mTexture);
	mSprite.setPosition(mPositon);
	mSprite.setOrigin(sf::Vector2f(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2));
	abducting = false;
	canShoot = true;

	shotdelay = 0.5;
	shotTimer = rand() % (5) + 1;
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


void Mutant::Wandering(std::vector<Mutant*>& mutants, std::vector<Obstacles*>& obstacles, sf::Vector2f playerPos)
{
	seekPoint = playerPos;

	alignment = ComputeAlignment(mutants);
	cohesion = ComputeCohesion(mutants);
	seperation = ComputeSeperation(mutants);
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



	velocity.x += (alignment.x) + cohesion.x + (seperation.x) + obsticleseperation.x * 2 + Direction.x;
	velocity.y += (alignment.y) + cohesion.y + (seperation.y) + obsticleseperation.y * 2 + Direction.y;

	velocity = CollisionManager::instance()->NormaliseVector(velocity);
	velocity.x = velocity.x * 10;
	velocity.y = velocity.y * 10;

	mPositon += velocity;
	mSprite.setPosition(mPositon);



}
sf::Vector2f Mutant::ComputeAlignment(std::vector<Mutant*>& mutants)
{
	sf::Vector2f m_alignment;
	int neighbourcount = 0;
	for (int i = 0; i < mutants.size(); i++)
	{
		if (i != myIndex)
		{
			if (CollisionManager::instance()->RectangleCollision(FeildofView, mutants[i]->getFOVRect()) == true && CollisionManager::instance()->RectangleCollision(collisionRect, mutants[i]->getRect()) == false)
			{
				m_alignment += mutants[i]->getVelocity();
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
sf::Vector2f Mutant::ComputeCohesion(std::vector<Mutant*>& mutants)
{
	sf::Vector2f m_Cohesion;
	int neighbourcount = 0;

	for (int i = 0; i < mutants.size(); i++)
	{
		if (i != myIndex)
		{
			if (CollisionManager::instance()->RectangleCollision(FeildofView, mutants[i]->getRect()) == true && CollisionManager::instance()->RectangleCollision(collisionRect, mutants[i]->getRect()) == false)//CollisionManager::CheckRange(neighbourRange, abductors[i]->getPosition(), mPositon) == true && CollisionManager::RectangleCollision(collisionRect, abductors[i]->getRect()) == false)
			{
				m_Cohesion += mutants[i]->getPosition();
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
sf::Vector2f Mutant::ComputeSeperation(std::vector<Mutant*>& mutants)
{
	sf::Vector2f m_Seperation;
	int neighbourcount = 0;

	for (int i = 0; i < mutants.size(); i++)
	{
		if (i != myIndex)
		{
			if (CollisionManager::instance()->RectangleCollision(collisionRect, mutants[i]->getRect()) == true)//CollisionManager::CheckRange(mSprite.getGlobalBounds().width, abductors[i]->getPosition(), mPositon) == true)
			{
				m_Seperation += mutants[i]->getPosition() - mPositon;
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
sf::Vector2f Mutant::ComputeObsticleSeperation(std::vector<Obstacles*>& obstacles)
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


void Mutant::Shoot(sf::Vector2f playerpos, sf::Vector2f playervel)
{
	shotTimer += shotClock.getElapsedTime().asSeconds();
	sf::Time dt = shotClock.restart();

	if (shotTimer > shotdelay)
	{
		bullets.push_back(new MutantBullet(mPositon, playerpos, playervel));
		shotTimer = 0;
	}

}

void Mutant::Update(std::vector<Mutant*>& mutants, int indexofCurrentMutant, std::vector<Obstacles*>& obstacles, sf::Vector2f playerPos, sf::Vector2f playerVel)
{
	myIndex = indexofCurrentMutant;


	Wandering(mutants, obstacles, playerPos);

	if (CollisionManager::instance()->CheckRange(500, mPositon, playerPos) == true)
	{

		Shoot(playerPos, playerVel);

	}

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
void Mutant::Draw(sf::RenderWindow & window)
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
void Mutant::setPosition(sf::Vector2f vec)
{
	mPositon = vec;
}

sf::Vector2f Mutant::getVelocity()
{
	return velocity;
}
sf::Vector2f Mutant::getPosition()
{
	return mPositon;
}

sf::RectangleShape Mutant::getRect()
{
	return collisionRect;
}

sf::RectangleShape Mutant::getFOVRect()
{
	return FeildofView;
}









