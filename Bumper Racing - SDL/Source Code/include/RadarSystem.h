#pragma once

#include "Entity.h"

class RadarSystem
{
public:
							RadarSystem();
							~RadarSystem();

	void					Initialize(Entity* player, Entity* radar, Entity* flag, std::vector<Entity*> characters, std::vector<Entity*>	checkpoints);
	void					Process(float dt);

private:
	Entity*					_player;
	Entity*					_radar;
	Entity*					_flag;

	std::vector<Entity*>	_characters;
	std::vector<Entity*>	_checkpoints;
};

