#include "AISystem.h"
#include "StatusEffectComponent.h"
#include "PhysicsComponent.h"
#include "GunComponent.h"
#include "NetworkHandler.h"
#include "WeaponComponent.h"
#include "RemoteComponent.h"

AISystem::AISystem(float updateRate)
	: System(updateRate)
{
}

AISystem::~AISystem()
{

}

void AISystem::Initialize(Graph* waypoints, Entity* player)
{
	_waypoints = waypoints;
	vector<GraphNode*> nodes = _waypoints->getNodes();
	int size = nodes.size();
	for (int i = 0; i < size; i++)
	{
		if (nodes[i]->data().first == GraphNode::EntityData::Checkpoint)              
			_checkpointNode.push_back(nodes[i]);
	}
	_flagNode = _waypoints->getNodes()[size - 1];
	_players.push_back(player);

}


void AISystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				AIComponent* ai = static_cast<AIComponent*>(e->GetComponent(Component::Type::AI));
				TransformComponent* t = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
				PhysicsComponent* p = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
				WeaponComponent* w = static_cast<WeaponComponent*>(e->GetComponent(Component::Type::Weapon));
				FlagComponent* f = static_cast<FlagComponent*>(e->GetComponent(Component::Type::Flag));

				
				helper::Vector2 velocity = helper::Vector2(0, 0);

				helper::Vector2 AIPosition = helper::Vector2(t->rect.x, t->rect.y);
				
				switch (ai->state)
				{
				case AIState::Entry:
					Entry(ai, t);
					break;
				case AIState::SeekFlag:
				{
					//don't have flag, check for distance to flag and then set go to node!
					float distanceToFlag = AIPosition.distance(ai->destNode->getPosition());
					if (distanceToFlag < AI_CHASE_RADIUS)
					{
						ai->state = AIState::Chase;
						ai->destNode = _flagNode;
					}
					else
					{
						if (ai->nextNode == ai->destNode)
						{
							GraphNode* closestNode = FindClosestNode(ai, t);
							if (closestNode != nullptr)
								ai->nextNode = closestNode;
						}
						
						Avoidance(e, velocity, AIPosition, AI_WAYPOINT_FORCE + ai->avoidanceSeekForce);

						Entity* entityWithFlag = FindEntityWithFlag(e);
						if (entityWithFlag != nullptr)
						{
							Prediction(entityWithFlag, ai, t);

							if (!w->hasWeapon)
							{
								GraphNode* powerUpNode = FindClosestPowerUp(velocity, ai, t);
								if (powerUpNode != nullptr)
								{
									ai->state = AIState::SeekPowerUp;
									ai->destNode = powerUpNode;
								}
							}
						}
					}
					
					AStarUpdate(dt, ai, AIPosition);
					//learning
					ai->avoidanceSeekTimer -= dt;
					if (ai->avoidanceSeekTimer < -AI_MAX_ABVOIDANCE_TIMER)
					{
						if(ai->avoidanceSeekForce > -AI_MAX_ABVOIDANCE_SEEK)
							ai->avoidanceSeekForce -= 0.02f;
						ai->avoidanceSeekTimer = 0;
					}
					break;
				}
				case AIState::Chase:
				{
					if (!f->hasFlag) //don't have flag
					{
						float distanceToFlag = AIPosition.distance(ai->destNode->getPosition());
						if (distanceToFlag > AI_CHASE_RADIUS) //outside of chase range
						{
							//transition back to seekFlag
							ai->state = AIState::SeekFlag;
							ai->destNode = _flagNode;	
							AStar(ai);
							ai->pathfinderUpdateTimer = 0;
						}
						Entity* entityWithFlag = FindEntityWithFlag(e);
						if (entityWithFlag != nullptr)
						{
							//loop through all players and calculate distance and then add avoidance force
							for (int i = 0; i < _players.size(); i++)
							{
								if (entityWithFlag != _players[i])
								{
									TransformComponent* playerTransfrom = static_cast<TransformComponent*>(_players[i]->GetComponent(Component::Type::Transform));
									velocity += CalculateAvoidanceForce(AIPosition, helper::Vector2(playerTransfrom->rect.x, playerTransfrom->rect.y), AI_WAYPOINT_FORCE + ai->avoidanceChaseForce);
								}
							}
							//loop through all AI and calculate distance and then add avoidance force
							for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
							{
								for (Entity* otherAI : (*it).second)
								{
									if (e != otherAI && entityWithFlag != otherAI) //check if not self.
									{
										TransformComponent* otherAITransfrom = static_cast<TransformComponent*>(otherAI->GetComponent(Component::Type::Transform));
										velocity += CalculateAvoidanceForce(AIPosition, helper::Vector2(otherAITransfrom->rect.x, otherAITransfrom->rect.y), AI_WAYPOINT_FORCE + ai->avoidanceChaseForce);
									}
								}
							}
						}
					}
					else
					{
						//has flag, transition to seekCheckpoint			
						int size = _checkpointNode.size();
						for (int i = 0; i < size; i++)
						{
							if (f->currentCheckpointID + 1 == _checkpointNode[i]->data().second)
							{
								ai->destNode = _checkpointNode[i];
								break;
							}
						}
						GraphNode* closestNode = FindClosestNode(ai, t);
						if (closestNode != nullptr)
							ai->nextNode = closestNode;

						ai->pathfinderUpdateTimer = 0;
						AStar(ai);   //manual call on AStar.

						ai->state = AIState::SeekCheckpoint;
					}
					ai->avoidanceChaseTimer -= dt;
					if (ai->avoidanceChaseTimer < -AI_MAX_ABVOIDANCE_TIMER)
					{
						if (ai->avoidanceChaseForce > -AI_MAX_ABVOIDANCE_CHASE)
							ai->avoidanceChaseForce -= 0.01f;
						ai->avoidanceChaseTimer = 0;
					}
					break;
				}
				case AIState::Camp:
				{
					float distance = _flagNode->getPosition().distance(AIPosition);
					if (ai->nextNode == ai->destNode)
					{			

						if (distance < AI_CAMP_FLAG_DETECTION_RANGE)
						{
							ai->destNode = _flagNode;
							GraphNode* closestNode = FindClosestNode(ai, t);
							if (closestNode != nullptr)
								ai->nextNode = closestNode;
							ai->state = AIState::SeekFlag;
							
						}	
					}
					Entity* entityWithFlag = FindEntityWithFlag(e);
					if (entityWithFlag != nullptr)
					{
						if (entityWithFlag != ai->lastEntityWithFlag)
						{
							Prediction(entityWithFlag, ai, t);
						}
					}
					else if (entityWithFlag == nullptr)
					{
						ai->destNode = _flagNode;
						GraphNode* closestNode = FindClosestNode(ai, t);
						if (closestNode != nullptr)
							ai->nextNode = closestNode;
						ai->state = AIState::SeekFlag;
					}
					
					if (distance < AI_CHASE_RADIUS)
					{
						ai->destNode = _flagNode;
						GraphNode* closestNode = FindClosestNode(ai, t);
						if (closestNode != nullptr)
							ai->nextNode = closestNode;
						ai->state = AIState::SeekFlag;
					}

					Avoidance(e, velocity, AIPosition, AI_WAYPOINT_FORCE + 2.0f );
					
					break;
				}
				case AIState::SeekCheckpoint:
				{
					SeekCheckpoint(ai, f, t);
					Avoidance(e, velocity, AIPosition, AI_WAYPOINT_FORCE + ai->avoidanceCheckpointForce);
					AStarUpdate(dt, ai, AIPosition);
					break;
				}
				case AIState::SeekPowerUp:
				{				
					Avoidance(e, velocity, AIPosition, AI_WAYPOINT_FORCE);
					//update Astar
					AStarUpdate(dt, ai, AIPosition);

					if (ai->destNode->data().first == GraphNode::EntityData::Null) //someone took power, return to seek
					{
						
						ai->destNode = _flagNode;
						ai->state = AIState::SeekFlag;

					}
					ai->avoidanceCheckpointTimer -= dt;
					if (ai->avoidanceCheckpointTimer < -AI_MAX_ABVOIDANCE_TIMER)
					{
						if(ai->avoidanceCheckpointForce > -AI_MAX_ABVOIDANCE_CHECKPOINT)
							ai->avoidanceCheckpointForce -= 0.02f;
						ai->avoidanceCheckpointTimer = 0;
					}
					break;
				}
				default:
					break;
				}

				
				

				if (!ai->path.empty())
					TranverseNode(ai, t);


				if (ai->nextNode != nullptr)
				{
					helper::Vector2 nextNodeVector2 = ai->nextNode->getPosition() - AIPosition;
					helper::Vector2 waypointDirection = nextNodeVector2.normalize();
					if(ai->state == AIState::Chase || ai->state == AIState::SeekFlag)
						velocity += waypointDirection * (AI_WAYPOINT_FORCE + 1.0f);
					else
						velocity += waypointDirection * AI_WAYPOINT_FORCE;
				}
				

				float maxVelocity = 9.0f;
				float currentVelocity = sqrt(velocity.x* velocity.x + velocity.y * velocity.y);
				if (currentVelocity > maxVelocity)
				{
					velocity = velocity.normalize() * maxVelocity;
				}

				StatusEffectComponent* statusEffect = static_cast<StatusEffectComponent*>(e->GetComponent(Component::Type::StatusEffect));
				if (!statusEffect->staggered)
				{
					float xDrag = -p->xVelocity * DRAG;
					float yDrag = -p->yVelocity * DRAG;
					p->xVelocity += (velocity.x + xDrag) * dt;
					p->yVelocity += (velocity.y + yDrag) * dt;
				}	
				
				if (!statusEffect->speedUp)
				{
					velocity = velocity * 2;
				}

				if (w->hasWeapon && !statusEffect->staggered)
				{
					Entity* entityWithFlag = FindEntityWithFlag(e);
					if (entityWithFlag != nullptr)
					{
						TransformComponent* otherTransform = static_cast<TransformComponent*>(entityWithFlag->GetComponent(Component::Type::Transform));

						if (AIPosition.distance(helper::Vector2(otherTransform->rect.x, otherTransform->rect.y)) < AI_FIRING_RADIUS)
						{
	

							helper::Vector2 directionToFlag = helper::Vector2(otherTransform->rect.x, otherTransform->rect.y) - AIPosition;
							float angleInRad = atan2(directionToFlag.y, directionToFlag.x);
							float angleInDeg = angleInRad * 180.0f / PI;
							if (t->angle > angleInDeg - 20.0f && t->angle < angleInDeg + 20.0f)
							{
								w->fired = true;
								if (NetworkHandler::Instance().GetPlayerID() != -1)
								{
									RemoteComponent* remote = static_cast<RemoteComponent*>(entityWithFlag->GetComponent(Component::Type::Remote));
									FireData data;
									data.remoteID = remote->id;
									NetworkHandler::Instance().Send(&data);
									std::cout << "sending fire data" << std::endl;
								}
							}
						}
						
					}
				}
				UpdateLearning(dt, ai);
			}		
		}
	}
}



