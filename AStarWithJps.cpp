#include "AStarWithJps.h"
#include <cmath>
#include <algorithm>
#include <cassert>
//jps算法，主要减少了A*的open表的长度。参见：
//http://users.cecs.anu.edu.au/~dharabor/data/papers/harabor-grastien-aaai11.pdf
//原作者官网：http://harablog.wordpress.com/2011/09/07/jump-point-search/
//中文版解释：http://plusplus7.com/lemonbag/jpsstudy
//英文版解释，推荐：http://zerowidth.com/2013/05/05/jump-point-search-explained.html
//Lua源代码参考：https://github.com/Yonaba/Jumper
void AStarWithJps::expandSuccessors(const NodeSavingF* node){
	findNeighbours(node);
	//查找跳点
	for(std::vector<NodeSavingF*>::reverse_iterator it = ret.rbegin(); it != ret.rend(); ++it){
		bool skip = false;
		NodeSavingF* neighbour = *it;
		NodeSavingF* jumpNode = jump(neighbour, node, m_end);
		if(jumpNode){
			insertNodeToOpen(jumpNode, node);
		}
	}
}
void AStarWithJps::findNeighbours(const NodeSavingF* node){
	if(node->getParent() == NULL){//没有父节点，一般来说，只可能是因为，本node为start节点
		AStar::expandSuccessors(node);//那么，使用父类的expandSuccessors就可以了
	}
	const int x = node->getX();
	const int y = node->getY();//缓存自己的坐标。

	const int ddx = x - node->getParent()->getX();
	assert(std::abs(ddx) <= 1);
	const int dx = ddx;//; / std::max(std::abs(ddx), 1);//我们的程序中，邻居总是跟它的父节点相差『1，1』。
	const int ddy = y - node->getParent()->getY();
	assert(std::abs(ddy) <= 1);
	const int dy = ddy;

	assert(dx != 0 || dy != 0);//邻居

	if(dx != 0 && dy != 0){//斜线方向
		//自然邻居
		bool walkY = addNeighbours_Diagonal_Natural(x, y+dy);
		bool walkX = addNeighbours_Diagonal_Natural(x+dx, y);
		if(walkX || walkY){
			ret.push_back(m_map->getNode(x+dx, y+dy));//原算法这里没有判断有没有阻挡。不知道为什么。
			//addNeighbours_Diagonal_DiagonalNatural(x+dx, y+dy);这是我认为的，需要判断阻挡的操作。先注释掉。以后看看有没有用
		}
		//强制邻居
		addNeighbours_Diagonal_Force(x-dx, y, x-dx, y+dy, walkY);
		addNeighbours_Diagonal_Force(x, y-dy, x+dx, y-dy, walkY);
	}
	else{
		if(dx != 0){//x方向
			//自然邻居
			addNeighbours_Directional_Natural(x + dx, y);
			//看看有没有强制邻居。
			addNeighbours_Directional_Force(x, y+1, x+dx, y+1);
			addNeighbours_Directional_Force(x, y-1, x+dx, y-1);
		}
		else{//y方向
			assert(dy != 0);
			//自然邻居
			addNeighbours_Directional_Natural(x, y + dy);
			//看看有没有强制邻居。
			addNeighbours_Directional_Force(x+1, y, x+1, y+dy);
			addNeighbours_Directional_Force(x-1, y, x-1, y+dy);
		}
	}
}
NodeSavingF* AStarWithJps::jump(NodeSavingF* node, const NodeSavingF* parent, const NodeSavingF* endNode) const
{
	if( node == NULL || node->isBlock())//如果本节点不存在或者是阻挡点，就不能跳
		return NULL;

	const int x = node->getX();
	const int y = node->getY();//缓存自己的坐标。

	const int dx = x - node->getParent()->getX();
	const int dy = y - node->getParent()->getY();
	
	/*if(!m_map->isWalkableAt(x,y){//再开头已经判断过了
		return NULL;
	}*/
	if(node == endNode){//等于终止节点，停止搜索
		return node;
	}
	if(dx != 0 && dy != 0){//斜向
		//本节点是跳点，如果左右方向邻居是强制邻居（至少一个是就可以了）
		if(m_map->isWalkableAt(x-dx, y+dy) && !m_map->isWalkableAt(x-dx, y)
		|| m_map->isWalkableAt(x+dx, y-dy) && !m_map->isWalkableAt(x, y-dy)
		){
			return node;
		}
	}
	else{
		if(dx != 0){//x方向：
			//上下2个邻居有一个是强制邻居，则此节点为跳点
			if(m_map->isWalkableAt(x+dx, y+1) && !m_map->isWalkableAt(x, y+1)
			|| m_map->isWalkableAt(x+dx, y-1) && !m_map->isWalkableAt(x, y-1)
				){
				return node;
			}
		}
		else{//y方向：
			//左右2个邻居有一个是强制邻居，则此节点为跳点
			if(m_map->isWalkableAt(x+1, y+dy) && !m_map->isWalkableAt(x+1, y)
			|| m_map->isWalkableAt(x-1, y+dy) && !m_map->isWalkableAt(x-1, y)
				){
				return node;
			}
		}
	}
	//斜向的话还需要递归检查直向节点是不是跳点
	if(dx != 0 && dy != 0){//斜向	
		if(jump(m_map->getNode(x+dx, y), node, endNode) != NULL){
			return node;//注意，返回的不是递归搜索到的点，而是自己
		}
		if(jump(m_map->getNode(x, y+dy), node, endNode) != NULL){
			return node;//注意，返回的不是递归搜索到的点，而是自己
		}
	}
	//递归斜向搜索
	if(m_map->isWalkableAt(x+dx, y) || m_map->isWalkableAt(x, y+dy)){
		return jump(m_map->getNode(x+dx, y+dy), node, endNode);
	}
	//什么也没有找到，返回错误。
	return NULL;
}