#pragma once
class AStar;
class Map;
class Node;
class NodeSavingF;
class NeighbourExpander{
public:
	NeighbourExpander(AStar* holder):m_holder(holder){}
	virtual void expandSuccessors(const NodeSavingF* node);
protected:
	AStar* m_holder;
};