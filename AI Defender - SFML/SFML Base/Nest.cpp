#include "Nest.h"

Nest::Nest(sf::Vector2f pos)
{
	mTexture = ResourceLoader::instance()->getnestTexture();

	if (pos.y > 0)
	{
		mPositon = pos;
	}
	else
	{
		mPositon = sf::Vector2f(rand() % MAP_WIDTH_PIXEL, 50);
	}

	velocity = sf::Vector2f(10, 10);

	mSprite.setTexture(mTexture);
	mSprite.setPosition(mPositon);
	mSprite.setOrigin(sf::Vector2f(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2));

	currentBehaviour = Behaviour::Wander;
	range = 700;
	seekPoint = FindNewPoint();


	canShoot = true;
	shotTimer = 0;
	shotdelay = 2;

	spawnTimer = 0;


	collisionRect.setOrigin(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
	collisionRect.setSize(sf::Vector2f(mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height));
	collisionRect.setOutlineColor(sf::Color::Red);
	collisionRect.setFillColor(sf::Color::Transparent);
	collisionRect.setOutlineThickness(2);
	collisionRect.setPosition(mPositon);
	spawnDelay = (rand() % (10) + 5);

	FeildofView.setOrigin(300, 150);
	FeildofView.setSize(sf::Vector2f(600, 300));
	FeildofView.setOutlineColor(sf::Color::Yellow);
	FeildofView.setFillColor(sf::Color::Transparent);
	FeildofView.setOutlineThickness(2);
	FeildofView.setPosition(mPositon);

	seperation = sf::Vector2f(0, 0);
}

Nest::~Nest()
{
}

sf::Vector2f Nest::FindNewPoint()
{
	sf::Vector2f newPoint = sf::Vector2f(rand() % (1920 * 9) + 1, rand() % (700) + 1);
	return  newPoint;
}



void Nest::Wandering(std::vector<Obstacles*>& obstacles)
{
	seperation = ComputeObsticleSeperation(obstacles);
	sf::Vector2f Direction = seekPoint - mPositon;
	Direction = seekPoint - mPositon;
	Direction = CollisionManager::instance()->NormaliseVector(Direction);

	if (mPositon.y < 100)//stop form going to high
	{
		seperation.y += +1;
	}
	if (mPositon.y > 700)//stop from going to low
	{
		seperation.y += -1;
	}


	velocity.x += seperation.x * 2 + Direction.x;
	velocity.y += seperation.y * 2 + Direction.y;

	velocity = CollisionManager::instance()->NormaliseVector(velocity);
	velocity.x = velocity.x * 10;
	velocity.y = velocity.y * 10;

	mPositon += velocity;
	//mPositon += sf::Vector2f(Direction.x * velocity.x, Direction.y * velocity.y);
	mSprite.setPosition(mPositon);

	if (CollisionManager::instance()->CheckRange(10, seekPoint, mPositon) == true)
	{
		seekPoint = FindNewPoint();
	}

}

void Nest::Evading()
{
	if (playerPosition.x > mPositon.x)
	{
		velocity = sf::Vector2f(-5, 0);
	}
	else
	{
		velocity = sf::Vector2f(5, 0);
	}

	mPositon += velocity;
	mSprite.setPosition(mPositon);


}
void Nest::Update(sf::RectangleShape  player, std::vector<Abductor*> &abductors, std::vector<Obstacles*>& obstacles)
{
	SpawnAbductors(abductors);
	playerPosition = player.getPosition();
	shotTimer += shotClock.getElapsedTime().asSeconds();
	sf::Time dt = shotClock.restart();

	if (shotTimer > shotdelay)
	{
		canShoot = true;
	}
	else
	{
		canShoot = false;
	}
	if (CollisionManager::instance()->CheckRange(400, playerPosition, mPositon) == true)
	{
		//currentBehaviour = Behaviour::Evade;

	}
	else
	{
		currentBehaviour = Behaviour::Wander;

	}
	if (CollisionManager::instance()->CheckRange(range, playerPosition, mPositon) == true && missileList.size() <3 && canShoot == true)
	{

		missileList.push_back(new InterceptorMissile(mPositon));
		shotTimer = 0;
	}


	for (int i = 0; i< missileList.size(); i++)
	{
		if (missileList[i]->Update(player) == false)
		{
			missileList.erase(missileList.begin() + i);
		}
	}

	if (currentBehaviour == Behaviour::Wander)
	{
		Wandering(obstacles);
	}
	else if (currentBehaviour == Behaviour::Evade)
	{
		Evading();
	}
	FeildofView.setPosition(mPositon);
	collisionRect.setPosition(mPositon);
}
void Nest::Draw(sf::RenderWindow & window)
{
	window.draw(mSprite);

	if (myGlobalOptions->drawCollisionBox)
	{
		window.draw(collisionRect);
		window.draw(FeildofView);
	}

	for (int i = 0; i < missileList.size(); i++)
	{
		missileList[i]->Draw(window);
	}
}

void Nest::SpawnAbductors(std::vector<Abductor*> &abductors)
{

	spawnTimer += spawnClock.getElapsedTime().asSeconds();
	spawnClock.restart();
	if (spawnTimer > spawnDelay)
	{
		spawnTimer = 0;
		spawnDelay = (rand() % (10) + 5);
		abductors.push_back(new Abductor(mPositon));
	}
}

//void Nest::calculateDrift()
//{
//	driftTimer += shotClock.getElapsedTime().asSeconds();
//	
//
//	if (driftTimer > 3)
//	{
//		int randomUpDown = rand() % (100) + 1;
//
//		if (randomUpDown % 2 == 0)
//		{
//			velocity.y = -velocity.y;
//			driftTimer = 0;
//		}
//		else
//		{
//			velocity.y = velocity.y;
//			driftTimer = 0;
//		}
//	}
//}

sf::Vector2f Nest::getPosition()
{
	return mPositon;
}

sf::Vector2f Nest::ComputeObsticleSeperation(std::vector<Obstacles*>& obstacles)
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
sf::RectangleShape Nest::getRect()
{
	return collisionRect;
}

