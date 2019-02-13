#pragma once

#include "Unit.h"

class Factory 
{
public:
	// Return a GOAP Builder Unit
	virtual Unit* CreateGOAPBuilder() const = 0;
	// Return a GOAP Soldier Unit
	virtual Unit* CreateGOAPSoldier() const = 0;
	// Return a GOAP Archer Unit
	virtual Unit* CreateGOAPArcher() const = 0;

	// Return a MCTS Builder Unit
	virtual Unit* CreateMCTSBuilder() const = 0;
	// Return a MCTS Soldier Unit
	virtual Unit* CreateMCTSSoldier() const = 0;
	// Return a MCTS Archer Unit
	virtual Unit* CreateMCTSArcher() const = 0;
};

class UnitFactory : public Factory 
{
public:
	// Return a GOAP Builder Unit
	Unit* CreateGOAPBuilder() const
	{
		return new Unit(Team::LEFT, UnitType::BUILDER);
	}

	// Return a GOAP Soldier Unit
	Unit* CreateGOAPSoldier() const
	{
		return new Unit(Team::LEFT, UnitType::SOLDIER);
	}

	// Return a GOAP Archer Unit
	Unit* CreateGOAPArcher() const
	{
		return new Unit(Team::LEFT, UnitType::ARCHER);
	}

	// Return a MCTS Builder Unit
	Unit* CreateMCTSBuilder() const
	{
		return new Unit(Team::RIGHT, UnitType::BUILDER);
	}

	// Return a MCTS Soldier Unit
	Unit* CreateMCTSSoldier() const
	{
		return new Unit(Team::RIGHT, UnitType::SOLDIER);
	}

	// Return a MCTS Archer Unit
	Unit* CreateMCTSArcher() const
	{
		return new Unit(Team::RIGHT, UnitType::ARCHER);
	}
};