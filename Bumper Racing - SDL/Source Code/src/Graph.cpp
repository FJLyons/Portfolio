#include "Graph.h"
Graph::Graph()
{
	_nodes = vector<GraphNode*>();
}


Graph::~Graph() {
	for (int index = 0; index < _nodes.size(); index++)
	{
		if (_nodes[index] != nullptr)
		{
			delete _nodes[index];
			_nodes[index] = nullptr;
		}
	}
	_nodes.clear();
}


void Graph::addNode(pair<GraphNode::EntityData,int> data, helper::Vector2 position)
{
	GraphNode* node = new GraphNode();
	node->setData(data);
	node->setMarked(false);
	node->setPosition(position);
	_nodes.push_back(node);
}

void Graph::removeNode(int index) {

	if (_nodes[index] != nullptr) {

		GraphArc* arc = nullptr;
		for (int node = 0; node < _nodes.size(); node++) {
			if (_nodes[node] != nullptr) {

				arc = _nodes[node]->getArc(_nodes[index]);

			}

			if (arc != nullptr) {
				removeArc(node, index);
			}
		}
		delete _nodes[index];
		_nodes[index] = nullptr;
		_nodes.erase(_nodes.begin() + index);

	}
}

bool Graph::addArc(int from, int to, float weight, bool directed) {
	bool proceed = true;
	// make sure both nodes exist.
	if (_nodes[from] == 0 || _nodes[to] == 0) {
		proceed = false;
	}

	// if an arc already exists we should not proceed
	if (_nodes[from]->getArc(_nodes[to]) != 0) {
		proceed = false;
	}

	if (proceed == true) {
		// add the arc to the "from" node.
		_nodes[from]->addArc(_nodes[to], weight);
		if (directed == false) //add node back the other way if undirected
			_nodes[to]->addArc(_nodes[from], weight);
	}

	return proceed;
}

void Graph::removeArc(int from, int to) {
	// Make sure that the node exists before trying to remove
	// an arc from it.
	bool nodeExists = true;
	if (_nodes[from] == NULL || _nodes[to] == NULL) {
		nodeExists = false;
	}

	if (nodeExists == true) {
		// remove the arc.

		_nodes[from]->removeArc(_nodes[to]);
	}
}


GraphArc* Graph::getArc(int from, int to) {
	GraphArc* pArc = 0;
	// make sure the to and from nodes exist
	if (_nodes[from] != NULL && _nodes[to] != NULL) {
		pArc = _nodes[from]->getArc(_nodes[to]);
	}

	return pArc;
}

int Graph::getNodesSize() {

	return _nodes.size();
}

void Graph::reset() {
	int index;
	for (index = 0; index < _nodes.size(); index++) {
		if (_nodes[index] != NULL) {
			_nodes[index]->reset();
		}
	}
}

void Graph::aStar(GraphNode* pStart, GraphNode* pDest, std::vector<GraphNode *>& path) {

	if (pStart != 0 && pDest != 0) {
		vector<GraphNode*> pq;
		pq.push_back(pStart);
		pStart->setMarked(true);
		pStart->setHCost(0);
		pStart->setGCost(0);

		while (pq.size() != 0 && pq.back() != pDest) {
			list<GraphArc>::const_iterator iter = pq.back()->arcList().begin();
			list<GraphArc>::const_iterator endIter = pq.back()->arcList().end();

			for (; iter != endIter; iter++) {
				GraphNode * child = (*iter).node(); 
				if (child != pq.back()->getPrevious()) {
					GraphArc arc = (*iter);
					int Hc = child->hCost();
					int Gc = pq.back()->gCost() + arc.weight();
					int Fc = Hc + Gc;
					if (Fc < child->fCost() || child->gCost() == -1) {  //is G(n) not set, H(n) should be always set with setHeuristics()		
						child->setHCost(Hc);
						child->setGCost(Gc);
						child->setPrevious(pq.back());
						//make_heap(const_cast<GraphNode**>(&pq.top()), const_cast<GraphNode**>(&pq.top()) + pq.size(), NodeSearchCostComparer());
					}

					if (child->marked() == false) {
						pq.push_back(child);
						child->setMarked(true);
						child->setColour(SDL_Colour{ 0,255,255,255 });
						std::sort(pq.begin(), pq.end(), NodeSearchCostComparer());
						//child->setColour(sf::Color(0, 128, 128, 255));
					}
				}
			}
			pq.pop_back();
			//std::sort(pq.begin(), pq.end(), NodeSearchCostComparer());
		}

		if (pq.size() != 0 && pq.back() == pDest) {
			for (GraphNode* previous = pDest; previous->getPrevious() != 0; previous = previous->getPrevious()) {
				path.push_back(previous);
				previous->setColour(SDL_Colour{ 0,80,255,255 });
			}
			path.push_back(pStart);
			std::reverse(path.begin(), path.end());
		}
		else if (pq.size() == 0)
			cout << "Couldn't find path." << endl;
	}
}

void Graph::setHeuristics(GraphNode* pDest){
if (pDest != 0) {
for (int i = 0; i < _nodes.size(); i++){
helper::Vector2 vectorTo = pDest->getPosition() - _nodes[i]->getPosition();
int Hc = (int)(sqrt((vectorTo.x * vectorTo.x) + (vectorTo.y * vectorTo.y)));
_nodes[i]->setHCost(Hc);
}
}
}



//draw the nodes
void Graph::drawNodes(SDL_Renderer* renderer, Camera* camera) const {
	for (int i = 0; i < _nodes.size(); i++) {
		_nodes[i]->drawNodes(renderer, camera);
	}
}

//draw arcs
void Graph::drawArcs(SDL_Renderer* renderer, Camera* camera) const {
	for (int i = 0; i < _nodes.size(); i++)
	{
		_nodes[i]->drawArcs(renderer, camera);
	}
}

std::vector<GraphNode*> Graph::getNodes() const {
	return _nodes;
}