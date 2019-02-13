#include "MCTS_Planner.h"

MCTSPlanner::MCTSPlanner() :
	m_iterations(0),
	m_uct_k(sqrt(2)),
	m_maxIterations(40),
	m_timeLimit(std::chrono::milliseconds(40)),
	m_simulationDepth(4)
{
}

MCTSPlanner::~MCTSPlanner()
{
	if (ToDelete.size() > 0)
	{
		for (const auto& node : ToDelete)
		{
			if (node != NULL)
			{
				delete node;
			}
		}
	}
	ToDelete.clear();
}

const int MCTSPlanner::GetIterations() const
{
	return m_iterations;
}

MCTSNode * MCTSPlanner::GetBestUCTChild(MCTSNode * node, float uct_k) const
{
	//// sanity check
	//if (!node->IsFullyExpanded())
	//{
	//	return NULL;
	//}

	float best_utc_score = -std::numeric_limits<float>::max();

	MCTSNode* best_node = NULL;

	// iterate all immediate children and find best UTC score
	int num_children = node->GetNumChildren();

	for (int i = 0; i < num_children; i++)
	{
		MCTSNode* child = node->GetChild(i);

		// Wins
		float uct_exploitation = child->GetValue() / (child->GetNumVisits() + FLT_EPSILON);

		// Visits
		float uct_exploration = sqrt(log(node->GetNumVisits() + 1) / (child->GetNumVisits() + FLT_EPSILON));

		// Score
		float uct_score = uct_exploitation + uct_k * uct_exploration;

		if (uct_score > best_utc_score)
		{
			best_utc_score = uct_score;
			best_node = child;
		}
	}

	return best_node;
}

MCTSNode * MCTSPlanner::GetMostVisitedChild(MCTSNode * node) const
{
	int most_visits = -1;
	MCTSNode* best_node = NULL;

	// iterate all immediate children and find most visited
	int num_children = node->GetNumChildren();

	for (int i = 0; i < num_children; i++)
	{
		MCTSNode* child = node->GetChild(i);

		if (child->GetNumVisits() > most_visits)
		{
			most_visits = child->GetNumVisits();
			best_node = child;
		}
	}

	return best_node;
}

MCTSAction MCTSPlanner::Plan(MCTSWorldState& currentState)
{
	// initialize timer
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

	// initialize root MCTSNode with current state
	MCTSNode* root_node = new MCTSNode(currentState);
	ToDelete.push_back(root_node);
	MCTSNode* best_node = NULL;
	float result = 0;

	m_iterations = 0;
	while(m_iterations < m_maxIterations)
	{
		// indicate start of loop
		std::chrono::steady_clock::time_point curTime = std::chrono::steady_clock::now();
		std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - startTime);

		result = DoIteration(currentState, root_node);

		// exit loop if current run > time limit
		if (duration.count() >= m_timeLimit.count()) {  break;  }

		m_iterations++;
	}

	// Update root
	root_node->Update(result);

	// find most visited child
	best_node = GetMostVisitedChild(root_node);

	// return best node's action
	if (best_node)
	{
		return best_node->GetAction();
	}

	// we shouldn't be here
	return MCTSAction();
}

float MCTSPlanner::DoIteration(MCTSWorldState& currentState, MCTSNode* rootNode)
{
	// Clone root node for simulation
	MCTSNode* clone_node = rootNode;
	MCTSWorldState* state = &currentState;
	MCTSAction action;

	float result = 0; // win = 1, lose = 0

	// 1. SELECT
	while (!clone_node->IsTerminal() && clone_node->IsFullyExpanded())
	{
		clone_node = GetBestUCTChild(clone_node, m_uct_k);
	}

	// 2. EXPAND
	if (!clone_node->IsTerminal() && !clone_node->IsFullyExpanded())
	{
		clone_node = clone_node->Expand(); // Action applied when child added
	}

	// 3. SIMULATE
	if (!clone_node->IsTerminal())
	{
		// Limit simulation in case terminal node can't be found
		for (int i = 0; i < m_simulationDepth; i++)
		{
			if (state->IsTerminal())
			{
				break;
			}

			if (state->GetRandomAction(action))
			{
				result = state->SimulateAction(action);
			}
			else
			{
				break;
			}
		}
	}


	// 4. BACK PROPAGATION
	while (clone_node)
	{
		clone_node->Update(result);
		clone_node = clone_node->GetParent();
	}

	return result;
}