#ifndef GRAPHARC_H
#define GRAPHARC_H

#include "GraphNode.h"
#include <iostream>
#include <list>
#include "SDL.h" //for debuging

#include "Camera2D.h"
using namespace Camera2D;
using namespace helper;

class GraphArc {
private:

	GraphNode* m_pNode;

    float m_weight;

	SDL_Point points[2];
	SDL_Color _color;
public:

	GraphArc() : m_pNode(nullptr){
		_color = SDL_Color{ 255,255,255,255 };
	}

	virtual void draw(SDL_Renderer* renderer, Camera* camera) const {
		SDL_SetRenderDrawColor(renderer, _color.r,_color.g,_color.b,_color.a);
		SDL_Point drawPoint[2];
		for (int i = 0; i < 2; i++)
		{
			Point point = { (float)points[i].x, (float)points[i].y };
			point = camera->worldToScreen(point);
			drawPoint[i].x = point.x;
			drawPoint[i].y= point.y;
		}
		SDL_RenderDrawLines(renderer, drawPoint, 2);
	}

	void setLine(helper::Vector2 start, helper::Vector2 end){
		points[0] = { (int)start.x, (int)start.y };
		points[1] = { (int)end.x, (int)end.y };
	}
	
    // Accessor functions
	
    GraphNode* node() const{
        return m_pNode;
    }
                              
    float weight() const {
        return m_weight;
    }
    
    // Manipulator functions
	
    void setNode(GraphNode* pNode) {
		m_pNode = pNode;
    }
    
    void setWeight(float weight) {
       m_weight = weight;
    }
    
};

#endif
