// SFML Headers 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib")
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

// Open GL
#include "SFML/OpenGL.hpp"

// FMOD Headers
//#pragma comment(lib,"fmodex_vc.lib")
//#include "fmod.hpp"
//#include "fmod_errors.h"

// C++ Headers
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

// Custom Classes
#include "SceneManager.h"
#include "GlobalVariables.h"
#include "InputManager.h"

SceneManager *sceneManager;
GlobalVariables* myGlobalOptions;

///Entrypoint of application 
int main()
{
	// Random Seed
	srand(time(0));

	// Get Global Variables
	myGlobalOptions = GlobalVariables::getInstance();

	// Create the main window 
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	sf::Vector2f screenSize = myGlobalOptions->screenSize;
	sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "GAME", sf::Style::Default, settings);

	myGlobalOptions->windowReference = &window;

	// Set Frame Rate
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	// Classes
	sceneManager = new SceneManager();

	// Start game loop 
	while (window.isOpen())
	{
		// Process events 
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			InputManager::getInstance()->update(Event);

			// Close Window
			if (Event.type == Event.Closed) { window.close(); }
			if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape) { window.close(); }

			// Update input
			if (myGlobalOptions->getCurrentScene() == myGlobalOptions->SPLASH) { sceneManager->splashScreen->input(Event); }
			else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->MAINMENU) { sceneManager->mainMenu->input(Event); }
			else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->GAME) { sceneManager->game->input(Event); }
			else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->LOAD) { sceneManager->load->input(Event); }
			else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->OPTIONS) { sceneManager->optionsMenu->input(Event); }
			else if (myGlobalOptions->getCurrentScene() == myGlobalOptions->INSTRUCTIONS) { sceneManager->instructions->input(Event); }
			else { sceneManager->input(Event); }
		}

		//prepare frame
		window.clear();

		// Update and Draw
		sceneManager->update();
		sceneManager->draw(window);

		// Needed to ignore poll event stopping, while also using functions needing event
		if (myGlobalOptions->getCurrentScene() == myGlobalOptions->GAME) { sceneManager->game->input(Event); }

		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}