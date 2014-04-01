#ifndef MAP_H
#define MAP_H
class Node;
#include "node.h"
#include "noderect.h"
#include <cstddef>
#include <iosfwd>
#include <vector>
struct Point
{
    int x;
    int y;
};
static const Point coords[] = {
    {-1,-1},
    {-1,0},
    {-1,1},
    {0,-1},
    //{0,0},自己，不需要迭代了
    {0,1},
    {1,-1},
    {1,0},
    {1,1},
};
class NeighbourIterator
{
public:
    NeighbourIterator(const NodeRect& rect, const /*Node**/NodeIdx startNode)
        :m_coordIdx(0),m_rect(rect),m_startNode(startNode) {
        operator ++();//保证刚开始调用的是正确的点
    }
    const NeighbourIterator& operator++(){//operator++必须保证找到的就是一个可以使用的邻居。
        ++m_coordIdx;
        const int coordsSize = sizeof(coords) / sizeof(Point);
        /*Node**/NodeIdx ret = getNeighbour(coordsSize, coords);
        while(ret == -1 && m_coordIdx < coordsSize){//m_coordIdx < coordsSize这句话，应该可以提前到外面来。
            ++m_coordIdx;
            ret = getNeighbour(coordsSize, coords);
        }
        return *this;
    }
    /*Node**/NodeIdx operator*() const{

        const int coordsSize = sizeof(coords) / sizeof(Point);
        /*Node**/NodeIdx ret = getNeighbour(coordsSize, coords);
        return ret;
    }
protected:
    int m_coordIdx;
    const NodeRect& m_rect;
    const /*Node**/NodeIdx m_startNode;
    /*Node**/NodeIdx getNeighbour(const int num,const Point* coords) const{
        if(m_coordIdx >= num)
        {
            return -1;
        }
		
        /*const int x = m_startNode->getX() + coords[m_coordIdx].x;
        const int y = m_startNode->getY() + coords[m_coordIdx].y;*/

		const coord_type x = m_rect.getParts().getX(m_startNode) + coords[m_coordIdx].x;
        const coord_type y = m_rect.getParts().getY(m_startNode) + coords[m_coordIdx].y;
        return m_rect.getNode(x, y);
    }
};
class JpsNeighbourIterator : public NeighbourIterator
{
	JpsNeighbourIterator(const NodeRect& rect, const /*Node**/NodeIdx startNode)
		: NeighbourIterator(rect, startNode){}
	const NeighbourIterator& operator++(){
		++m_coordIdx;
		const int coordsSize = sizeof(coords) / sizeof(Point);
		/*Node**/NodeIdx ret = getNeighbour(coordsSize, coords);
		while(ret == -1 && m_coordIdx < coordsSize){
			++m_coordIdx;
			ret = getNeighbour(coordsSize, coords);
		}
		return *this;
	}
};
//class OthogonalNeighbourIterator : public NeighbourIterator
//{
//public:
//    OthogonalNeighbourIterator(const NodeRect& rect, const Node* startNode)
//        :NeighbourIterator(rect, startNode){}
//    Node* operator*() const{
//        static const Point coords[] = {
//            {-1,0},
//            {0,-1},
//            //{0,0},自己，不需要迭代了
//            {0,1},
//            {1,0},
//        };
//        return getNeighbour(sizeof(coords) / sizeof(Point), coords);
//    }
//};
//class DiagonalNeighbourIterator : public NeighbourIterator
//{
//public:
//    DiagonalNeighbourIterator(const NodeRect& rect, const Node* startNode)
//        :NeighbourIterator(rect, startNode){}
//    Node* operator*() const{
//        static const Point coords[] = {
//            {-1,-1},
//            {-1,1},
//            //{0,0},自己，不需要迭代了
//            {1,-1},
//            {1,1},
//        };
//        return getNeighbour(sizeof(coords) / sizeof(Point), coords);
//    }
//};
class Map
{
public:
    Map();
    void setMap(const char* mapString);
    void getNeighbours();
    /*Node**/NodeIdx getNode(coord_type x, coord_type y) const{return m_nodes.getNode(x, y);}
    NeighbourIterator begin(const /*Node**/NodeIdx node) const{
        return NeighbourIterator(m_nodes, node);
    }
    /*DiagonalNeighbourIterator beginDiagonal(const Node *node) const{
        return DiagonalNeighbourIterator(m_nodes, node);
    }
    OthogonalNeighbourIterator beginOthogonal(const Node *node) const{
        return OthogonalNeighbourIterator(m_nodes, node);
    }*/
	void dumpMap(std::ofstream& os, const std::vector<const /*Node**/NodeIdx>& path);

	bool isWalkableAt(const int x, const int y) const{
		const /*Node**/NodeIdx node = m_nodes.getNode(x,y);
		//return node && !node->isBlock();
		return node != -1 && !isBlock(node);
	}
	bool isBlock(const NodeIdx idx){
		return m_nodes.getParts().isBlock(idx);
	}
	const bool isBlock(const NodeIdx idx) const{
		return m_nodes.getParts().isBlock(idx);
	}
	const dis_type getF(const NodeIdx idx) const{
		return m_nodes.getParts().getF(idx);
	}
	const NodeRect& getRect() const{return m_nodes;}
	NodeRect& getRect(){return m_nodes;}

protected:
    NodeRect m_nodes;
};

#endif // MAP_H

