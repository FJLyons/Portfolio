#include "Manager_Scene.h"

SceneManager::SceneManager()
{
	Init();
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	ResourceManager::GetInstance()->music.play();

	// Load scene into vector of scenes
	m_splash = new SplashScreen();
	m_currentScene.push_back(m_splash);
	m_runningScene = GameScene::SPLASH;
}

void SceneManager::Process()
{
	m_previousScene = m_runningScene;

	m_runningScene = m_currentScene.back()->Update();
	m_currentScene.back()->Draw(*ResourceManager::GetInstance()->windowReference);

	SwapScene();
}

void SceneManager::Input(sf::Event evt)
{
	m_currentScene.back()->Input(evt);
}

void SceneManager::SwapScene()
{
	if (m_runningScene != m_previousScene)
	{

		for (std::vector< Scene* >::iterator it = m_currentScene.begin(); it != m_currentScene.end(); ++it)
		{
			delete *it;
		}
		m_currentScene.pop_back();

		if (m_runningScene == GameScene::SPLASH)
		{
			m_splash = new SplashScreen();
			m_currentScene.push_back(m_splash);
		}

		else if (m_runningScene == GameScene::MAINMENU)
		{
			m_mainMenu = new MainMenu();
			m_currentScene.push_back(m_mainMenu);
		}

		else if (m_runningScene == GameScene::GAME)
		{
			m_game = new Game();
			m_currentScene.push_back(m_game);
		}

		else if (m_runningScene == GameScene::INSTRUCTIONS)
		{
			m_instructions = new InstructionsScreen();
			m_currentScene.push_back(m_instructions);
		}
	}
}
