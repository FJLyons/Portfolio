#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <queue>
#include "BasicTypes.h"
#include "GraphNode.h"
#include "GraphArc.h"

using namespace helper;
using namespace std;

class Graph {
public:           
								Graph() ;
								~Graph();

	void						addNode(pair<GraphNode::EntityData, int> data, helper::Vector2 position);
    void						removeNode( int index );
    bool						addArc( int from, int to, float weight, bool directed = true );
    void						removeArc( int from, int to );
	void						reset();
	
	//Pathfinding
	void						aStar(GraphNode* pStart, GraphNode* pDest, std::vector<GraphNode *>& path);
	void						setHeuristics(GraphNode* pDest);
	void						drawNodes(SDL_Renderer* renderer, Camera* camera) const;
	void						drawArcs(SDL_Renderer* renderer, Camera* camera) const;

	// Accessors
	std::vector<GraphNode*>		getNodes() const;
	GraphArc*					getArc(int from, int to);
	int							getNodesSize();

private:
	std::vector<GraphNode*>		_nodes;

	class NodeSearchCostComparer {
	public:
		bool operator()(const GraphNode * n1, const GraphNode * n2) {
			int f1 = n1->hCost() + n1->gCost();
			int f2 = n2->hCost() + n2->gCost();
			return f1 > f2;
		}
	};

};









#endif
