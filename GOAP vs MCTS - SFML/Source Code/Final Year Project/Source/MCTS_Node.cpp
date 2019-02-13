#include "MCTS_Node.h"

MCTSNode::MCTSNode(const MCTSWorldState& state, MCTSNode* parent) :
	m_state(state),
	m_action(),
	m_parent(parent),
	m_num_visits(0),
	m_value(0),
	m_depth(parent ? parent->m_depth + 1 : 0)
{
}

MCTSNode::~MCTSNode()
{
	m_children.clear();
	m_actions.clear();
}

MCTSNode * MCTSNode::Expand()
{
	// sanity check that we're not already fully expanded
	if (IsFullyExpanded())
	{
		return NULL;
	}

	// sanity check that we don't have more children than we do actions
	//assert(m_children.size() < m_actions.size());

	// if this is the first expansion and we haven't yet got all of the possible actions
	if (m_actions.empty())
	{
		// retrieve list of actions from the state
		m_state.GetActions(m_actions);

		// randomize the order
		std::random_shuffle(m_actions.begin(), m_actions.end());
	}

	// called if no real terminal state
	if (m_actions.empty())
	{
		int test = 0;
	}

	// add the next action in queue as a child
	return AddChildWithAction(m_actions[m_children.size()]);
}

void MCTSNode::Update(float & rewards)
{
	this->m_value += rewards;
	m_num_visits++;
}

const MCTSWorldState & MCTSNode::GetState() const
{
	return m_state;
}

const MCTSAction & MCTSNode::GetAction() const
{
	return m_action;
}

bool MCTSNode::IsFullyExpanded() const
{
	return m_children.empty() == false && m_children.size() == m_actions.size();
}

bool MCTSNode::IsTerminal() const
{
	return m_state.IsTerminal();
}

int MCTSNode::GetNumVisits() const
{
	return m_num_visits;
}

float MCTSNode::GetValue() const
{
	return m_value;
}

int MCTSNode::GetDepth() const
{
	return m_depth;
}

int MCTSNode::GetNumChildren() const
{
	return m_children.size();
}

MCTSNode * MCTSNode::GetChild(int i) const
{
	return m_children[i].get();
}

MCTSNode *  MCTSNode::GetParent() const
{
	return m_parent;
}

MCTSNode *  MCTSNode::AddChildWithAction(const MCTSAction & new_action)
{
	// create a new TreeNode with the same state (will get cloned) as this TreeNode
	MCTSNode* child_node(new MCTSNode(m_state, this));

	// set the action of the child to be the new action
	child_node->m_action = new_action;

	// apply the new action to the state of the child TreeNode
	child_node->m_state.SimulateAction(new_action);

	// add to children
	m_children.push_back(Ptr(child_node));

	return child_node;
}
