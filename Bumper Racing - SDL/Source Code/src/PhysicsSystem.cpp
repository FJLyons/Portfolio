#include "PhysicsSystem.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "StatusEffectComponent.h"
#include "DestructionComponent.h"
#include "RemoteComponent.h"
#include "FlagComponent.h"

#include "ConstHolder.h"
#include "Helpers.h"

#include <iostream>

PhysicsSystem::PhysicsSystem(float updateRate)
	: System(updateRate)
{
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				if (e->GetType() == EntityType::Flag)
				{
					UpdateFlag(e, dt);
				}
				else
				{
					UpdateOther(e, dt);
				}
			}
		}
	}
}

void PhysicsSystem::UpdateFlag(Entity*& e, float dt)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));

	physics->xVelocity = physics->xVelocity * FLAG_DRAG;
	physics->yVelocity = physics->yVelocity * FLAG_DRAG;

	if (physics->xDir == 0 && std::abs(physics->xVelocity) <= 0.01f) { physics->xVelocity = 0.f; }
	if (physics->yDir == 0 && std::abs(physics->yVelocity) <= 0.01f) { physics->yVelocity = 0.f; }

	ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));
	TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));

	if (collider->body->IsActive())
	{
		collider->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));

		transform->rect.x = (int)metersToPixels(collider->body->GetPosition().x);
		transform->rect.y = (int)metersToPixels(collider->body->GetPosition().y);
	}
	else
	{
		collider->body->SetTransform(b2Vec2(pixelsToMeters(transform->rect.x), pixelsToMeters(transform->rect.y)), collider->body->GetAngle());
	}
}
void PhysicsSystem::UpdateOther(Entity*& e, float dt)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
	TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
	ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));

	if (e->GetType() == EntityType::Player)
	{
		StatusEffectComponent* statusEffects = static_cast<StatusEffectComponent*>(e->GetComponent(Component::Type::StatusEffect));

		float maxVelocity = physics->maxVelocity;

		float xDrag = (physics->xDir == 0) ? -physics->xVelocity * DRAG : 0.f;
		float yDrag = (physics->yDir == 0) ? -physics->yVelocity * DRAG : 0.f;

		physics->xVelocity += (xDrag + (physics->xDir * physics->xAcceleration)) * dt;//change dt to _updateRate?//maybe?
		physics->yVelocity += (yDrag + (physics->yDir * physics->yAcceleration)) * dt;


		float xAccel = physics->xAcceleration;
		float yAccel = physics->yAcceleration;

		if (statusEffects->speedUp)
		{
			xAccel += xAccel;
			yAccel += yAccel;
		}

		physics->xVelocity += (xDrag + (physics->xDir * xAccel)) * dt;//change dt to _updateRate?//maybe?
		physics->yVelocity += (yDrag + (physics->yDir * yAccel)) * dt;

		float currentVelocity = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);
		
		if (currentVelocity > maxVelocity * 0.01f)
		{
			transform->angle = atan2(physics->yVelocity, physics->xVelocity) * 180.f / M_PI;
		}
		else if (currentVelocity > maxVelocity)
		{
			physics->xVelocity = (physics->xVelocity / currentVelocity) * maxVelocity;
			physics->yVelocity = (physics->yVelocity / currentVelocity) * maxVelocity;
		}

		if (physics->xDir == 0 && std::abs(physics->xVelocity) <= 0.01f) { physics->xVelocity = 0.f; }
		if (physics->yDir == 0 && std::abs(physics->yVelocity) <= 0.01f) { physics->yVelocity = 0.f; }
	}
	else if (e->GetType() == EntityType::AI)
	{
		StatusEffectComponent* statusEffects = static_cast<StatusEffectComponent*>(e->GetComponent(Component::Type::StatusEffect));

		if (statusEffects->staggered)
		{
			float xDrag = -physics->xVelocity * DRAG;
			float yDrag = -physics->yVelocity * DRAG;

			physics->xVelocity += xDrag * dt;
			physics->yVelocity += yDrag * dt;
		}

		float maxVelocity = physics->maxVelocity;

		float currentVelocity = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);
		if (currentVelocity > maxVelocity)
		{
			physics->xVelocity = (physics->xVelocity / currentVelocity) * maxVelocity;
			physics->yVelocity = (physics->yVelocity / currentVelocity) * maxVelocity;

			currentVelocity = maxVelocity;
		}

		if (currentVelocity > maxVelocity * 0.01f)
		{
			transform->angle = atan2(physics->yVelocity, physics->xVelocity) * 180.f / M_PI;
		}

		collider->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));
	}
	else if (e->GetType() == EntityType::Bullet)
	{
		physics->xVelocity = physics->xVelocity * BULLET_DRAG;
		physics->yVelocity = physics->yVelocity * BULLET_DRAG;

		float currentVelocity = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);
		if (currentVelocity < MAX_PLAYER_VELOCITY)
		{
			static_cast<DestructionComponent*>(e->GetComponent(Component::Type::Destroy))->destroy = true;
		}
	}
	if (collider->body->IsActive())
	{
		collider->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));

		transform->rect.x = (int)metersToPixels(collider->body->GetPosition().x);
		transform->rect.y = (int)metersToPixels(collider->body->GetPosition().y);
	}
	else
	{
		transform->rect.x += physics->xVelocity * dt;
		transform->rect.y += physics->yVelocity * dt;

		collider->body->SetTransform(b2Vec2(pixelsToMeters(transform->rect.x), pixelsToMeters(transform->rect.y)), collider->body->GetAngle());
	}

	if (e->GetType() == EntityType::RemotePlayer)
	{
		RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));
		if (remote->disconnected)
		{
			collider->body->SetTransform(b2Vec2(-10000, -10000), 0.f);
			collider->setActive = false;
			transform->rect.x = (int)metersToPixels(collider->body->GetPosition().x);
			transform->rect.y = (int)metersToPixels(collider->body->GetPosition().y);
			FlagComponent* flag = static_cast<FlagComponent*>(e->GetComponent(Component::Type::Flag));
			flag->hasFlag = false;
		}
	}
}

