#include "SceneManager.h"

SceneManager::SceneManager()
{
	init();
}

SceneManager::~SceneManager()
{
}

void SceneManager::init()
{
	myGlobalOptions = GlobalVariables::getInstance();
	inputManager = InputManager::getInstance();

	camera = new Camera();
	splashScreen = new SplashScreen();
	mainMenu = new MainMenu();
	game = new Game(camera);
	load = new LoadScreen();
	optionsMenu = new OptionsMenu();
	instructions = new InstructionsScreen();
}

void SceneManager::update()
{
	if (myGlobalOptions->getCurrentScene() == myGlobalOptions->SPLASH) { splashScreen->update(); }
	else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->MAINMENU) { mainMenu->update(); }
	else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->GAME) { game->update(); }
	else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->LOAD) { load->update(); }
	else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->OPTIONS) { optionsMenu->update(); }
	else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->INSTRUCTIONS) { instructions->update(); }
}

void SceneManager::draw(sf::RenderWindow &window)
{
	if (myGlobalOptions->getCurrentScene() == myGlobalOptions->SPLASH) { splashScreen->draw(window); camera->drawGame(window); }
	else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->MAINMENU) { mainMenu->draw(window); camera->drawGame(window); }
	else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->GAME) { game->draw(window); }
	else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->LOAD) { load->draw(window); }
	else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->OPTIONS) { optionsMenu->draw(window); }
	else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->INSTRUCTIONS) { instructions->draw(window); }
}

void SceneManager::input(sf::Event &Event)
{

}