#include "Player.h"
#include <iostream>

Player::Player()
{

}

Player::Player(sf::Vector2f pos, sf::Vector2f maxVel)
{
	mTexture = ResourceLoader::instance()->getplayershipTexture();
	powerupTex = ResourceLoader::instance()->getpowerTexture();
	mPositon = pos;
	maxVelocity = maxVel;

	mTexture.setSmooth(true);

	mSprite.setTexture(mTexture);
	mSprite.setPosition(mPositon);
	mSprite.setOrigin(sf::Vector2f(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2));


	powerupPos.x = 0;
	powerupPos.y = -1000;
	powerupSprite.setTexture(powerupTex);
	powerupSprite.setPosition(powerupPos);
	powerupSprite.setOrigin(sf::Vector2f(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2));
	mScale = sf::Vector2f(1.f, 1.f);

	bombLoaded = true;
	bombfired = false;
	warping = false;
	warpReady = true;
	invisible = false;
	flipSpeed = 0.15f;
	playerFacingRight = true;
	warptimer = 0;
	canShoot = true;
	shotTimer = 0;
	shotdelay = 0.3;
	powerupScale = 1;

	collisionRect.setOrigin(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
	collisionRect.setSize(sf::Vector2f(mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height));
	collisionRect.setOutlineColor(sf::Color::Red);
	collisionRect.setFillColor(sf::Color::Transparent);
	collisionRect.setOutlineThickness(2);
	collisionRect.setPosition(mPositon);

	powerupCollisionRect.setOrigin(powerupSprite.getGlobalBounds().width / 2, powerupSprite.getGlobalBounds().height / 2);
	powerupCollisionRect.setSize(sf::Vector2f(powerupSprite.getGlobalBounds().width, powerupSprite.getGlobalBounds().height));
	powerupCollisionRect.setOutlineColor(sf::Color::Red);
	powerupCollisionRect.setFillColor(sf::Color::Transparent);
	powerupCollisionRect.setOutlineThickness(2);
	powerupCollisionRect.setPosition(powerupPos);

	bombrectSize = sf::Vector2f(0, 0);
	bombRectangle.setOrigin(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
	bombRectangle.setSize(bombrectSize);
	bombRectangle.setOutlineColor(sf::Color::Magenta);
	bombRectangle.setFillColor(sf::Color::Transparent);
	bombRectangle.setOutlineThickness(4);
	bombRectangle.setPosition(mPositon);

	bombTimer = 0;
	powerLevel = 1;
	powertimer = 0;
	powerupdelay = rand() % (20) + 5;
	powercollected = false;
}

void Player::Update()
{
	shotTimer += shotClock.getElapsedTime().asSeconds();


	if (bombLoaded == false)
	{
		bombTimer += shotClock.getElapsedTime().asSeconds();
		if (bombTimer > 5)
		{
			bombLoaded = true;
			bombTimer = 0;
		}
	}

	if (warpReady == false)
	{
		warptimer += shotClock.getElapsedTime().asSeconds();
		if (warptimer > 5)
		{
			warpReady = true;
			warptimer = 0;
		}
	}
	if (powercollected == false)
	{
		powertimer += shotClock.getElapsedTime().asSeconds();
		if (powertimer > powerupdelay)
		{
			powertimer = 0;
			powerupdelay = rand() % (20) + 5;
			powerupPos.x = rand() % (MAP_WIDTH_PIXEL)+1;
			powerupPos.y = rand() % (500) + 1;
			powercollected == true;
		}
	}

	if (shotTimer > shotdelay)
	{
		canShoot = true;
	}
	else
	{
		canShoot = false;
	}
	sf::Time dt = shotClock.restart();
	Flip();

	mSprite.setScale(mScale);
	mPositon += velocity;
	mSprite.setPosition(mPositon);

	if (abs(velocity.x) <= 1) { velocity.x = 0; }

	// Flip
	if (velocity.x > 0) { FaceRight(); }
	if (velocity.x < 0) { FaceLeft(); }

	for (int i = 0; i< bulletList.size(); i++)
	{
		if (bulletList[i]->Update() == false)
		{
			bulletList.erase(bulletList.begin() + i);
		}
	}

	// Top and Bottom wall collision
	if (mPositon.y <= 0)
	{
		velocity.y = 0;
		mPositon.y = 0;
	}

	if (mPositon.y >= 1080)
	{
		velocity.y = 0;
		mPositon.y = 1080;
	}

	if (bombfired == true)
	{
		ShootBomb();
	}
	if (warping == true && warpReady == true)
	{
		Warp();
	}

	if (CollisionManager::instance()->RectangleCollision(powerupCollisionRect, collisionRect) == true)
	{
		powerLevel++;
		powerupPos.x = 0;
		powerupPos.y = -1000;
		powercollected = false;
	}

	bombRectangle.setPosition(mPositon);
	collisionRect.setPosition(mPositon);
	updatePowerup();
	powerupCollisionRect.setPosition(powerupPos);
	powerupSprite.setPosition(powerupPos);

}

void Player::Draw(sf::RenderWindow &window)
{
	window.draw(mSprite);

	if (myGlobalOptions->drawCollisionBox)
	{
		window.draw(collisionRect);
		window.draw(powerupCollisionRect);
	}
	window.draw(bombRectangle);
	window.draw(powerupSprite);
	for (int i = 0; i < bulletList.size(); i++)
	{
		bulletList[i]->Draw(window);
	}
}

void Player::FaceLeft()
{
	if (playerFacingRight != false)
	{
		velocity.x = 0;
		playerFacingRight = false;
	}
}

void Player::FaceRight()
{
	if (playerFacingRight != true)
	{
		velocity.x = 0;
		playerFacingRight = true;
	}
}

void Player::MoveUp(bool pressed)
{
	if (pressed == true)
	{
		if (velocity.y > -maxVelocity.y)
		{
			velocity.y = -20.5f;
		}
	}
	else
	{
		velocity.y = 0;
	}
}
void Player::MoveDown(bool pressed)
{
	if (pressed == true)
	{
		if (velocity.y < maxVelocity.y)
		{
			velocity.y = 20.5f;
		}
	}
	else
	{
		velocity.y = 0;
	}
}
void Player::MoveLeft()
{
	if (velocity.x > -maxVelocity.x)
	{
		velocity.x -= 1.5f - (velocity.x / 10);
	}
	else
	{
		velocity.x = -maxVelocity.x;
	}
}
void Player::MoveRight()
{
	if (velocity.x < maxVelocity.x)
	{
		velocity.x += 1.5f + (velocity.x / 10);
	}
	else
	{
		velocity.x = maxVelocity.x;
	}
}

void Player::ShootBomb()
{

	if (bombLoaded == true)
	{
		velocity = sf::Vector2f(0, 0);
		if (bombrectSize.x <= SCREEN_WIDTH_PIXEL + 10)
		{
			bombrectSize.x += 100;
			bombrectSize.y += 100;
		}

		bombRectangle.setOrigin(bombrectSize.x / 2, bombrectSize.y / 2);
		bombRectangle.setSize(sf::Vector2f(bombrectSize.x, bombrectSize.y));

		if (bombrectSize.x >= SCREEN_WIDTH_PIXEL)
		{
			bombLoaded = false;
			bombfired = false;
			bombrectSize.x = 0;
			bombrectSize.y = 0;

		}
	}

	bombRectangle.setOrigin(bombrectSize.x / 2, bombrectSize.y / 2);
	bombRectangle.setSize(sf::Vector2f(bombrectSize.x, bombrectSize.y));
}

void Player::Shoot()
{
	if (canShoot == true)
	{
		if (powerLevel == 1)
		{

			bulletList.push_back(new Bullet(mPositon, ResourceLoader::instance()->getbulletTexture(), playerFacingRight, sf::Vector2f(0, 0)));

			shotTimer = 0;
			resource->shoot.play();
		}
		else if (powerLevel == 2)
		{
			bulletList.push_back(new Bullet(mPositon + sf::Vector2f(0, 20), ResourceLoader::instance()->getbulletTexture(), playerFacingRight, sf::Vector2f(0, 0)));
			bulletList.push_back(new Bullet(mPositon, ResourceLoader::instance()->getbulletTexture(), playerFacingRight, sf::Vector2f(0, 0)));
			bulletList.push_back(new Bullet(mPositon + sf::Vector2f(0, -20), ResourceLoader::instance()->getbulletTexture(), playerFacingRight, sf::Vector2f(0, 0)));
			shotTimer = 0;
			resource->shoot.play();
		}
		else
		{
			bulletList.push_back(new Bullet(mPositon + sf::Vector2f(0, 20), ResourceLoader::instance()->getbulletTexture(), playerFacingRight, sf::Vector2f(0, 0)));
			bulletList.push_back(new Bullet(mPositon, ResourceLoader::instance()->getbulletTexture(), playerFacingRight, sf::Vector2f(0, 0)));
			bulletList.push_back(new Bullet(mPositon + sf::Vector2f(0, -20), ResourceLoader::instance()->getbulletTexture(), playerFacingRight, sf::Vector2f(0, 0)));
			bulletList.push_back(new Bullet(mPositon, ResourceLoader::instance()->getbulletTexture(), !playerFacingRight, sf::Vector2f(0, 0)));
			shotTimer = 0;
			resource->shoot.play();
		}
	}
}

void Player::Warp()
{
	if (mScale.x >= 0 && mScale.y >= 0 && invisible == false)
	{
		mScale.x -= 0.2;
		mScale.y -= 0.2;
	}
	else
	{
		invisible = true;
	}


	if (invisible == true)
	{
		mPositon.x = rand() % (MAP_WIDTH_PIXEL)+1;

		mScale.y += 0.2;

		if (mScale.y >= 1)
		{
			mScale = sf::Vector2f(1, 1);
			warping = false;
			warpReady = false;
			invisible = false;
		}

	}

	if (mScale.y <= 1 && invisible == true)
	{
		mScale.y += 0.2;
	}

}

void Player::Flip()
{
	if (playerFacingRight == false)
	{
		if (mScale.x > -1)
		{
			mScale.x -= flipSpeed;
		}
	}
	else if (playerFacingRight == true)
	{
		if (mScale.x < 1)
		{
			mScale.x += flipSpeed;
		}
	}

	/*if (playerFacingUp == true)
	{
	if (mScale.y > -1)
	mScale.y -= flipSpeed;
	}
	else if (playerFacingUp == false)
	{
	if (mScale.y < 1)
	mScale.y += flipSpeed;
	}*/
}

sf::Vector2f Player::getVelocity()
{
	return velocity;
}

sf::Vector2f Player::getPosition()
{
	return mPositon;
}
sf::RectangleShape Player::getRect()
{
	return collisionRect;
}

bool Player::teleport()
{
	// Wrap around teleport
	if (mPositon.x < 0)
	{
		mPositon.x = 1920 * 9;
		teleportLeft = true;
		return true;
	}

	else if (mPositon.x > 1920 * 9)
	{
		mPositon.x = 0;
		teleportLeft = false;
		return true;
	}

	else
	{
		return false;
	}
}

bool Player::getBombfired()
{
	return bombfired;
}

void Player::setBombfired(bool fire)
{
	bombfired = fire;
}

bool Player::getbombLoaded()
{
	return bombLoaded;
}

bool Player::getwarpready()
{
	return warpReady;
}

void Player::setWarp(bool warp)
{
	warping = warp;
}


void Player::Decelerate()
{
	if (velocity.x != 0) { velocity.x *= 0.9f; }
}

void Player::updatePowerup()
{
	powerupScale -= 0.05;
	if (powerupScale <= -1)
	{
		powerupScale = 1;
	}
	powerupSprite.setScale(powerupScale, 1);
}

std::vector<Bullet*> Player::getBullets()
{
	return bulletList;
}

void Player::deleteBullet(int index)
{
	bulletList.erase(bulletList.begin() + index);
	bulletList.shrink_to_fit();
}