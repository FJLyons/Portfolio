#include "InputManager.h"

InputManager* InputManager::inputManagerInstance = nullptr;

InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

InputManager * InputManager::getInstance()
{
	if (inputManagerInstance == nullptr)
	{
		inputManagerInstance = new InputManager();
	}
	return inputManagerInstance;
}

//Gnereate events
void InputManager::update(sf::Event evn)
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

// Multiple Keys
bool InputManager::KeysPressed(std::vector<sf::Keyboard::Key> keys)
{
	for (int i = 0; i < keys.size(); i++)
	{
		if (Event.type == sf::Event::KeyPressed && Event.key.code == keys[i])
		{
			previousKeys.push_back(keys[i]);
			return true;
		}
	}
	previousKeys.empty();
	return false;
}

bool InputManager::KeysReleased(std::vector<sf::Keyboard::Key> keys)
{
	for (int i = 0; i < keys.size(); i++)
	{
		if (Event.type == sf::Event::KeyReleased && Event.key.code == keys[i])
		{
			previousKeys.empty();
			return true;
		}
	}
	return false;
}

bool InputManager::KeysHeld(std::vector<sf::Keyboard::Key> keys)
{
	if (previousKeys == keys)
	{
		for (int i = 0; i < keys.size(); i++)
		{
			if (Event.key.code == keys[i])
			{
				previousKeys.push_back(keys[i]);
				return true;
			}
		}
	}
	previousKeys.empty();
	return false;
}
