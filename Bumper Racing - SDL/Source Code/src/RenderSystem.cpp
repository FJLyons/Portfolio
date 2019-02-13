#include "RenderSystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "StatusEffectComponent.h"
#include "ConstHolder.h"

RenderSystem::RenderSystem(float updateRate)
	: System(updateRate)
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::Initialize(SDL_Renderer*& renderer, Camera2D::Camera* camera)
{
	_renderer = renderer;
	_camera = camera;
}

void RenderSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				bool render = true;

				if (e->GetType() == EntityType::Player || e->GetType() == EntityType::AI || e->GetType() == EntityType::RemotePlayer)
				{
					StatusEffectComponent* statusEffect = static_cast<StatusEffectComponent*>(e->GetComponent(Component::Type::StatusEffect));

					if (statusEffect->invisible)
					{
						render = false;
					}
				}

				if (render)
				{
					TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
					SpriteComponent* sprite = static_cast<SpriteComponent*>(e->GetComponent(Component::Type::Sprite));

					SDL_Rect drawRectangle = transform->rect;
					drawRectangle.x -= transform->origin.x;
					drawRectangle.y -= transform->origin.y;

					//Render to screen
					SDL_RenderCopyEx(_renderer, sprite->texture, &sprite->sourceRect, &_camera->worldToScreen(drawRectangle), transform->angle, &transform->origin, SDL_FLIP_NONE);
				}
			}
		}

		float translationX = SCREEN_WIDTH * 0.75f;
		float translationY = SCREEN_HEIGHT * 0.75f;
		float minimapWidth = SCREEN_WIDTH*0.25f;
		float minimapHeight = SCREEN_HEIGHT*0.25f;
		float objectScaler = 0.2f;

		for (int i = 0; i < _minimapThings.size(); i++)
		{
			bool render = true;

			if (_minimapThings[i]->GetType() == EntityType::Player || _minimapThings[i]->GetType() == EntityType::AI || _minimapThings[i]->GetType() == EntityType::RemotePlayer)
			{
				StatusEffectComponent* statusEffect = static_cast<StatusEffectComponent*>(_minimapThings[i]->GetComponent(Component::Type::StatusEffect));

				if (statusEffect->invisible)
				{
					render = false;
				}
			}
			else if (_minimapThings[i]->GetType() == EntityType::Checkpoint)
			{
				objectScaler = 0.1f;
			}

			if (render)
			{
				TransformComponent* transform = static_cast<TransformComponent*>(_minimapThings[i]->GetComponent(Component::Type::Transform));
				SpriteComponent* sprite = static_cast<SpriteComponent*>(_minimapThings[i]->GetComponent(Component::Type::Sprite));

				SDL_Rect drawRectangle = transform->rect;
				drawRectangle.x -= transform->origin.x;
				drawRectangle.y -= transform->origin.y;

				drawRectangle.x = translationX + ((float)drawRectangle.x / (float)WORLD_WIDTH) * (float)minimapWidth;
				drawRectangle.y = translationY + ((float)drawRectangle.y / (float)WORLD_HEIGHT) * (float)minimapHeight;
				drawRectangle.w *= objectScaler;
				drawRectangle.h *= objectScaler;

				SDL_Point origin = { (int)(drawRectangle.w * 0.5f), (int)(drawRectangle.h * 0.5f)};

				SDL_RenderCopyEx(_renderer, sprite->texture, &sprite->sourceRect, &drawRectangle, transform->angle, &origin, SDL_FLIP_NONE);
			}
		}
	}
}


void RenderSystem::AddEntity(Entity* e)
{
	System::AddEntity(e);

	if (e->GetType() == EntityType::Checkpoint ||
		e->GetType() == EntityType::PowerUp ||
		e->GetType() == EntityType::AI ||
		e->GetType() == EntityType::Player ||
		e->GetType() == EntityType::Flag ||
		e->GetType() == EntityType::RemotePlayer ||
		e->GetType() == EntityType::Tile)
	{
		_minimapThings.push_back(e);
	}
}
void RenderSystem::RemoveEntity(EntityType tag, Entity* e)
{
	System::RemoveEntity(tag, e);

	if (tag == EntityType::Checkpoint ||
		tag == EntityType::PowerUp ||
		tag == EntityType::AI ||
		tag == EntityType::Player ||
		tag == EntityType::Flag ||
		tag == EntityType::RemotePlayer ||
		tag == EntityType::Tile)
	{
		for (int i = _minimapThings.size() - 1; i > -1; i--)
		{
			if (_minimapThings[i] == e)
			{
				_minimapThings.erase(_minimapThings.begin() + i);
				break;
			}
		}
	}
}
