// SFML Headers
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include<vector>

#pragma once
class InputManager
{
public:
	static InputManager * getInstance();
private:
	static InputManager * inputManagerInstance;

public:
	InputManager();
	~InputManager();

	void update(sf::Event event);

	bool KeyPressed(sf::Keyboard::Key key);
	bool KeysPressed(std::vector<sf::Keyboard::Key> keys);

	bool KeyReleased(sf::Keyboard::Key key);
	bool KeysReleased(std::vector<sf::Keyboard::Key> keys);

	bool KeyHeld(sf::Keyboard::Key key);
	bool KeysHeld(std::vector<sf::Keyboard::Key> keys);

private:

	sf::Event Event;

	sf::Keyboard::Key previousKey = sf::Keyboard::Key::Unknown;
	std::vector<sf::Keyboard::Key> previousKeys = std::vector<sf::Keyboard::Key>();
};