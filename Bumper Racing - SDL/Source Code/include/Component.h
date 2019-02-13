#pragma once



class Component
{
public:
	enum class Type
	{
		Health,
		Transform,
		Physics,
		Collider,
		Sprite,
		Control,
		AI,
		Gun,
		Destroy,
		UI,
		Checkpoint,
		Flag,
		StatusEffect,
		PowerUp,
		Animation,
		Remote,
		Weapon,
		Particle
	};

public:
	Component(Type type)
		: _type(type)
	{
	}

	~Component()
	{
	}

	Type GetType() const
	{
		return _type;
	}

private:
	Type	_type;
};

