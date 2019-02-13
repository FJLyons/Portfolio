#include "Scene_Instructions.h"



InstructionsScreen::InstructionsScreen()
{
	Init();
}


InstructionsScreen::~InstructionsScreen()
{
}

void InstructionsScreen::Init()
{
	for (int i = 0; i < INSUI; i++)
	{
		m_text[i].setString("");
		m_text[i].setFont(ResourceManager::GetInstance()->FONT);
		m_text[i].setPosition(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * (0.08f * (i + 1)));
		m_text[i].setCharacterSize(52);
		m_text[i].setOutlineColor(Grey);
		m_text[i].setOutlineThickness(1);
	}

	m_text[0].setString("Use Arrow keys to navigate Main Menu");
	m_text[1].setString("Press Enter to go to a new scene");
	m_text[2].setString("Press BackSpace to exit a scene");
	m_text[3].setString("IN GAME:");
	m_text[4].setString("Press -P- to pause game");
	m_text[5].setString("Press -U- to hide UI");
	m_text[6].setString("Press -M- to pause music");
	m_text[7].setString("Press -Num 1- to show full debug");
	m_text[8].setString("Press -Num 2- to show only debug text");
	m_text[9].setString("Press -Num 3- to hide debug info");


	m_currentScene = GameScene::INSTRUCTIONS;
}

GameScene InstructionsScreen::Update()
{

	return m_currentScene;
}

void InstructionsScreen::Draw(sf::RenderWindow &window)
{
	for (int i = 0; i < INSUI; i++)
	{
		window.draw(m_text[i]);
	}
}

void InstructionsScreen::Input(sf::Event Event)
{
	if (m_inputManager->KeyPressed(sf::Keyboard::BackSpace))
	{
		m_currentScene = GameScene::MAINMENU;
	}
}
