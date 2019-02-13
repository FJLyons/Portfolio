#include "MCTS_WorldState.h"
#include "Manager_Resource.h"

#include "Manager_Collision.h"


MCTSWorldState::MCTSWorldState() :
	m_isTerminal(false),
	m_winner(0),
	m_previousAction(MCTSAction(ActionState::Idle))
{
	// Add available actions
	for (int i = 0; i < m_previousAction.GetChildren().size(); i++)
	{
		m_moves.push_back(m_previousAction.GetChildren()[i]);
	}
}

MCTSWorldState::MCTSWorldState(const MCTSAction& action) :
	m_isTerminal(false),
	m_winner(0),
	m_previousAction(action)
{
	// Add available actions
	for (int i = 0; i < m_previousAction.GetChildren().size(); i++)
	{
		m_moves.push_back(m_previousAction.GetChildren()[i]);
	}
}


MCTSWorldState::~MCTSWorldState()
{
	reset();
	m_moves.clear();
}

bool MCTSWorldState::IsTerminal() const
{
	return m_isTerminal;
}

float MCTSWorldState::SimulateAction(const MCTSAction & action)
{
	m_previousAction = action;
	m_winner = 0;

	switch (action.GetAction())
	{
	case ActionState::Idle:						m_winner = CollisionManager::GetInstance()->MCTS_Idle();					break;

	case ActionState::Move_To_Castle:			m_winner = CollisionManager::GetInstance()->MCTS_MoveToCastle(m_id);		break;
	case ActionState::Attack_Castle:			m_winner = CollisionManager::GetInstance()->MCTS_AttackCastle(m_id);		break;
	case ActionState::Castle_Dead:				m_winner = CollisionManager::GetInstance()->MCTS_CastleDead(); m_isTerminal = true;				break;

	case ActionState::Move_To_Enemy:			m_winner = CollisionManager::GetInstance()->MCTS_MoveToEnemy(m_id);			break;
	case ActionState::Attack_Enemy:				m_winner = CollisionManager::GetInstance()->MCTS_AttackEnemy(m_id);			break;
	case ActionState::Enemy_Dead:				m_winner = CollisionManager::GetInstance()->MCTS_EnemyDead(m_id); 			break;

	case ActionState::Move_To_Structure:		m_winner = CollisionManager::GetInstance()->MCTS_MoveToStructure(m_id);		break;
	case ActionState::Attack_Structure:			m_winner = CollisionManager::GetInstance()->MCTS_AttackStructure(m_id);		break;
	case ActionState::Structure_Dead:			m_winner = CollisionManager::GetInstance()->MCTS_StructureDead(m_id); 		break;

	case ActionState::Move_To_Base:				m_winner = CollisionManager::GetInstance()->MCTS_MoveToBase();				break;
	case ActionState::Base_Reached:				m_winner = CollisionManager::GetInstance()->MCTS_BaseReached(m_id); 		break;

	case ActionState::Idle_Builder:				m_winner = CollisionManager::GetInstance()->MCTS_Idle();					break;

	case ActionState::Move_To_Resource:			m_winner = CollisionManager::GetInstance()->MCTS_MoveToResource(m_id);		break;
	case ActionState::Gather_Resources:			m_winner = CollisionManager::GetInstance()->MCTS_GatherResources(m_id);		break;
	case ActionState::Resources_Collected:		m_winner = CollisionManager::GetInstance()->MCTS_ResourcesCollected(m_id);	break;

	case ActionState::Move_To_Site:				m_winner = CollisionManager::GetInstance()->MCTS_MoveToSite(m_id);			break;
	case ActionState::Build_Structure:			m_winner = CollisionManager::GetInstance()->MCTS_BuildStructure(m_id);		break;
	case ActionState::Structure_Built:			m_winner = CollisionManager::GetInstance()->MCTS_StuctureBuilt(m_id);		break;
	}

	return Evaluate();
}

const float MCTSWorldState::Evaluate()
{
	if (m_winner == 1)
	{
		return 1; // if good move
	}
	else if (m_winner == 0)
	{
		return 0.5f; // if draw
	}
	else
	{
		return 0; // if bad move
	}
}

void MCTSWorldState::GetActions(std::vector<MCTSAction>& actions) const
{
	actions.clear();

	for (int i = 0; i < m_moves.size(); i++)
	{
		actions.push_back(m_moves[i]);
	}
}

bool MCTSWorldState::GetRandomAction(MCTSAction & action) const
{
	std::vector<MCTSAction> actions;
	GetActions(actions);

	// sanity check
	if (actions.empty())
	{
		return false;
	}

	action = actions[floor(std::rand() % actions.size())]; // Random shuffle
	return true;
}

void MCTSWorldState::reset()
{
	m_isTerminal = false;
	m_winner = 0;
}

int MCTSWorldState::GetID()
{
	return m_id;
}

void MCTSWorldState::SetID(const int newID)
{
	m_id = newID;
}
