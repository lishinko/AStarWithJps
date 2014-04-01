#ifndef ASTAR_H
#define ASTAR_H
#include "node.h"
#include "heap.h"
#include <cmath>
class Map;
class NeighbourExpander;
typedef float (*DistanceFunc)(const NodeSavingF* base, const NodeSavingF* neighbour);
float Euclidean(const NodeSavingF* base, const NodeSavingF* neighbour);
float Diagonal(const NodeSavingF* base, const NodeSavingF* neighbour);
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
    FindPathResult findPath(const NodeSavingF& start, const NodeSavingF& end);

    void setStardAndEnd(const NodeSavingF& start, const NodeSavingF& end);
    void setDistanceFunc(DistanceFunc func){
        m_distanceFunc = func;
    }
    std::vector<const NodeSavingF*> getPath();

	const Map* getMap(){return m_map;}
	void pushToOpen(const NodeSavingF* node){m_open.push(node);}
	bool inOpen(const NodeSavingF* node) const{return m_open.has(node);}
	float getDis(const NodeSavingF* node, const NodeSavingF* parent) const{return m_distanceFunc(node, parent);}

	void insertNodeToOpen(NodeSavingF* node, const NodeSavingF* parent);
	const NodeSavingF* getEndNode(){return m_end;}
protected:
    FindPathResult findpathImpl();
    void expandSuccessors(const NodeSavingF* node);
    bool inClosed(const NodeSavingF* node){return std::find(m_closed.begin(), m_closed.end(), node) != m_closed.end();}
	


    const Map* m_map;
    Heap m_open;
    std::vector<const NodeSavingF*> m_closed;//closed表就不需要排序了
    DistanceFunc m_distanceFunc;
	NeighbourExpander* m_expander;

    const NodeSavingF* m_start;
    const NodeSavingF* m_end;
};

#endif // ASTAR_H