void AISystem::UpdateLearning(float dt, AIComponent* ai)
{
	

	

	


}



void AISystem::Entry(AIComponent* ai, TransformComponent* t)
{
	//transition to SeekFlag
	ai->state = AIState::SeekFlag;
	GraphNode* closestNode = FindClosestNode(ai, t);
	if (closestNode != nullptr)
		ai->nextNode = closestNode; //AI doesn't know starting node, so we search for the closest one. this is a manual update.
	ai->destNode = _flagNode;
	//ai->pathfinderUpdateTimer = 0;
	//AStar(ai);
}

void AISystem::Prediction(Entity* entityWithFlag, AIComponent* ai, TransformComponent* t)
{
	FlagComponent* f = static_cast<FlagComponent*>(entityWithFlag->GetComponent(Component::Type::Flag));
	int size = _checkpointNode.size();
	int target = f->currentCheckpointID + 1;
	for (int i = 0; i < size; i++)
	{
		if (target == _checkpointNode[i]->data().second)
		{

			float selfDistanceToCheckpoint = _checkpointNode[i]->getPosition().distance(helper::Vector2(t->rect.x, t->rect.y));

			TransformComponent* entityWIthTransform = static_cast<TransformComponent*>(entityWithFlag->GetComponent(Component::Type::Transform));
			float otherDistanceToCheckpoint = _checkpointNode[i]->getPosition().distance(helper::Vector2(entityWIthTransform->rect.x, entityWIthTransform->rect.y));
			float weighDistance = selfDistanceToCheckpoint - otherDistanceToCheckpoint; //difference between entity pos to checkpoint and entity with flag pos to checkpoint
			if (weighDistance > AI_TO_CHASE_RANGE_OFFSET)
			{
				target++;
				if (target > 4)
					target = 1;
				for (int j = 0; j < size; j++)
				{
					if (target == _checkpointNode[j]->data().second)
					{
						ai->destNode = _checkpointNode[j];
						break;
					}
				}
				GraphNode* closestNode = FindClosestNode(ai, t);
				if(closestNode != nullptr)
					ai->nextNode = closestNode;
				AStar(ai);
				ai->state = AIState::Camp;
				ai->lastEntityWithFlag = entityWithFlag;
			}
			break;
		}
	}
	
}

