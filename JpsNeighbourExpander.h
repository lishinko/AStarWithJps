#pragma once
#include "NeighbourExpander.h"
#include "astar.h"
#include "map.h"
class Node;
class JpsNeighbourExpander : public NeighbourExpander{
public:
	JpsNeighbourExpander(AStar* holder):NeighbourExpander(holder){}
	void expandSuccessors(const Node *node);

protected:
	void findNeighbours(const Node* node);
	Node* jump(Node* node, const Node* parent, const Node* endNode) const;
protected:
	void addNeighbours_Directional_Natural(const int blockx, const int blocky, const Node* parent){
		Node* neighbour = m_holder->getMap()->getNode(blockx, blocky);
		if(neighbour && !neighbour->isBlock()){
			pushNode(neighbour, parent);//本方向自然邻居。
		}
	}
	void addNeighbours_Directional_Force(const int blockx, const int blocky, const int neighbourx, const int neighboury, const Node* parent){
		if(!m_holder->getMap()->isWalkableAt(blockx, blocky)){
			Node* neighbour = m_holder->getMap()->getNode(neighbourx, neighboury);
			if(neighbour && !neighbour->isBlock()){
				pushNode(neighbour, parent);//本方向强制邻居。
			}
		}
	}
	bool addNeighbours_Diagonal_Natural(const int blockx, const int blocky, const Node* parent){
		Node* neighbour = m_holder->getMap()->getNode(blockx, blocky);
		if(neighbour && !neighbour->isBlock()){
			pushNode(neighbour, parent);//x,y方向自然邻居
			return true;
		}
		return false;
	}
	void addNeighbours_Diagonal_DiagonalNatural(const int blockx, const int blocky, const Node* parent){
		//addNeighbours_Directional_Natural(blockx, blocky);//本方向（斜向）自然邻居。
		Node* neighbour = m_holder->getMap()->getNode(blockx, blocky);
		if(neighbour && !neighbour->isBlock()){
			pushNode(neighbour, parent);//x,y方向自然邻居
			//return true;
		}
	}
	void addNeighbours_Diagonal_Force(const int blockx, const int blocky, const int neighbourx, const int neighboury, bool walkable, const Node* parent){
		if(walkable){
			if(!m_holder->getMap()->isWalkableAt(blockx, blocky)){
				Node* neighbour = m_holder->getMap()->getNode(neighbourx, neighboury);
				if(neighbour && !neighbour->isBlock()){
					pushNode(neighbour, parent);//强制邻居。
				}
			}
		}
	}
	void pushNode(Node* node, const Node* parent){
		node->setParent(parent);
		ret.push_back(node);
	}
	std::vector<Node*> ret;
};