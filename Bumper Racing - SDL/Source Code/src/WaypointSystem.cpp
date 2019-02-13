#include "WaypointSystem.h"
#include "TransformComponent.h" 
#include "ColliderComponent.h"
#include "Helpers.h"
#include "AIComponent.h"
#include "TransformComponent.h"
#include "PowerUpComponent.h"
#include "PhysicsComponent.h"
#include "DestructionComponent.h"
#include "NetworkHandler.h"

WaypointSystem::WaypointSystem(std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests, std::map<InteractionSystemEvent, std::vector<std::pair<Entity*, Entity*>>>& interactionSystemEvents, float updateRate)
	: System(updateRate)
	, _creationRequests(creationRequests)
	, _interactionSystemEvents(interactionSystemEvents)
	, POWER_UP_DESTRUCTION(InteractionSystemEvent::PowerUpDestoyed)
	, _powerUpCount(0)
{
}

WaypointSystem::~WaypointSystem()
{

}

void WaypointSystem::Initialize(Graph* waypoints)
{
	_waypoints = waypoints;
}

void WaypointSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;

		if (NetworkHandler::Instance().GetPlayerID() == -1 || (NetworkHandler::Instance().GetPlayerID() != -1 && NetworkHandler::Instance().GetHost()))
		{ //singleplayer or multiplayer and host
			CreatePowerUp(dt);
		}//multiplayer non hosts dont create power ups
		ListenForEvents();

		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
			
				TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
				ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));
				helper::Vector2 position = helper::Vector2(transform->rect.x, transform->rect.y);
				
				int maxNode = _waypoints->getNodesSize();
				int flagNode = maxNode - 1;
				GraphNode* node = _waypoints->getNodes()[flagNode];
				node->setPosition(helper::Vector2(transform->rect.x, transform->rect.y));

				bool hasNeighbourNode = false;
				int closestNode = 0;
				int size = maxNode - 1; //-1 because the flag is the last node
				for (int i = 0; i < size; i++)
				{
					float distance = (position - _waypoints->getNodes()[i]->getPosition()).length();
					if (distance < FLAG_CONNECTION_RADIUS)
					{
						GraphArc* arcFromFlag = _waypoints->getArc(flagNode, i);
						if (arcFromFlag == nullptr)
						{
							_waypoints->addArc(flagNode, i, distance, false);
						}
						else
						{
							helper::Vector2 flagPos = _waypoints->getNodes()[flagNode]->getPosition();
							helper::Vector2 otherPos = _waypoints->getNodes()[i]->getPosition();
							float distanceBetweenNode = (otherPos - flagPos).length();
							arcFromFlag->setWeight(distanceBetweenNode);
							arcFromFlag->setLine(flagPos, otherPos);

							GraphArc* arcToFlag = _waypoints->getArc(i, flagNode);
							arcToFlag->setWeight(distanceBetweenNode);
							arcToFlag->setLine(flagPos, otherPos);
						}
					}
					else
					{
						if (node->arcList().size() > 1)
						{
							GraphArc* arcFromFlag = _waypoints->getArc(flagNode, i);
							if (arcFromFlag != nullptr)
							{
								node->removeArc(_waypoints->getNodes()[i]);
								_waypoints->getNodes()[i]->removeArc(node);
							}
						}
					}
				}

			}
		}
	}
}

void WaypointSystem::ListenForEvents()
{
	PowerUpDestructionEvent();
}

void WaypointSystem::PowerUpDestructionEvent()
{
	if (!_interactionSystemEvents[POWER_UP_DESTRUCTION].empty())
	{
		std::vector<GraphNode*>& nodes = _waypoints->getNodes();

		for (int i = 0; i < _interactionSystemEvents[POWER_UP_DESTRUCTION].size(); i++)
		{
			PowerUpComponent* powerUp = static_cast<PowerUpComponent*>(_interactionSystemEvents[POWER_UP_DESTRUCTION].at(i).first->GetComponent(Component::Type::PowerUp));
			if (powerUp != nullptr)
			{
				nodes[powerUp->waypointIndex]->setData(make_pair(GraphNode::EntityData::Null, 0));
			}
			DestructionComponent* destruction = static_cast<DestructionComponent*>(_interactionSystemEvents[POWER_UP_DESTRUCTION].at(i).first->GetComponent(Component::Type::Destroy));
			if (destruction != nullptr)
				destruction->destroy = true;
			//this broke some how ^
			_interactionSystemEvents[POWER_UP_DESTRUCTION].erase(_interactionSystemEvents[POWER_UP_DESTRUCTION].begin() + i);
			_powerUpCount--;
			i--;
	}
		}
}

void WaypointSystem::CreatePowerUp(float dt)
{
	if (_powerUpTimer < 0)
	{
		std::vector<GraphNode*>& nodes = _waypoints->getNodes();
		
		int maxPowerUps = nodes.size() * 0.5f;
		int powerUpsToSpawn = rand() % 3 + 1;
		int guard = 0;

		while (powerUpsToSpawn != 0 && _powerUpCount < maxPowerUps)
		{
			int index = rand() % nodes.size();

			if (nodes[index]->data().first == GraphNode::EntityData::Null)
			{
				std::vector<float> data = std::vector<float>();

				int powerType = rand() % ((int)PowerUpComponent::Type::Count);
				data.push_back(powerType); //id
				data.push_back(nodes[index]->getPosition().x); //xPosition
				data.push_back(nodes[index]->getPosition().y); //yPosition
				data.push_back(index); //node index

				_creationRequests.push_back(std::pair<EntityType, std::vector<float>>(EntityType::PowerUp, data));
				nodes[index]->setData(make_pair(GraphNode::EntityData::PowerUp, powerType));

				if (NetworkHandler::Instance().GetPlayerID() != -1 && NetworkHandler::Instance().GetHost())
				{//multiplayer & host
					CreatePowerUpData createData;
					createData.index = index;
					createData.powerType = powerType;
					NetworkHandler::Instance().Send(&createData);
				}
				powerUpsToSpawn--;
				_powerUpCount++;
			}

			guard++;

			if (guard > 1000)
			{
				break;
			}
		}

		_powerUpTimer = POWER_UP_SPAWN_RATE;
	}
	else
	{
		_powerUpTimer -= dt;
	}
}