#include "SFML/Audio.hpp"

#include <iostream>
#include <conio.h>
#include <windows.h>

#include "GlobalVariables.h"
#include "InputManager.h"
#include "ResourceLoader.h"

#define FULL_COLOUR 255

class SplashScreen
{
private:
	GlobalVariables* myGlobalOptions = GlobalVariables::getInstance();
	InputManager* inputManager = InputManager::getInstance();
	ResourceLoader* resource = ResourceLoader::instance();

public:
	SplashScreen();
	~SplashScreen();

	void init();
	void update();
	void draw(sf::RenderWindow &window);
	void input(sf::Event Event);

private:
	sf::Vector2f screenSize = myGlobalOptions->screenSize;

	void goToScene(int scene);

	void print(const std::string &str, int delay_time);
	void fade();

	// Art
	sf::Texture logoTexture;
	sf::Sprite logoSprite;

	// Font
	sf::Font font;
	sf::Text pressAnyText;

	// Art Variables
	int logoAlphaFade; // Fade Image Alpha
	float logoFadeMultiply; // Increase fade speed
	sf::Vector2f scaleFactor; // Resolution vs Screen

							  // Print text
	sf::Clock clock; // System Clock for timing print
	std::string pressAnyString; // On Screen Text - Press Any Key
	int currentLetter; // Current letter being printed
	bool printed; // Has been printed to screen

				  // Fade text
	int textAlphaFade; // alpha Fade for text
	bool fading; // is the text fading
};