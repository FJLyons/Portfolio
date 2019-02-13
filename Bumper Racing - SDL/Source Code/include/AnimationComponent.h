#pragma once

#include "Component.h"


class AnimationComponent : public Component
{
public:
	enum class State
	{
		Idle,
		Slow,
		Medium,
		Fast, 
		TurningLeft,
		TurningRight,
		Bumping,
		Staggered
	};

	AnimationComponent()
		: state(State::Idle)
		, Component::Component(Component::Type::Animation)
	{
	}

	~AnimationComponent()
	{
	}

	State state;
	float counter = 0;
	float bumpingCounter = 0;
};
