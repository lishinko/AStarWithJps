#ifndef ASTAR_H
#define ASTAR_H
#include "node.h"
#include "heap.h"
#include <cmath>
class Map;
typedef float (*DistanceFunc)(const Node* base, const Node* neighbour);
float Euclidean(const Node* base, const Node* neighbour);
float Diagonal(const Node* base, const Node* neighbour);
class AStar
{
public:
    enum FindPathResult{
        fp_NoRoute,
        fp_PushedNeighbours,
        fp_FoundRoute,
        fp_Invalid,
    };

    AStar();
    void setMap(const Map* map){m_map = map;}
    //寻路算法，路径保存在Node的里面。
    FindPathResult findPath(const Node& start, const Node& end);

    void setStardAndEnd(const Node& start, const Node& end){
        m_start = &start;
        m_end = &end;
    }
    void setDistanceFunc(DistanceFunc func){
        m_distanceFunc = func;
    }
    std::vector<const Node*> getPath();
protected:
    FindPathResult findpathImpl();
    void expandSuccessors(const Node* node);
    bool inClosed(const Node* node){return std::find(m_closed.begin(), m_closed.end(), node) != m_closed.end();}

    const Map* m_map;
    Heap m_open;
    std::vector<const Node*> m_closed;
    DistanceFunc m_distanceFunc;

    const Node* m_start;
    const Node* m_end;

};

#endif // ASTAR_H
