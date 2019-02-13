#include "GOAP_Action.h"
#include "GOAP_WorldState.h"

GOAPAction::GOAPAction(const ActionState& name, const int cost)
	: m_name(name)
	, m_cost(cost)
{
}

GOAPAction::~GOAPAction()
{
	if (actions != nullptr && actions->size() > 0)
	{
		for (const auto& action : *actions)
		{
			delete action;
		}
		actions->clear();
		delete actions;
	}

	m_preconditions.clear();
	m_effects.clear();
}

std::string GOAPAction::GetName() const
{
	return ResourceManager::GetInstance()->stateNames[m_name];
}

int GOAPAction::GetCost() const
{
	return m_cost;
}

void GOAPAction::SetPreconditions(ActionState key, bool value)
{
	m_preconditions[key] = value;
}

void GOAPAction::SetEffects(ActionState key, bool value)
{
	m_effects[key] = value;
}

std::unordered_map<ActionState, bool> GOAPAction::GetEffects() const
{
	return m_effects;
}

ActionState GOAPAction::GetAction() const
{
	return m_name;
}

bool GOAPAction::OperableOn(const GOAPWorldState& worldState) const
{
	for (const auto& precondition : m_preconditions)
	{
		try
		{
			if (worldState.m_variables.at(precondition.first) != precondition.second)
			{
				return false;
			}
		}
		catch (const std::out_of_range&)
		{
			return false;
		}
	}

	return true;
}

GOAPWorldState GOAPAction::ActOn(const GOAPWorldState& worldState) const
{
	GOAPWorldState temp = worldState;

	for (const auto& effect : m_effects)
	{
		temp.SetVariable(effect.first, effect.second);
	}

	return temp;
}

