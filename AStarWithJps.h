#pragma once

#include "astar.h"
#include "map.h"
class AStarWithJps : public AStar
{
protected:
	void expandSuccessors(const Node* node);//这里没有使用虚函数，需要测试下能不能真的执行到。
	void findNeighbours(const Node* node);
	Node* jump(Node* node, const Node* parent, const Node* endNode) const;
protected:
	void addNeighbours_Directional_Natural(const int blockx, const int blocky){
		Node* neighbour = m_map->getNode(blockx, blocky);
		if(!neighbour->isBlock()){
			ret.push_back(neighbour);//本方向自然邻居。
		}
	}
	void addNeighbours_Directional_Force(const int blockx, const int blocky, const int neighbourx, const int neighboury){
		if(m_map->isWalkableAt(blockx, blocky)){
			Node* neighbour = m_map->getNode(neighbourx, neighboury);
			ret.push_back(neighbour);//本方向强制邻居。
		}
	}
	bool addNeighbours_Diagonal_Natural(const int blockx, const int blocky){
		Node* neighbour = m_map->getNode(blockx, blocky);
		if(!neighbour->isBlock()){
			ret.push_back(neighbour);//x,y方向自然邻居
			return true;
		}
		return false;
	}
	void addNeighbours_Diagonal_DiagonalNatural(const int blockx, const int blocky){
		addNeighbours_Directional_Natural(blockx, blocky);//本方向（斜向）自然邻居。
	}
	void addNeighbours_Diagonal_Force(const int blockx, const int blocky, const int neighbourx, const int neighboury, bool walkable){
		if(walkable){
			if(m_map->isWalkableAt(blockx, blocky)){
				Node* neighbour = m_map->getNode(neighbourx, neighboury);
				ret.push_back(neighbour);//强制邻居。
			}
		}
	}
	std::vector<Node*> ret;

};