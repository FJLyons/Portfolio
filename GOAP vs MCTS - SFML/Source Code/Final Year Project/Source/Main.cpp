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

// C++ Headers
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

// Custom Classes
#include "Manager_Scene.h"
#include "Manager_Resource.h"
#include "Manager_Input.h"
#include "Logger.h"

SceneManager *sceneManager;
ResourceManager* GV;

///Entrypoint of application 
int main()
{
	// Random Seed
	srand(time(0));

	// Get Global Variables
	GV = ResourceManager::GetInstance();

	// Create the main window 
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "GOAP vs. MCTS", sf::Style::Default, settings);

	GV->windowReference = &window;

	// Set Frame Rate
	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	// Classes
	sceneManager = new SceneManager();

	// Clock

	// Start game loop 
	while (window.isOpen())
	{
		// Process events 
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			InputManager::GetInstance()->Update(Event);

			// Close Window
			if (Event.type == Event.Closed) 
			{ 
				Logger::GetInstance()->SaveFile();
				window.close(); 
			}
			if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape) 
			{
				Logger::GetInstance()->SaveFile();
				window.close(); 			
			}

			sceneManager->Input(Event);
		}

		//prepare frame
		window.clear();

		// Update and Draw
		sceneManager->Process();

		// Finally, display rendered frame on screen 
		window.display();
	} 

	//loop back for next frame
	return EXIT_SUCCESS;
}