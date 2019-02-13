#pragma once
#include <deque>
#include "Component.h"

class RemoteComponent : public Component
{
public:
	struct State 
	{
		State() : xPos(-100000.f), yPos(-100000.f), xVel(0.f), yVel(0.f), ts(0.f) {}
		float ts;
		float xPos;
		float yPos;
		float xVel;
		float yVel;
	};

public:
	RemoteComponent(int _id)
		: id(_id)
		, Component::Component(Component::Type::Remote)
		, disconnected(false)
	{
	}

	~RemoteComponent()
	{
	}

public:
	std::deque<State> states;
	State startState;
	State endState;
	int		id;
	float	timeSincePacket;
	bool	disconnected;
};