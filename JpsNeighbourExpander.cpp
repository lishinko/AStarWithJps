#include "JpsNeighbourExpander.h"
#include <vector>
#include <cassert>
#include "noderect.h"
void JpsNeighbourExpander::expandSuccessors(const /*Node**/NodeIdx node){
	ret.clear();
	findNeighbours(node);
	//查找跳点
	for(std::vector</*Node**/NodeIdx>::reverse_iterator it = ret.rbegin(); it != ret.rend(); ++it){
		bool skip = false;
		/*Node**/NodeIdx neighbour = *it;
		/*Node**/NodeIdx jumpNode = jump(neighbour, node, m_holder->getEndNode());
		if(jumpNode){
			m_holder->insertNodeToOpen(jumpNode, node);
		}
	}
}

void JpsNeighbourExpander::findNeighbours(const /*Node**/NodeIdx node){
	if(/*node->getParent() == NULL*/m_holder->getMap()->getRect().getParts().getParent(node) == -1){//没有父节点，一般来说，只可能是因为，本node为start节点
		NeighbourExpander::expandSuccessors(node);//那么，使用父类的expandSuccessors就可以了
		return;
	}
	//const int x = node->getX();
	//const int y = node->getY();//缓存自己的坐标。

	//const int ddx = x - node->getParent()->getX();
	//const int dx = ddx / std::max(std::abs(ddx), 1);//我们的程序中，邻居总是跟它的父节点相差『1，1』。
	//const int ddy = y - node->getParent()->getY();
	//const int dy = ddy / std::max(std::abs(ddy), 1);

	const coord_type x = m_holder->getMap()->getRect().getParts().getX(node);
	const coord_type y = m_holder->getMap()->getRect().getParts().getY(node);

	const NodeIdx parent = m_holder->getMap()->getRect().getParts().getParent(node);
	const coord_type ddx = x - m_holder->getMap()->getRect().getParts().getX(parent);
	const coord_type dx = ddx / std::max(std::abs(ddx), 1);//我们的程序中，邻居总是跟它的父节点相差『1，1』。
	const coord_type ddy = y - m_holder->getMap()->getRect().getParts().getY(parent);
	const coord_type dy = ddy / std::max(std::abs(ddy), 1);

	assert(dx != 0 || dy != 0);//邻居

	if(dx != 0 && dy != 0){//斜线方向
		//自然邻居
		bool walkY = addNeighbours_Diagonal_Natural(x, y+dy);
		bool walkX = addNeighbours_Diagonal_Natural(x+dx, y);
		//if(walkX || walkY){
		//	ret.push_back(m_holder->getMap()->getNode(x+dx, y+dy));//原算法这里没有判断有没有阻挡。不知道为什么。
		//	//addNeighbours_Diagonal_DiagonalNatural(x+dx, y+dy);这是我认为的，需要判断阻挡的操作。先注释掉。以后看看有没有用
		//}
		//ret.push_back(m_holder->getMap()->getNode(x+dx, y+dy));//原算法这里没有判断有没有阻挡。不知道为什么。
		addNeighbours_Diagonal_DiagonalNatural(x+dx, y+dy);
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
/*Node**/NodeIdx JpsNeighbourExpander::jump(/*Node**/NodeIdx node, const /*Node**/NodeIdx parent, const /*Node**/NodeIdx endNode) const
{
	if( /*node == NULL || node->isBlock()*/node == -1 || m_holder->getMap()->isBlock(node))//如果本节点不存在或者是阻挡点，就不能跳
		return -1;

	//const int x = node->getX();
	//const int y = node->getY();//缓存自己的坐标。

	//const int dx = x - parent->getX();
	//const int dy = y - parent->getY();

	const coord_type x = m_holder->getMap()->getRect().getParts().getX(node);
	const coord_type y = m_holder->getMap()->getRect().getParts().getY(node);

	const coord_type dx = x - m_holder->getMap()->getRect().getParts().getX(parent);
	const coord_type dy = y - m_holder->getMap()->getRect().getParts().getY(parent);
	
	/*if(!m_holder->getMap()->isWalkableAt(x,y){//再开头已经判断过了
		return NULL;
	}*/
	if(node == endNode){//等于终止节点，停止搜索
		return node;
	}
	if(dx != 0 && dy != 0){//斜向
		//本节点是跳点，如果左右方向邻居是强制邻居（至少一个是就可以了）
		if(m_holder->getMap()->isWalkableAt(x-dx, y+dy) && !m_holder->getMap()->isWalkableAt(x-dx, y)
		|| m_holder->getMap()->isWalkableAt(x+dx, y-dy) && !m_holder->getMap()->isWalkableAt(x, y-dy)
		){
			return node;
		}
	}
	else{
		if(dx != 0){//x方向：
			//上下2个邻居有一个是强制邻居，则此节点为跳点
			if(m_holder->getMap()->isWalkableAt(x+dx, y+1) && !m_holder->getMap()->isWalkableAt(x, y+1)
			|| m_holder->getMap()->isWalkableAt(x+dx, y-1) && !m_holder->getMap()->isWalkableAt(x, y-1)
				){
				return node;
			}
		}
		else{//y方向：
			//左右2个邻居有一个是强制邻居，则此节点为跳点
			if(m_holder->getMap()->isWalkableAt(x+1, y+dy) && !m_holder->getMap()->isWalkableAt(x+1, y)
			|| m_holder->getMap()->isWalkableAt(x-1, y+dy) && !m_holder->getMap()->isWalkableAt(x-1, y)
				){
				return node;
			}
		}
	}
	//斜向的话还需要递归检查直向节点是不是跳点
	if(dx != 0 && dy != 0){//斜向	
		if(jump(m_holder->getMap()->getNode(x+dx, y), node, endNode) != NULL){
			return node;//注意，返回的不是递归搜索到的点，而是自己
		}
		if(jump(m_holder->getMap()->getNode(x, y+dy), node, endNode) != NULL){
			return node;//注意，返回的不是递归搜索到的点，而是自己
		}
	}
	//递归斜向搜索
	if(m_holder->getMap()->isWalkableAt(x+dx, y) || m_holder->getMap()->isWalkableAt(x, y+dy)){
		return jump(m_holder->getMap()->getNode(x+dx, y+dy), node, endNode);
	}
	//什么也没有找到，返回错误。
	return NULL;
}