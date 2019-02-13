#include "Manager_Input.h"

InputManager* InputManager::inputManagerInstance = nullptr;

InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

InputManager * InputManager::GetInstance()
{
	if (inputManagerInstance == nullptr)
	{
		inputManagerInstance = new InputManager();
	}
	return inputManagerInstance;
}

//Gnereate events
void InputManager::Update(sf::Event evn)
{
	Event = evn;
}

bool InputManager::KeyPressed(sf::Keyboard::Key key)
{
	if (Event.type == sf::Event::KeyPressed && Event.key.code == key)
	{
		return true;
	}
	return false;
}

bool InputManager::KeyReleased(sf::Keyboard::Key key)
{
	if (Event.type == sf::Event::KeyReleased && Event.key.code == key)
	{
		return true;
	}
	return false;
}

bool InputManager::KeyHeld(sf::Keyboard::Key key)
{
	if (sf::Keyboard::isKeyPressed(key))
	{
		return true;
	}
	return false;
}
