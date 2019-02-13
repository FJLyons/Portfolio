#include "Scene_Main_Menu.h"


MainMenu::MainMenu()
{
	Init();
}


MainMenu::~MainMenu()
{
}

void MainMenu::Init()
{
	//Art
	scaleFactor = sf::Vector2f(1, 1);

	if (!m_logoTexture.loadFromFile("content\\logo.png")){ std::cout << "Logo Art Error" << std::endl; }
	m_logoTexture.setSmooth(true);
	m_logoSprite.setTexture(m_logoTexture);
	m_logoSprite.setScale(scaleFactor);
	m_logoSprite.setOrigin(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	m_logoSprite.setPosition(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));

	// Index
	selectedItemIndex = 0;

	//Text	
	text[0].setString("New Game");
	text[1].setString("Instructions");
	text[2].setString("Quit");

	for (int i = 0; i < MAIN_MENU_INDEX; i++)
	{
		text[i].setFont(ResourceManager::GetInstance()->FONT);
		text[i].setPosition((SCREEN_WIDTH / 2) - text[i].getLocalBounds().width / 3.5f, (SCREEN_HEIGHT / 2) + (48.0f * i) - text[i].getLocalBounds().height / 2.0f);
		text[i].setCharacterSize(18);
		text[i].setFillColor(sf::Color::White);
	}

	text[0].setFillColor(sf::Color::Red);

	m_currentScene = GameScene::MAINMENU;
}

GameScene MainMenu::Update()
{
	return m_currentScene;
}

void MainMenu::Draw(sf::RenderWindow &window)
{
	window.draw(m_logoSprite);

	for (int i = 0; i < MAIN_MENU_INDEX; i++)
	{
		window.draw(text[i]);
	}
}

void MainMenu::Input(sf::Event Event)
{
	if (m_inputManager->KeyPressed(sf::Keyboard::BackSpace)) 
	{ 
		m_currentScene = GameScene::SPLASH;
		ResourceManager::GetInstance()->s_enter.play();
	}

	if (m_inputManager->KeyPressed(sf::Keyboard::Return))
	{
		SwapScreen();
		ResourceManager::GetInstance()->s_enter.play();
	}

	if (m_inputManager->KeyPressed(sf::Keyboard::Up))
	{
		MoveUp();
		ResourceManager::GetInstance()->s_menu.play();
	}

	if (m_inputManager->KeyPressed(sf::Keyboard::Down))
	{
		MoveDown();
		ResourceManager::GetInstance()->s_menu.play();
	}
}

void MainMenu::SwapScreen()
{
	if (GetPressedItem() == MenuOptions::GAME) 
	{
		std::cout << "New Game" << std::endl;
		m_currentScene = GameScene::GAME;
	}

	else if (GetPressedItem() == MenuOptions::INSTRUCTIONS)
	{
		std::cout << "Instrctions Screen" << std::endl;
		m_currentScene = GameScene::INSTRUCTIONS;
	}

	if (GetPressedItem() == MenuOptions::QUIT)
	{
		std::cout << "Quit" << std::endl;
		ResourceManager::GetInstance()->windowReference->close();
	}
}

void MainMenu::MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		text[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex--;
		text[selectedItemIndex].setFillColor(sf::Color::Red);
	}
	// Jump to bottom
	else
	{
		text[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex = MAIN_MENU_INDEX - 1;
		text[selectedItemIndex].setFillColor(sf::Color::Red);
	}
}

void MainMenu::MoveDown()
{
	if (selectedItemIndex + 1 < MAIN_MENU_INDEX)
	{
		text[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex++;
		text[selectedItemIndex].setFillColor(sf::Color::Red);
	}
	// Jump to top
	else
	{
		text[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex = 0;
		text[selectedItemIndex].setFillColor(sf::Color::Red);
	}
}

int MainMenu::GetPressedItem()
{
	// Index in menu
	return selectedItemIndex;
}
