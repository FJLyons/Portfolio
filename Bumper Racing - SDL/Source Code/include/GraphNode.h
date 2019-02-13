#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <list>
#include "BasicTypes.h"
#include <string>
#include "Camera2D.h" //debugging
using namespace Camera2D;

using namespace std;



class GraphArc;
class GraphNode {

public:
	enum class EntityData
	{
		Null,
		Checkpoint,
		PowerUp,
		Flag
	};

	GraphNode();
	~GraphNode();
	GraphArc* getArc(GraphNode* pNode);

	void  reset();

	void addArc(GraphNode* pNode, float weight);
	void removeArc(GraphNode* pNode);

	void drawArcs(SDL_Renderer* renderer, Camera* camera) const;
	void drawNodes(SDL_Renderer* renderer, Camera* camera) const;

	// Accessor functions
	list<GraphArc> & arcList() ;
	bool marked() const;
	std::pair<EntityData,int> const & data() const;
	void setData(const pair<GraphNode::EntityData, int> &data);
	void setHCost(int hCost);
	void setGCost(int gCost);
	int const & hCost() const;
	int const & gCost() const;
	int const & fCost() const;
	void setMarked(bool mark);
	void setPrevious(GraphNode* previous);
	void setColour(const SDL_Color& color);
	void setPosition(helper::Vector2 newPosition);
	helper::Vector2 getPosition();
	GraphNode* getPrevious();

private:
	list<GraphArc>		_arcList;
	GraphNode*			_prevNode;
	SDL_Color			_color;
	pair<EntityData,int>  _data;

	helper::Vector2		_position;

	int					_hCost;
	int					_gCost;

	bool				_marked;

};
#endif
