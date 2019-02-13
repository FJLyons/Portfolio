#include "LoadScreen.h"



LoadScreen::LoadScreen()
{
	init();
}


LoadScreen::~LoadScreen()
{
}

void LoadScreen::init()
{
	font.loadFromFile("content\\fonts\\kenvector_future.TTF");
	text.setFont(font);
	text.setString("    Load Game Unavailable! \n\nBuy Premium now for $ 0.99");
	text.setPosition(screenSize.x / 2 - text.getLocalBounds().width / 4.0f, screenSize.y / 2);
	text.setCharacterSize(18);
}

void LoadScreen::update()
{

}

void LoadScreen::draw(sf::RenderWindow &window)
{
	window.draw(text);
}

void LoadScreen::input(sf::Event Event)
{
	if (inputManager->KeyPressed(sf::Keyboard::BackSpace))
	{
		std::cout << "Back Space" << std::endl;
		goToScene(myGlobalOptions->MAINMENU);
		resource->back.play();
	}
}

void LoadScreen::goToScene(int scene)
{
	myGlobalOptions->setCurrentScene(scene);
}