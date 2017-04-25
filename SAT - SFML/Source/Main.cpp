//Frankie Lyons
//C00177077
//SAT Assignment

//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
//#include "stdafx.h" 
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
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>

#include "Objects.h"
#include "Triangles.h"
#include "Squares.h"
#include "CollisionDetection.h"
#include "SATCollisions.h"
#include "QuadTree.h"

using namespace std;
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>



////////////////////////////////////////////////////////////
///Entrypoint of application 
//////////////////////////////////////////////////////////// 

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720, 32), "SAT Program");
	//window.setFramerateLimit(30);

	srand(time(0));
	const int numberOfShapes = 20;
	Objects* shapes[numberOfShapes];
	
	CollisionDetection collision;
	SATCollisions SAT;

	Quadtree quadtree(0, 0, 1280, 720, 0, 5);

	for (int i = 0; i < numberOfShapes; i++)
	{
		//create a shape
		Objects*  myComplexShape = new Objects();

		myComplexShape->setSize(rand() % 25 + 25);
		myComplexShape->setPosition(sf::Vector2f(rand() % 1180 + 50, rand() % 620 + 50));
		myComplexShape->setVelocity(sf::Vector2f((rand() % 5 - 2.5f), (rand() % 5 - 2.5f)));
		myComplexShape->setShape(rand() % 2 + 3);

		shapes[i] = myComplexShape;
	}

	// Start game loop 
	while (window.isOpen())
	{
		// Process events 
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit 
			if (Event.type == sf::Event::Closed){ window.close(); }

			// Escape key : exit 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)){ window.close(); }
		}

		//prepare frame
		window.clear();


		// Update


		quadtree.AddObject(shapes, numberOfShapes);

		if (quadtree.quad == true)
		{
			for (int i = 0; i < numberOfShapes; i++)
			{
				shapes[i]->update();

				for (int j = i + 1; j < numberOfShapes; j++)
				{
					if (collision.circleCollision(shapes[i], shapes[j]))
					{
						SAT.gapDetection(shapes[i], shapes[j]);

						//if (SAT.gapDetection(shapes[i], shapes[j]))
						//{
						//	collision.circleResolve(shapes[i], shapes[j]);
						//}
					}
				}

				window.draw(shapes[i]->getShape());
				collision.borderCollision(shapes[i]);
			}
		}

		quadtree.Update(&window, shapes);
		quadtree.Clear();

		// Finally, display rendered frame on screen 
		window.display();

	} //loop back for next frame

	return EXIT_SUCCESS;
}