void AISystem::SeekCheckpoint(AIComponent* ai, FlagComponent* f, TransformComponent* t)
{
	if (f->hasFlag)
	{
		//keep going to checkpoint and check collision with it.
		float distanceToCheckpoint = ai->destNode->getPosition().distance(helper::Vector2(t->rect.x, t->rect.y));
		if (distanceToCheckpoint < CHECKPOINT_COLLISION_RADIUS)
		{
			ai->nextNode = ai->destNode;
			//reach checkpoint, increase and search for next waypoint node.
			int size = _checkpointNode.size();
			for (int i = 0; i < size; i++)
			{
				if (f->currentCheckpointID + 1== _checkpointNode[i]->data().second)
				{
					ai->destNode = _checkpointNode[i];
					break;
				}
			}		
			AStar(ai);   //manual call on AStar.
			ai->pathfinderUpdateTimer = 0;
		}
	}
	else
	{
		//lose flag, transition to seek flag 
		ai->state = AIState::SeekFlag;
		ai->destNode = _flagNode;
		GraphNode* closestNode = FindClosestNode(ai, t);
		if (closestNode != nullptr)
			ai->nextNode = closestNode;
		AStar(ai);
		ai->pathfinderUpdateTimer = 0;
	}
}

void AISystem::Avoidance(Entity* self, helper::Vector2& velocity, const helper::Vector2& AIPosition, float force)
{
	//loop through all players and calculate distance and then add avoidance force
	for (int i = 0; i < _players.size(); i++)
	{
		TransformComponent* playerTransfrom = static_cast<TransformComponent*>(_players[i]->GetComponent(Component::Type::Transform));
		velocity += CalculateAvoidanceForce(AIPosition, helper::Vector2(playerTransfrom->rect.x, playerTransfrom->rect.y), force);
	}
	//loop through all AI and calculate distance and then add avoidance force
	for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
	{
		for (Entity* otherAI : (*it).second)
		{
			if (self != otherAI) //check if not self.
			{
				TransformComponent* otherAITransfrom = static_cast<TransformComponent*>(otherAI->GetComponent(Component::Type::Transform));
				velocity += CalculateAvoidanceForce(AIPosition, helper::Vector2(otherAITransfrom->rect.x, otherAITransfrom->rect.y), force);
			}
		}
	}
}

