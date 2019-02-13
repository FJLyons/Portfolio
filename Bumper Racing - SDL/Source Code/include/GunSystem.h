#pragma once

#include "System.h"

class GunSystem : public System
{
public:
									GunSystem(std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests, float updateRate = 0.f);
									~GunSystem();

	void							Process(float dt = 0.f) override;

private:
	void							CreateBulletRequest(Entity*& e, int id);

private:
	std::vector<
		std::pair<EntityType,
			std::vector<float>>>&	_creationRequests;
	int _recoil;
};