#include "GOAP_Node.h"
#include <iostream>

int GOAPNode::m_previous_id = 0;

GOAPNode::GOAPNode()
	: m_g(0)
	, m_h(0)
{
	m_id = ++m_previous_id; // ++ before return
}

GOAPNode::GOAPNode(const GOAPWorldState state, int g, int h, int parent_id, const GOAPAction* action)
	: m_worldState(state)
	, m_g(g)
	, m_h(h)
	, m_parent_id(parent_id)
	, m_action(action)
{
	m_id = ++m_previous_id;
}

GOAPNode::~GOAPNode()
{
//	delete m_action;
}

bool operator<(const GOAPNode& lhs, const GOAPNode& rhs)
{
	return lhs.f() < rhs.f();
}