helper::Vector2 AISystem::CalculateAvoidanceForce(const helper::Vector2& AIPosition, const helper::Vector2& otherPosition, float force)
{
	helper::Vector2 velocity = helper::Vector2(0, 0);
	helper::Vector2 AB = AIPosition - otherPosition;
	if (AB.length() < AI_AVOIDANCE_RADIUS) // distance to AI
	{
		helper::Vector2 direction = AB.normalize();
		velocity += direction * force;
	}
	return velocity;
}

GraphNode* AISystem::FindClosestNode(AIComponent* ai, TransformComponent* t)
{
	//loop through the nodes and determine the closest node.
	vector<GraphNode*> nodes = _waypoints->getNodes();

	int closestNodeIndex = 0;
	float closestNodeDistance = 99999999.f;

	int size = nodes.size() - 1;
	for (int i = 0; i < size; i++)
	{
		float distance = (nodes[i]->getPosition() - helper::Vector2(t->rect.x, t->rect.y)).length();
		if (distance < closestNodeDistance)
		{
			if (distance < AI_CHASE_RADIUS)
			{
				closestNodeDistance = distance;
				closestNodeIndex = i;
			}
		}
	}
	if (ai->nextNode != nodes[closestNodeIndex] && closestNodeDistance < 99999990.f)
		return nodes[closestNodeIndex];
	else
		return nullptr;
}

