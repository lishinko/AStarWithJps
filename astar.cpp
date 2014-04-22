#include "astar.h"
#include "map.h"
#include "node.h"
#include <stack>
#include <iostream>
#include "NeighbourExpander.h"
#include <cassert>
using namespace std;
AStar::AStar()
    :m_distanceFunc(NULL)
    ,m_map(NULL)
	,m_needFullParentInfo(false)
{
}

AStar::FindPathResult AStar::findPath(const Node &start, const Node &end)
{
    if(&start == &end){
        return fp_FoundRoute;//开始与结束相同，直接返回就可以了
    }

    setStardAndEnd(start, end);

    m_open.push(&start);
    AStar::FindPathResult result = fp_Invalid;
    while (true) {
        result = findpathImpl();
        if(result == fp_NoRoute)
            return result;
        if(result == fp_FoundRoute)
            return result;
    }
    return fp_Invalid;
}

std::vector<const Node *> AStar::getPath()
{
    std::vector<const Node*> path;
//    std::stack<const Node*> s;
//    for(const Node* pathNode = m_end; pathNode != NULL; pathNode = pathNode->getParent()){
//        s.push(pathNode);
//    }
//    while (!s.empty()) {
//        const Node* node = s.top();
//        path.push_back(node);
//        s.pop();
//    }
    for(const Node* pathNode = m_end; pathNode != NULL; pathNode = pathNode->getParent()){
        path.push_back(pathNode);
    }
    cout << path.size() << endl;
    return path;
}



AStar::FindPathResult AStar::findpathImpl()
{
    const Node* node = m_open.pop();
    if(!node)
    {//走投无路，寻路失败了
        return fp_NoRoute;
    }
    if(node == m_end){
        return fp_FoundRoute;//找到路径了
    }

    //将自己放入closed表
    m_closed.push_back(node);
    expandSuccessors(node);

    return fp_PushedNeighbours;
}

void AStar::expandSuccessors(const Node *node)
{
	m_expander->expandSuccessors(node);
}
void AStar::insertNodeToOpen(Node *node, const Node* parent){
	if(node->isBlock() || inClosed(node))
    {//如果是阻挡点，或者已经搜索过的点，pass
        return;
    }
    const dis_type newG = parent->getG() + m_distanceFunc(parent,node);
    if(m_open.has(node))
    {//如果已经出现在open表，检查g值，如果较小，就更新
        if(node->getG() > newG)
        {
            node->setG(newG);
			setParent(node, parent);
            m_open.sort();
        }
    }
    else//不是阻挡点，不在closed表，也不在open表，
    {//创建新open表节点。
        node->setH(m_distanceFunc(node, m_end));
        node->setG(newG);
		setParent(node, parent);
        m_open.push(node);
    }
}
void AStar::setStardAndEnd(const Node& start, const Node& end)
{
	if(m_map->getNode(start.getX(), start.getY()) == NULL)
	{
		//start节点不存在
		return;
	}
	if(m_map->getNode(end.getX(), end.getY()) == NULL)
	{
		//end节点不存在
		return;
	}
    m_start = &start;
    m_end = &end;
	if(m_start->isBlock())
	{
		return;
	}
	if(m_end->isBlock())
	{
		return;
	}
}
void AStar::setParent(Node* node, const Node* parent){
	if(!m_needFullParentInfo){
		node->setParent(parent);
		return;
	}

	//如果我们允许直线寻路(目标可以走直线而不管格子是不是相邻),那么下面的这些可以排除,从而提升一些性能.
	//实际上,在一个水平的地面走的话,可以这么做(动画系统支持走无限长直线即可).
	//如果地面有起伏,那么就必须一格一格的走(用来计算高度)

	//如果一些点被跳过了,那么需要设置他们的父节点,但是不需要加入open表
	const coord_type dx = parent->getX() - node->getX();
	const coord_type dy = parent->getY() - node->getY();
	assert(dx == 0 || dy == 0 || std::abs(dx) == std::abs(dy));//jps算法跳过的点总是直线,横竖斜都可以
	if(dx == 0 && dy == 0){
		return;
	}
	//如果是横线,那么stepy == 0,从而不会改变y方向,x方向同理.xy同时改变的时候,当然也是没有问题的
	const coord_type stepx = dx == 0 ? 0 : std::abs(dx) / dx;//获得x发现,y方向的符号,结果有+1,-1,0,一共3种
	const coord_type stepy = dy == 0 ? 0 : std::abs(dy) / dy;

	Node* pCurNode = node;
	for(int x = stepx, y = stepy; x != dx + stepx || y != dy+stepy; x+=stepx,y+=stepy){
		Node* pDirectParent = m_map->getNode(node->getX() + x, node->getY() + y);
		assert(pDirectParent);
		if(pDirectParent){
			pCurNode->setParent(pDirectParent);
			pCurNode = pDirectParent;//接下来要设置pDirectParent的parent了,一直到parent节点本身为止
		}
	}
	assert(pCurNode == parent);
}
dis_type Euclidean(const Node *base, const Node *neighbour)
{//寻路的路径计算方式：直线法
    const coord_type absoluteX = abs(base->getX() - neighbour->getX());
    const coord_type absoluteY = abs(base->getY() - neighbour->getY());
    const dis_type square = absoluteX*absoluteX + absoluteY*absoluteY;
    return std::sqrt(static_cast<float>(square));
}


dis_type Diagonal(const Node *base, const Node *neighbour)
{//寻路的路径计算方式：横，竖线+斜线法。实际上，游戏内就是使用这个来计算的。
    const coord_type absoluteX = abs(base->getX() - neighbour->getX());
    const coord_type absoluteY = abs(base->getY() - neighbour->getY());
    const coord_type longD = std::max(absoluteX, absoluteY);
    const coord_type shortD = std::min(absoluteX, absoluteY);
    //具体算法：短*斜向 + （长-短）*直向
	//这里使用了整数来代替1.414这样的浮点数。可以提升性能578->438,性能提升还是比较明显的。
    return (dis_type)shortD*14 + (dis_type)(longD-shortD)*10;
    //征途2使用10，14（或15）来计算路径，从而避免了浮点数运算的问题。最后考虑这个。
}