std::vector<GOAPAction*>* GOAPAction::SetUpAction()
{
	actions = new std::vector<GOAPAction*>();

	// idle
	GOAPAction* idle = new GOAPAction(ActionState::Idle, 1);
	idle->SetPreconditions(ActionState::Idle, true);
	idle->SetEffects(ActionState::Idle, false);
	idle->SetEffects(ActionState::Move_To_Castle, true);
	idle->SetEffects(ActionState::Move_To_Enemy, true);
	actions->push_back(idle);

	// Idle builder
	GOAPAction* idle_builder = new GOAPAction(ActionState::Idle_Builder, 1);
	idle_builder->SetPreconditions(ActionState::Idle_Builder, true);
	idle_builder->SetEffects(ActionState::Idle_Builder, false);
	idle_builder->SetEffects(ActionState::Move_To_Resource, true);
	actions->push_back(idle_builder);

	// Attack Castle
	GOAPAction* move_to_castle = new GOAPAction(ActionState::Move_To_Castle, 1);
	move_to_castle->SetPreconditions(ActionState::Move_To_Castle, true);
	move_to_castle->SetEffects(ActionState::Attack_Castle, true);
	actions->push_back(move_to_castle);

	GOAPAction* attack_castle = new GOAPAction(ActionState::Attack_Castle, 1);
	attack_castle->SetPreconditions(ActionState::Attack_Castle, true);
	attack_castle->SetPreconditions(ActionState::Castle_Dead, false);
	attack_castle->SetEffects(ActionState::Castle_Dead, true);
	actions->push_back(attack_castle);

	GOAPAction* castle_dead = new GOAPAction(ActionState::Castle_Dead, 1);
	castle_dead->SetPreconditions(ActionState::Castle_Dead, true);
	castle_dead->SetEffects(ActionState::Idle, true);
	actions->push_back(castle_dead);

	// Attack Enemy
	GOAPAction* move_to_enemy = new GOAPAction(ActionState::Move_To_Enemy, 1);
	move_to_enemy->SetPreconditions(ActionState::Move_To_Enemy, true);
	move_to_enemy->SetEffects(ActionState::Attack_Enemy, true);
	actions->push_back(move_to_enemy);

	GOAPAction* attack_enemy = new GOAPAction(ActionState::Attack_Enemy, 1);
	attack_enemy->SetPreconditions(ActionState::Attack_Enemy, true);
	attack_enemy->SetPreconditions(ActionState::Enemy_Dead, false);
	attack_enemy->SetEffects(ActionState::Enemy_Dead, true);
	actions->push_back(attack_enemy);

	GOAPAction* enemy_dead = new GOAPAction(ActionState::Enemy_Dead, 1);
	enemy_dead->SetPreconditions(ActionState::Enemy_Dead, true);
	enemy_dead->SetEffects(ActionState::Idle, true);
	actions->push_back(enemy_dead);

	// Attack Wall
	GOAPAction* move_to_structure = new GOAPAction(ActionState::Move_To_Structure, 1);
	move_to_structure->SetPreconditions(ActionState::Move_To_Structure, true);
	move_to_structure->SetEffects(ActionState::Attack_Structure, true);
	actions->push_back(move_to_structure);

	GOAPAction* attack_structure = new GOAPAction(ActionState::Attack_Structure, 1);
	attack_structure->SetPreconditions(ActionState::Attack_Structure, true);
	attack_structure->SetPreconditions(ActionState::Structure_Dead, false);
	attack_structure->SetEffects(ActionState::Structure_Dead, true);
	actions->push_back(attack_structure);

	GOAPAction* structure_dead = new GOAPAction(ActionState::Structure_Dead, 1);
	structure_dead->SetPreconditions(ActionState::Structure_Dead, true);
	structure_dead->SetEffects(ActionState::Idle, true);
	actions->push_back(structure_dead);

	// Run back to base
	GOAPAction* move_to_base = new GOAPAction(ActionState::Move_To_Base, 1);
	move_to_base->SetPreconditions(ActionState::Move_To_Base, true);
	move_to_base->SetEffects(ActionState::Base_Reached, true);
	actions->push_back(move_to_base);

	GOAPAction* base_reached = new GOAPAction(ActionState::Base_Reached, 1);
	base_reached->SetPreconditions(ActionState::Base_Reached, true);
	base_reached->SetEffects(ActionState::Move_To_Enemy, true);
	base_reached->SetEffects(ActionState::Move_To_Base, false);
	actions->push_back(base_reached);

	// Gather Resources
	GOAPAction* move_to_resources = new GOAPAction(ActionState::Move_To_Resource, 1);
	move_to_resources->SetPreconditions(ActionState::Move_To_Resource, true);
	move_to_resources->SetEffects(ActionState::Gather_Resources, true);
	actions->push_back(move_to_resources);

	GOAPAction* gather_resources = new GOAPAction(ActionState::Gather_Resources, 1);
	gather_resources->SetPreconditions(ActionState::Gather_Resources, true);
	gather_resources->SetEffects(ActionState::Resources_Collected, true);
	actions->push_back(gather_resources);

	GOAPAction* resources_collected = new GOAPAction(ActionState::Resources_Collected, 1);
	resources_collected->SetPreconditions(ActionState::Resources_Collected, true);
	resources_collected->SetEffects(ActionState::Idle_Builder, true);
	actions->push_back(resources_collected);

	// Build Wall
	GOAPAction* run_to_build = new GOAPAction(ActionState::Move_To_Site, 1);
	run_to_build->SetPreconditions(ActionState::Move_To_Site, true);
	run_to_build->SetEffects(ActionState::Build_Structure, true);
	actions->push_back(run_to_build);

	GOAPAction* build_structure = new GOAPAction(ActionState::Build_Structure, 1);
	build_structure->SetPreconditions(ActionState::Build_Structure, true);
	build_structure->SetEffects(ActionState::Structure_Built, true);
	actions->push_back(build_structure);

	GOAPAction* structure_built = new GOAPAction(ActionState::Structure_Built, 1);
	structure_built->SetPreconditions(ActionState::Structure_Built, true);
	structure_built->SetEffects(ActionState::Idle_Builder, true);
	actions->push_back(structure_built);

	return actions;
}