#pragma once

#include "Scene.h"

class SplashScreen : public Scene
{
private:
	// Avoid unwanted construction
	SplashScreen(const SplashScreen&) = delete;
	// Avoid unwanted assignment
	SplashScreen& operator=(const SplashScreen&) = delete;

public:
	// Create Scene
	SplashScreen();
	// Destructor
	~SplashScreen();

	// Initialise scene
	void Init() override;
	// Update scene
	GameScene Update() override;
	// Draw scene
	// @param window - SFML window
	void Draw(sf::RenderWindow &window) override;
	// Update Input
	// @param event - SF Event
	void Input(sf::Event Event) override;

private:
	//// VARIABLES

	// Logo Texture
	sf::Texture	 m_logoTexture;
	// Logo Sprite
	sf::Sprite m_logoSprite;

	// "Press Any Key" text
	sf::Text pressAnyText;

	// Resolution vs Screen
	sf::Vector2f scaleFactor;

	//// FADE SPRITE

	// Fade Image Alpha
	float logoAlphaFade;	
	// Increase fade speed
	float logoFadeMultiply;	

	//// FADE TEXT

	// alpha Fade for text
	int textAlphaFade;		
	// is the text fading
	bool fading;				

	////  PRINT TEXT

	// System Clock for timing print
	sf::Clock clock;		
	// On Screen Text - Press Any Key
	std::string pressAnyString;	
	// Current letter being printed
	int currentLetter;
	// Has been printed to screen
	bool printed;			

	//// FUNCTIONS

	// Print Text Letter by Letter
	// @param str - Text string
	// @param delayTime - Speed of print
	void Print(const std::string& str, const int delayTime);

	// Fade Alpha od text
	void Fade();
};

