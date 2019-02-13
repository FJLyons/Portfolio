#include "MCTS_Action.h"

MCTSAction::MCTSAction(ActionState action) :
	m_action(action)
{
	SetUpAction();
}

MCTSAction::~MCTSAction()
{
	m_children.clear();
}

std::string MCTSAction::GetName() const
{
	return ResourceManager::GetInstance()->stateNames[m_action];
}

ActionState MCTSAction::GetAction() const
{
	return m_action;
}

void MCTSAction::AddChild(ActionState child)
{
	bool isPresent = false;
	for (int i = 0; i < m_children.size(); i++)
	{
		if (child == m_children[i])
		{
			isPresent = true;
		}
	}
	if (!isPresent)
	{
		m_children.push_back(child);
	}
}

std::vector<ActionState> MCTSAction::GetChildren()
{
	return m_children;
}

void MCTSAction::SetUpAction()
{
	// Idle
	if (m_action == ActionState::Idle)
	{
		AddChild(ActionState::Idle);
		AddChild(ActionState::Move_To_Castle);
		//AddChild(ActionState::Move_To_Enemy); 
	}

	else if (m_action == ActionState::Idle_Builder)
	{
		AddChild(ActionState::Idle_Builder);
		AddChild(ActionState::Move_To_Resource);
		//AddChild(ActionState::Move_To_Site);
	}

	// castle
	else if (m_action == ActionState::Move_To_Castle)
	{
		AddChild(ActionState::Move_To_Castle);
		AddChild(ActionState::Attack_Castle);
	}

	else if (m_action == ActionState::Attack_Castle)
	{
		AddChild(ActionState::Attack_Castle);
		AddChild(ActionState::Castle_Dead);
	}

	else if (m_action == ActionState::Castle_Dead)
	{
		AddChild(ActionState::Castle_Dead);
		AddChild(ActionState::Idle);
	}

	// Enemy
	else if (m_action == ActionState::Move_To_Enemy)
	{
		AddChild(ActionState::Move_To_Enemy);
		AddChild(ActionState::Attack_Enemy);
	}

	else if (m_action == ActionState::Attack_Enemy)
	{
		AddChild(ActionState::Attack_Enemy);
		AddChild(ActionState::Enemy_Dead);
	}

	else if (m_action == ActionState::Enemy_Dead)
	{
		AddChild(ActionState::Enemy_Dead);
		AddChild(ActionState::Idle);
	}

	// Structure
	else if (m_action == ActionState::Move_To_Structure)
	{
		AddChild(ActionState::Move_To_Structure);
		AddChild(ActionState::Attack_Structure);
	}

	else if (m_action == ActionState::Attack_Structure)
	{
		AddChild(ActionState::Attack_Structure);
		AddChild(ActionState::Structure_Dead);
	}

	else if (m_action == ActionState::Structure_Dead)
	{
		AddChild(ActionState::Structure_Dead);
		AddChild(ActionState::Idle);
	}

	// Run back to base
	else if (m_action == ActionState::Move_To_Base)
	{
		AddChild(ActionState::Move_To_Base);
		AddChild(ActionState::Base_Reached);
	}

	else if (m_action == ActionState::Base_Reached)
	{
		AddChild(ActionState::Base_Reached);
		AddChild(ActionState::Move_To_Enemy);
	}

	// Gather Resources
	else if (m_action == ActionState::Move_To_Resource)
	{
		AddChild(ActionState::Move_To_Resource);
		AddChild(ActionState::Gather_Resources);
	}

	else if (m_action == ActionState::Gather_Resources)
	{
		AddChild(ActionState::Gather_Resources);
		AddChild(ActionState::Resources_Collected);
	}

	else if (m_action == ActionState::Resources_Collected)
	{
		AddChild(ActionState::Resources_Collected);
		AddChild(ActionState::Idle_Builder);
	}

	// Build Structure
	else if (m_action == ActionState::Move_To_Site)
	{
		AddChild(ActionState::Move_To_Site);
		AddChild(ActionState::Build_Structure);
	}

	else if (m_action == ActionState::Build_Structure)
	{
		AddChild(ActionState::Build_Structure);
		AddChild(ActionState::Structure_Built);
	}

	else if (m_action == ActionState::Structure_Built)
	{
		AddChild(ActionState::Structure_Built);
		AddChild(ActionState::Idle_Builder);
	}
}
