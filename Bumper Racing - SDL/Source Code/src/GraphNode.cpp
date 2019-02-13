#include "GraphNode.h"
#include "GraphArc.h"

GraphNode::GraphNode() :
	_prevNode(nullptr),
	_hCost(-1),
	_gCost(-1),
	_color(SDL_Color{ 255,255,255,255 }) 
{
}

GraphNode::~GraphNode()
{
}

GraphArc* GraphNode::getArc(GraphNode* pNode) {

	list<GraphArc>::iterator iter = _arcList.begin();
	list<GraphArc>::iterator endIter = _arcList.end();
	GraphArc* pArc = nullptr;

	// find the arc that matches the node
	for (; iter != endIter && pArc == 0; ++iter) {
		if ((*iter).node() == pNode) {
			pArc = &((*iter));
		}
	}

	// returns null if not found
	return pArc;
}


void GraphNode::reset() {
	_marked = false;
	_prevNode = nullptr;
	_hCost = -1;
	_gCost = -1;
	_color = SDL_Color{ 255,255,255,255 };
}


void GraphNode::addArc(GraphNode* pNode, float weight) {
	// Create a new arc.
	GraphArc a;
	a.setNode(pNode);
	a.setWeight(weight);
	a.setLine(_position, pNode->getPosition());
	// Add it to the arc list.
	_arcList.push_back(a);
}

void GraphNode::removeArc(GraphNode* pNode) {
	list<GraphArc>::iterator iter = _arcList.begin();


	int size = _arcList.size();
	// find the arc that matches the node
	while (iter != _arcList.end())
	{
		GraphNode* node = (*iter).node();
		
		if (node == pNode) {
			iter = _arcList.erase(iter);
			//_arcList.remove( *iter );
		}
		else
		{
			++iter;
		}

	}
	int i = 0;
}



void GraphNode::drawArcs(SDL_Renderer* renderer, Camera* camera) const {
	int arcs = _arcList.size();
	for (auto a : _arcList)
		a.draw(renderer, camera);
}


void GraphNode::drawNodes(SDL_Renderer* renderer, Camera* camera) const {
	SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
	Point point = { (float)(_position.x - 10), (float)(_position.y - 10) };
	point = camera->worldToScreen(point);

	SDL_Rect rect = { point.x,point.y, 20, 20 };
	SDL_RenderFillRect(renderer, &rect);
}


// Accessor functions
list<GraphArc> & GraphNode::arcList()  {
	return _arcList;
}

bool GraphNode::marked() const {
	return _marked;
}

pair<GraphNode::EntityData, int> const & GraphNode::data() const {
	return _data;
}
void GraphNode::setData(const pair<GraphNode::EntityData, int> &data) {
	_data = data;
}

void GraphNode::setHCost(int hCost) {
	_hCost = hCost;
}

void GraphNode::setGCost(int gCost) {
	_gCost = gCost;
}

int const & GraphNode::hCost() const {
	return _hCost;
}

int const & GraphNode::gCost() const {
	return _gCost;
}

int const & GraphNode::fCost() const {
	return _hCost + _gCost;
}

void GraphNode::setMarked(bool mark) {
	_marked = mark;
}
void GraphNode::setPrevious(GraphNode* previous) {
	_prevNode = previous;
}
void GraphNode::setColour(const SDL_Color& color) {
	_color = color;
}
void GraphNode::setPosition(helper::Vector2 newPosition) {
	_position = newPosition;
}

helper::Vector2 GraphNode::getPosition() {
	return _position;
}

GraphNode* GraphNode::getPrevious() {
	return _prevNode;
}