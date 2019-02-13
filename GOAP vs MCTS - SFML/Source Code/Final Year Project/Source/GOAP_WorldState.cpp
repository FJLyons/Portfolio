#include "GOAP_WorldState.h"

GOAPWorldState::GOAPWorldState(const std::string name)
	: m_priority(0)
	, m_name(name)
{
}

GOAPWorldState::~GOAPWorldState()
{
	m_variables.clear();
}

void GOAPWorldState::SetVariable(const int var_id, const bool value)
{
	m_variables[var_id] = value;
}

bool GOAPWorldState::GetVariable(const int var_id) const
{
	return m_variables.at(var_id);
}

bool GOAPWorldState::MeetsGoal(const GOAPWorldState& goal_state) const
{
	for (const auto& variable : goal_state.m_variables)
	{
		try
		{
			if (m_variables.at(variable.first) != variable.second)
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

int GOAPWorldState::DistanceTo(const GOAPWorldState& goal_state) const
{
	int result = 0;

	for (const auto& variable : goal_state.m_variables)
	{
		auto itr = m_variables.find(variable.first);

		if (itr == end(m_variables) || itr->second != variable.second)
		{
			++result;
		}
	}

	return result;
}

bool GOAPWorldState::operator==(const GOAPWorldState& otherWorld) const
{
	return (m_variables == otherWorld.m_variables);
}
