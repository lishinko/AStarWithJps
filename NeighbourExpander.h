#pragma once
#include "noderect.h"
class AStar;
class Map;
class NeighbourExpander{
public:
	NeighbourExpander(AStar* holder):m_holder(holder){}
	virtual void expandSuccessors(const /*Node**/NodeIdx node);
protected:
	AStar* m_holder;
};