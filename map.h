#ifndef MAP_H
#define MAP_H
class Node;
#include "node.h"
#include "noderect.h"
#include <cstddef>
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
    NeighbourIterator(const NodeRect& rect, const Node* startNode)
        :m_coordIdx(0),m_rect(rect),m_startNode(startNode) {
        operator ++();//保证刚开始调用的是正确的点
    }
    const NeighbourIterator& operator++(){
        ++m_coordIdx;
        const int coordsSize = sizeof(coords) / sizeof(Point);
        Node* ret = getNeighbour(coordsSize, coords);
        while(ret == NULL && m_coordIdx < coordsSize){
            ++m_coordIdx;
            ret = getNeighbour(coordsSize, coords);
        }
        return *this;
    }
    Node* operator*() const{

        const int coordsSize = sizeof(coords) / sizeof(Point);
        Node* ret = getNeighbour(coordsSize, coords);
        return ret;
    }
protected:
    int m_coordIdx;
    const NodeRect& m_rect;
    const Node* m_startNode;
    Node* getNeighbour(const int num,const Point* coords) const{
        if(m_coordIdx >= num)
        {
            return NULL;
        }
        const int x = m_startNode->getX() + coords[m_coordIdx].x;
        const int y = m_startNode->getY() + coords[m_coordIdx].y;
        return m_rect.getNode(x, y);
    }
};
class OthogonalNeighbourIterator : public NeighbourIterator
{
public:
    OthogonalNeighbourIterator(const NodeRect& rect, const Node* startNode)
        :NeighbourIterator(rect, startNode){}
    Node* operator*() const{
        static const Point coords[] = {
            {-1,0},
            {0,-1},
            //{0,0},自己，不需要迭代了
            {0,1},
            {1,0},
        };
        return getNeighbour(sizeof(coords) / sizeof(Point), coords);
    }
};
class DiagonalNeighbourIterator : public NeighbourIterator
{
public:
    DiagonalNeighbourIterator(const NodeRect& rect, const Node* startNode)
        :NeighbourIterator(rect, startNode){}
    Node* operator*() const{
        static const Point coords[] = {
            {-1,-1},
            {-1,1},
            //{0,0},自己，不需要迭代了
            {1,-1},
            {1,1},
        };
        return getNeighbour(sizeof(coords) / sizeof(Point), coords);
    }
};
class Map
{
public:
    Map();
    void setMap(const char* mapString);
    void getNeighbours();
    Node* getNode(int x, int y) const{return m_nodes.getNode(x, y);}
    NeighbourIterator begin(const Node *node) const{
        return NeighbourIterator(m_nodes, node);
    }
    DiagonalNeighbourIterator beginDiagonal(const Node *node) const{
        return DiagonalNeighbourIterator(m_nodes, node);
    }
    OthogonalNeighbourIterator beginOthogonal(const Node *node) const{
        return OthogonalNeighbourIterator(m_nodes, node);
    }
    void dumpMap();

protected:
    NodeRect m_nodes;
};

#endif // MAP_H
