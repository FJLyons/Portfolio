#include "Camera.h"

Camera::Camera() : radarView(sf::FloatRect(0, 0, 1920 * 9, 1080))
{
	init();
}


Camera::~Camera()
{
}

void Camera::init()
{
	// View
	gameView.setSize(screenSize);
	gameView.setCenter(screenCenter);

	gameView.setViewport(sf::FloatRect(0, 0, 1, 1)); //percentages
	radarView.setViewport(sf::FloatRect(0.20f, 0.8f, 0.60f, 0.10f)); //percentages
}

void Camera::update(Player *player)
{
	gameView.setCenter(sf::Vector2f(player->getPosition().x, screenSize.y / 2));
}

void Camera::drawGame(sf::RenderWindow &window)
{
	window.setView(gameView);
}

void Camera::drawRadar(sf::RenderWindow &window)
{
	window.setView(radarView);
}

sf::View Camera::getView()
{
	return gameView;
}

void Camera::setViewCenter(sf::Vector2f *position)
{
	gameView.setCenter(*position);
}

void Camera::resetView()
{
	if (gameView.getCenter() != screenCenter)
	{
		gameView.setCenter(screenCenter);
	}
}