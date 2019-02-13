#include "RadarSystem.h"

#include "FlagComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

#include "NetworkHandler.h"

#include "CheckpointComponent.h"

RadarSystem::RadarSystem()
{
}


RadarSystem::~RadarSystem()
{
}


void RadarSystem::Initialize(Entity* player, Entity* radar, Entity* flag, std::vector<Entity*> characters, std::vector<Entity*> checkpoints)
{
	_player = player;
	_radar = radar;
	_flag = flag;
	_characters = characters;
	_checkpoints = checkpoints;
}

void RadarSystem::Process(float dt)
{
	TransformComponent* playerTransform = static_cast<TransformComponent*>(_player->GetComponent(Component::Type::Transform));
	TransformComponent* radarTransform = static_cast<TransformComponent*>(_radar->GetComponent(Component::Type::Transform));

	radarTransform->rect.x = playerTransform->rect.x;
	radarTransform->rect.y = playerTransform->rect.y;


	FlagComponent* pFlag = static_cast<FlagComponent*>(_player->GetComponent(Component::Type::Flag));

	if (pFlag->hasFlag)
	{

		int targetCheckpoint = pFlag->currentCheckpointID + 1;

		for (int i = 0; i < _checkpoints.size(); i++)
		{
			CheckpointComponent* checkpoint = static_cast<CheckpointComponent*>(_checkpoints[i]->GetComponent(Component::Type::Checkpoint));

			if (targetCheckpoint == checkpoint->id)
			{
				TransformComponent* checkpointTransform = static_cast<TransformComponent*>(_checkpoints[i]->GetComponent(Component::Type::Transform));

				float xDir = checkpointTransform->rect.x - playerTransform->rect.x;
				float yDir = checkpointTransform->rect.y - playerTransform->rect.y;

				radarTransform->angle = atan2(yDir, xDir) * 180.f / M_PI;
			}
		}
	}
	else
	{
		ColliderComponent* flagCollider = static_cast<ColliderComponent*>(_flag->GetComponent(Component::Type::Collider));
		bool flagTaken = !flagCollider->body->IsActive();

		if ((flagTaken && NetworkHandler::Instance().GetPlayerID() == -1) || (flagTaken && NetworkHandler::Instance().GetPlayerID() != -1 && NetworkHandler::Instance().GetHost()) )
		{
			for (int i = 0; i < _characters.size(); i++)
			{
				FlagComponent* characterFlag = static_cast<FlagComponent*>(_characters[i]->GetComponent(Component::Type::Flag));

				if (characterFlag->hasFlag)
				{
					TransformComponent* characterTransform = static_cast<TransformComponent*>(_characters[i]->GetComponent(Component::Type::Transform));

					float xDir = characterTransform->rect.x - playerTransform->rect.x;
					float yDir = characterTransform->rect.y - playerTransform->rect.y;

					radarTransform->angle = atan2(yDir, xDir) * 180.f / M_PI;
					break;
				}
			}
		}
		else
		{
			TransformComponent* flagTransform = static_cast<TransformComponent*>(_flag->GetComponent(Component::Type::Transform));

			float xDir = flagTransform->rect.x - playerTransform->rect.x;
			float yDir = flagTransform->rect.y - playerTransform->rect.y;

			radarTransform->angle = atan2(yDir, xDir) * 180.f / M_PI;
		}
	}
}