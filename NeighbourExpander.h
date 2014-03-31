#pragma once
class AStar;
class Map;
class Node;
class NeighbourExpander{
public:
	NeighbourExpander(AStar* holder):m_holder(holder){}
	virtual void expandSuccessors(const Node *node);
protected:
	AStar* m_holder;
};