GraphNode* AISystem::FindClosestPowerUp(helper::Vector2& velocity, AIComponent* ai, TransformComponent* t)
{
	std::vector<GraphNode*> nodes = _waypoints->getNodes();
	int size = _waypoints->getNodesSize();
	int closestIndex;
	float closestPowerup = 999999999.0f;
	for (int i = 0; i < size; i++)
	{
		if (nodes[i]->data().first == GraphNode::EntityData::PowerUp)
		{

			float distanceToPowerUp = nodes[i]->getPosition().distance(helper::Vector2(t->rect.x, t->rect.x));
			if (distanceToPowerUp < closestPowerup)
			{
				if (distanceToPowerUp < AI_POWER_UP_RADIUS)
				{
					closestPowerup = distanceToPowerUp;
					closestIndex = i;
				}
			}
		}
	}

	if (closestPowerup < 99999990.f)
		return nodes[closestIndex];
	else
		return nullptr;

}

void AISystem::TranverseNode(AIComponent* ai, TransformComponent* t)
{
	float distance = ai->nextNode->getPosition().distance(helper::Vector2(t->rect.x, t->rect.y));
	if (distance < AI_NODE_COLLISION_RADIUS)
	{
		ai->path.erase(ai->path.begin());
		if(!ai->path.empty())
			ai->nextNode = ai->path.front();
		else
			ai->nextNode = ai->destNode;
	}
}

void AISystem::AStarUpdate(float dt, AIComponent* ai,  helper::Vector2& AIPosition)
{
	//update Astar
	float updateTimer = ai->pathfinderUpdateTimer;
	updateTimer += dt;
	if (updateTimer > AI_PATHFINDING_UPDATE_RATE)
	{
		updateTimer -= AI_PATHFINDING_UPDATE_RATE;
		AStar(ai);
		//fast check to wipe out nodes in radius
		/*
		for (int i = 0; i < ai->path.size(); i++)
		{
			float distanceToNode = AIPosition.distance(ai->path[i]->getPosition());
			if (distanceToNode < AI_NODE_COLLISION_RADIUS)
			{
				ai->path.erase(ai->path.begin());
				ai->nextNode = ai->path.front();
			}
		}*/
	}
	ai->pathfinderUpdateTimer = updateTimer;
}

void AISystem::AStar(AIComponent* ai)
{
	vector<GraphNode*> path;

	_waypoints->reset();
	_waypoints->setHeuristics(ai->nextNode);

	//call AStar
	_waypoints->aStar(ai->nextNode, ai->destNode, path);
	//set colour
	ai->destNode->setColour(SDL_Color{ 255,0,0,255 });
	ai->nextNode->setColour(SDL_Color{ 0,255,0,255 });

	if (!path.empty())
	{
		ai->path = path;
	}
}

Entity* AISystem::FindEntityWithFlag(Entity* self)
{
	Entity* entityWithFlag = nullptr;

	//loop through players and find player with flag
	int size = _players.size();
	for (int i = 0; i < size; i++)
	{
		FlagComponent* f = static_cast<FlagComponent*>(_players[i]->GetComponent(Component::Type::Flag));
		if (f->hasFlag)
		{
			entityWithFlag = _players[i];
			break;
		}
	}

	//loop through AI and find AI with flag
	for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
	{
		for (Entity* AIEntities : (*it).second)
		{
			if (self != AIEntities)
			{
				FlagComponent* f = static_cast<FlagComponent*>(AIEntities->GetComponent(Component::Type::Flag));
				if (f->hasFlag)
				{
					entityWithFlag = AIEntities;
					break;
				}
			}
		}
	}

	return entityWithFlag;
}