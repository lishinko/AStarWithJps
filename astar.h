#ifndef ASTAR_H
#define ASTAR_H
#include "node.h"
#include "heap.h"
#include <cmath>
class Map;
class NeighbourExpander;
typedef dis_type (*DistanceFunc)(const /*Node**/NodeIdx base, const /*Node**/NodeIdx neighbour);
dis_type Euclidean(const /*Node**/NodeIdx base, const /*Node**/NodeIdx neighbour);
dis_type Diagonal(const /*Node**/NodeIdx base, const /*Node**/NodeIdx neighbour);
typedef dis_type (*DistanceFunc2)(const coord_type basex, const coord_type basey, const coord_type neighbourx, const coord_type neighboury);
dis_type Euclidean2(const coord_type basex, const coord_type basey, const coord_type neighbourx, const coord_type neighboury);
dis_type Diagonal2(const coord_type basex, const coord_type basey, const coord_type neighbourx, const coord_type neighboury);
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
	void setMap(/*const */Map* map){m_map = map; m_open.setMap(map);}
	void setNeighbourExpander(NeighbourExpander* expander){m_expander = expander;}
    //寻路算法，路径保存在Node的里面。
    FindPathResult findPath(const /*Node**/NodeIdx start, const /*Node**/NodeIdx end);

    void setStardAndEnd(const /*Node**/NodeIdx start, const /*Node**/NodeIdx end);
    void setDistanceFunc(DistanceFunc2 func){
        m_distanceFunc = func;
    }
    std::vector<const /*Node**/NodeIdx> getPath();

	const Map* getMap(){return m_map;}
	void pushToOpen(const /*Node**/NodeIdx node){m_open.push(node);}
	bool inOpen(const /*Node**/NodeIdx node) const{return m_open.has(node);}
	dis_type getDis(const /*Node**/NodeIdx node, const /*Node**/NodeIdx parent) const{
		const coord_type parentx = m_map->getRect().getParts().getX(parent);
		const coord_type parenty = m_map->getRect().getParts().getY(parent);
		const coord_type nodex = m_map->getRect().getParts().getX(node);
		const coord_type nodey = m_map->getRect().getParts().getX(node);
		return m_distanceFunc(parentx, parenty, nodex, nodey);
	}

	void insertNodeToOpen(/*Node**/NodeIdx node, const /*Node**/NodeIdx parent);
	const /*Node**/NodeIdx getEndNode(){return m_end;}
protected:
    FindPathResult findpathImpl();
    void expandSuccessors(const /*Node**/NodeIdx node);
    bool inClosed(const /*Node**/NodeIdx node){return std::find(m_closed.begin(), m_closed.end(), node) != m_closed.end();}
	


    /*const */Map* m_map;
    Heap m_open;
    std::vector<const /*Node**/NodeIdx> m_closed;//closed表就不需要排序了
    DistanceFunc2 m_distanceFunc;
	NeighbourExpander* m_expander;

    /*Node**/NodeIdx m_start;
    /*Node**/NodeIdx m_end;
};

#endif // ASTAR_H
