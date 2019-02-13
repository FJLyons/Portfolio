#include <iostream>
#include <thread>
#include <string>

#include "LTimer.h"
#include "ConstHolder.h"

#include "SceneManager.h"
#include "Game.h"
#include "NetworkHandler.h"

using namespace std;

const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

#undef main

int main(int argc, char** argv)
{	
	SDLNet_Init();
	if (argc > 1)
		NetworkHandler::Instance().init(std::stoi(argv[1]));
	srand(time(NULL));
	SceneManager* sceneManager = new SceneManager();

	//Adjust screen positions as needed
	sceneManager->Initialize("Argo",100,100,1600,900, SDL_WINDOW_INPUT_FOCUS);

	LTimer capTimer;//to cap framerate

	while (sceneManager->IsRunning())
	{
		capTimer.start();

		sceneManager->Update();

		int frameTicks = capTimer.getTicks();//time since start of frame
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time before going to next frame
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}

	//system("PAUSE");

	SDLNet_Quit();

	return 0;
}


