#include "OptionsMenu.h"

OptionsMenu::OptionsMenu()
{
	init();
}

OptionsMenu::~OptionsMenu()
{
}

void OptionsMenu::init()
{
	//Art
	scaleFactor = sf::Vector2f((1280.0f / 1920.0f), (720.0f / 1080.0f));

	if (!logoTexture.loadFromFile("content\\art\\other\\logo.png")) { std::cout << "Logo Art Error" << std::endl; }
	logoTexture.setSmooth(true);
	logoSprite.setTexture(logoTexture);
	logoSprite.setScale(scaleFactor);
	logoSprite.setOrigin(sf::Vector2f(screenSize.x / 2, screenSize.y / 2));
	logoSprite.setPosition(sf::Vector2f(screenSize.x / 2, screenSize.y / 2));

	// Index
	selectedItemIndex = 0;

	//Text
	font.loadFromFile("content\\fonts\\kenvector_future.TTF");

	text[0].setString("test");


	text[0].setString("Collision Box Off");
	text[1].setString("Music Volume = " + std::to_string(volume));

	for (int i = 0; i < OPTIONS_MENU_INDEX; i++)
	{
		text[i].setFont(font);
		text[i].setPosition((screenSize.x / 2) - text[i].getLocalBounds().width / 3.5f, (screenSize.y / 2) + (48.0f * i) - text[i].getLocalBounds().height / 2.0f);
		text[i].setCharacterSize(18);
		text[i].setColor(sf::Color::White);
	}

	text[0].setColor(sf::Color::Red);
}

void OptionsMenu::update()
{

}

void OptionsMenu::input(sf::Event Event)
{
	if (inputManager->KeyPressed(sf::Keyboard::BackSpace))
	{
		std::cout << "Back Space" << std::endl;
		goToScene(myGlobalOptions->MAINMENU);
		resource->back.play();
	}

	if (inputManager->KeyPressed(sf::Keyboard::Return))
	{
		// Toggle option
		resource->menuSelect.play();
	}

	if (inputManager->KeyPressed(sf::Keyboard::Up))
	{
		std::cout << "Up" << std::endl;
		moveUp();
		resource->menuMove.play();
	}

	if (inputManager->KeyPressed(sf::Keyboard::Down))
	{
		std::cout << "Down" << std::endl;
		moveDown();
		resource->menuMove.play();
	}

	if (inputManager->KeyPressed(sf::Keyboard::Right))
	{
		std::cout << "Right" << std::endl;
		resource->menuMove.play();

		if (getPressedItem() == 0)
		{
			text[0].setString("Collision Box On");
			myGlobalOptions->drawCollisionBox = true;
		}

		if (getPressedItem() == 1)
		{
			if (volume != 100)
			{
				volume += 10;
				resource->musicMenu.setVolume(volume);
				resource->musicGame.setVolume(volume);
				text[1].setString("Music Volume = " + std::to_string(volume));
			}
		}
	}

	if (inputManager->KeyPressed(sf::Keyboard::Left))
	{
		std::cout << "Left" << std::endl;
		resource->menuMove.play();

		if (getPressedItem() == 0)
		{
			text[0].setString("Collision Box Off");
			myGlobalOptions->drawCollisionBox = false;
		}

		if (getPressedItem() == 1)
		{
			if (volume != 0)
			{
				volume -= 10;
				resource->musicMenu.setVolume(volume);
				resource->musicGame.setVolume(volume);
				text[1].setString("Music Volume = " + std::to_string(volume));
			}
		}
	}

}

void OptionsMenu::draw(sf::RenderWindow &window)
{
	window.draw(logoSprite);

	for (int i = 0; i < OPTIONS_MENU_INDEX; i++)
	{
		window.draw(text[i]);
	}
}

void OptionsMenu::goToScene(int scene)
{
	myGlobalOptions->setCurrentScene(scene);
}

void OptionsMenu::moveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		text[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex--;
		text[selectedItemIndex].setColor(sf::Color::Red);
	}
	// Jump to bottom
	else
	{
		text[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex = OPTIONS_MENU_INDEX - 1;
		text[selectedItemIndex].setColor(sf::Color::Red);
	}
}

void OptionsMenu::moveDown()
{
	if (selectedItemIndex + 1 < OPTIONS_MENU_INDEX)
	{
		text[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex++;
		text[selectedItemIndex].setColor(sf::Color::Red);
	}
	// Jump to top
	else
	{
		text[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex = 0;
		text[selectedItemIndex].setColor(sf::Color::Red);
	}
}

int OptionsMenu::getPressedItem()
{
	return selectedItemIndex;
}

