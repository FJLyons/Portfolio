#include "GlobalVariables.h"

GlobalVariables* GlobalVariables::globalVariablesInstance = nullptr;


GlobalVariables::GlobalVariables()
{
	init();
}


GlobalVariables::~GlobalVariables()
{
}

GlobalVariables * GlobalVariables::getInstance()
{
	if (globalVariablesInstance == nullptr)
	{
		globalVariablesInstance = new GlobalVariables();
	}
	return globalVariablesInstance;
}


void GlobalVariables::init()
{
	currentScene = GameStates::SPLASH;
}

void GlobalVariables::update()
{

}

void GlobalVariables::draw(sf::RenderWindow &window)
{

}

int GlobalVariables::getCurrentScene() { return currentScene; }
void GlobalVariables::setCurrentScene(int current) { currentScene = current; }
