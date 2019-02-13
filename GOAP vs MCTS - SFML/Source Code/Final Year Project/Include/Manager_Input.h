// SFML Headers
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include<vector>

#pragma once
class InputManager
{
public:
	// Return an instance of the InputManager class
	static InputManager * GetInstance();
private:
	// InputManager instance object - for returning instance
	static InputManager * inputManagerInstance;

	// Avoid unwanted construction
	InputManager(const InputManager&) = delete;
	// Avoid unwanted assignment
	InputManager& operator=(const InputManager&) = delete;

public:
	InputManager();
	~InputManager();

	// Update Input
	// @param event - event to be updated for processing
	void Update(sf::Event event);

	// Get key pressed
	// @param key - key to check
	bool KeyPressed(sf::Keyboard::Key key);
	// Get key released
	// @param key - key to check
	bool KeyReleased(sf::Keyboard::Key key);
	// Get key held
	// @param key - key to check
	bool KeyHeld(sf::Keyboard::Key key);

private:
	// Event for updatin
	sf::Event Event;
};