/*
TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));
PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));

if (e->GetType() != EntityType::AI && e->GetType() != EntityType::Bullet && e->GetType() != EntityType::Flag)
{
StatusEffectComponent* statusEffects = static_cast<StatusEffectComponent*>(e->GetComponent(Component::Type::StatusEffect));

float maxVelocity = physics->maxVelocity;

float xDrag = (physics->xDir == 0) ? -physics->xVelocity * DRAG : 0.f;
float yDrag = (physics->yDir == 0) ? -physics->yVelocity * DRAG : 0.f;


float xAccel = physics->xAcceleration;
float yAccel = physics->yAcceleration;

if (statusEffects != nullptr && statusEffects->speedUp)
{
xAccel += xAccel;
yAccel += yAccel;
}

physics->xVelocity += (xDrag + (physics->xDir * xAccel)) * dt;//change dt to _updateRate?//maybe?
physics->yVelocity += (yDrag + (physics->yDir * yAccel)) * dt;

float currentVelocity = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);

if (e->GetType() == EntityType::Player && currentVelocity > maxVelocity * 0.01f)
{
transform->angle = atan2(physics->yVelocity, physics->xVelocity) * 180.f / M_PI;
}
else if (currentVelocity > maxVelocity)
{
physics->xVelocity = (physics->xVelocity / currentVelocity) * maxVelocity;
physics->yVelocity = (physics->yVelocity / currentVelocity) * maxVelocity;
}

if (physics->xDir == 0 && std::abs(physics->xVelocity) <= 0.01f) { physics->xVelocity = 0.f; }
if (physics->yDir == 0 && std::abs(physics->yVelocity) <= 0.01f) { physics->yVelocity = 0.f; }

if (e->GetType() == EntityType::Player)
{
physics->xDir = 0;
physics->yDir = 0;
}
}
else
{
if (e->GetType() == EntityType::AI)
{
float maxVelocity = physics->maxVelocity;

if (physics->xVelocity + physics->yVelocity != 0)
{
transform->angle = atan2(physics->yVelocity, physics->xVelocity) * 180.f / M_PI;
}

float currentVelocity = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);
if (currentVelocity > maxVelocity)
{
physics->xVelocity = (physics->xVelocity / currentVelocity) * maxVelocity;
physics->yVelocity = (physics->yVelocity / currentVelocity) * maxVelocity;
}

collider->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));
}
else if (e->GetType() == EntityType::Bullet)
{
physics->xVelocity = physics->xVelocity * BULLET_DRAG;
physics->yVelocity = physics->yVelocity * BULLET_DRAG;

float currentVelocity = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);

if (currentVelocity < MAX_BULLET_VELOCITY * 0.25f)
{
static_cast<DestructionComponent*>(e->GetComponent(Component::Type::Destroy))->destroy = true;
}
}
else if (e->GetType() == EntityType::Flag)
{
physics->xVelocity = physics->xVelocity * DRAG;
physics->yVelocity = physics->yVelocity * DRAG;

if (physics->xDir == 0 && std::abs(physics->xVelocity) <= 0.01f) { physics->xVelocity = 0.f; }
if (physics->yDir == 0 && std::abs(physics->yVelocity) <= 0.01f) { physics->yVelocity = 0.f; }
}
}

if (collider->body->IsActive())
{
collider->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));

transform->rect.x = (int)metersToPixels(collider->body->GetPosition().x);
transform->rect.y = (int)metersToPixels(collider->body->GetPosition().y);
}
else
{
transform->rect.x += physics->xVelocity;
transform->rect.y += physics->yVelocity;

collider->body->SetTransform(b2Vec2(pixelsToMeters(transform->rect.x), pixelsToMeters(transform->rect.y)), collider->body->GetAngle());
}
}
*/