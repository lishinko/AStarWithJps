#ifndef ASTAR_H
#define ASTAR_H
#include "node.h"
#include "heap.h"
#include <cmath>
class Map;
class NeighbourExpander;
typedef dis_type (*DistanceFunc)(const Node* base, const Node* neighbour);
dis_type Euclidean(const Node* base, const Node* neighbour);
dis_type Diagonal(const Node* base, const Node* neighbour);
class AStar
{
public:
    enum FindPathResult{
        fp_NoRoute,
        fp_PushedNeighbours,
        fp_FoundRoute,
		/*fp_StartIsBlock,
		fp_EndIsBlock,*/
        fp_Invalid,
    };

    AStar();
    void setMap(const Map* map){m_map = map;}
	void setNeighbourExpander(NeighbourExpander* expander){m_expander = expander;}
    //寻路算法，路径保存在Node的里面。
    FindPathResult findPath(const Node& start, const Node& end);

    void setStardAndEnd(const Node& start, const Node& end);
    void setDistanceFunc(DistanceFunc func){
        m_distanceFunc = func;
    }
    std::vector<const Node*> getPath();

	const Map* getMap(){return m_map;}
	void pushToOpen(const Node* node){m_open.push(node);}
	bool inOpen(const Node* node) const{return m_open.has(node);}
	dis_type getDis(const Node* node, const Node* parent) const{return m_distanceFunc(node, parent);}

	void insertNodeToOpen(Node* node, const Node* parent);
	const Node* getEndNode(){return m_end;}
protected:
    FindPathResult findpathImpl();
    void expandSuccessors(const Node* node);
    bool inClosed(const Node* node){return std::find(m_closed.begin(), m_closed.end(), node) != m_closed.end();}
	


    const Map* m_map;
    Heap m_open;
    std::vector<const Node*> m_closed;//closed表就不需要排序了
    DistanceFunc m_distanceFunc;
	NeighbourExpander* m_expander;

    const Node* m_start;
    const Node* m_end;
};

#endif // ASTAR_H
