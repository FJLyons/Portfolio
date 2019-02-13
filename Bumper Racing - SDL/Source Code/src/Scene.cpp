#include "Scene.h"

Scene::Scene()
	: _running(false)
{

}

Scene::~Scene()
{

}

void Scene::Initialize(SDL_Renderer* renderer)
{
	_renderer = renderer;
}

int Scene::Update()
{
	return 0;
}

void Scene::Render()
{
}

bool Scene::IsRunning()
{
	return nullptr;
}

void Scene::Start()
{
}

void Scene::Stop()
{
}

void Scene::BindInput()
{
}

SDL_Texture * Scene::LoadTexture(const std::string & path)
{
	SDL_Texture* texture = NULL;

	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(_renderer, surface);
		if (texture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(surface);
	}

	return texture;
}

void Scene::LoadContent()
{
}

void Scene::CleanUp()
{
}
