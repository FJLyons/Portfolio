#include "Engine.h"

Engine::Engine()
{
	init();
}

Engine::~Engine()
{
}

void Engine::init()
{
	optionsLoader = new OptionsLoader();
	splashScreen = new SplashScreen();
	mainMenu = new MainMenu();
	game = new Game();
	load = new LoadScreen();
	optionsMenu = new OptionsMenu();
	instructions = new InstructionsScreen();

}

void Engine::update()
{
	if (optionsLoader->getCurrentScreen() == optionsLoader->SPLASH){ splashScreen->update(); }
	else if (optionsLoader->getCurrentScreen() == optionsLoader->MAINMENU){ mainMenu->update(); }
	else if (optionsLoader->getCurrentScreen() == optionsLoader->GAME){ game->update(); }
	else if (optionsLoader->getCurrentScreen() == optionsLoader->LOAD) { load->update(); }
	else if (optionsLoader->getCurrentScreen() == optionsLoader->OPTIONS){ optionsMenu->update(); }
	else if (optionsLoader->getCurrentScreen() == optionsLoader->INSTRUCTIONS) { instructions->update(); }
}

void Engine::draw(sf::RenderWindow &window)
{
	if (optionsLoader->getCurrentScreen() == optionsLoader->SPLASH){ splashScreen->draw(window); }
	else if (optionsLoader->getCurrentScreen() == optionsLoader->MAINMENU){ mainMenu->draw(window); }
	else if (optionsLoader->getCurrentScreen() == optionsLoader->GAME){ game->draw(window); }
	else if (optionsLoader->getCurrentScreen() == optionsLoader->LOAD) { load->draw(window); }
	else if (optionsLoader->getCurrentScreen() == optionsLoader->OPTIONS){ optionsMenu->draw(window); }
	else if (optionsLoader->getCurrentScreen() == optionsLoader->INSTRUCTIONS) { instructions->draw(window); }
}
