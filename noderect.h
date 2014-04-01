#ifndef NODERECT_H
#define NODERECT_H

#include "node.h"
typedef int NodeIdx;
template<typename T>
struct Array{
	Array():arr(NULL){}
	~Array(){delete[] arr;}
	T* arr;
	void allocateNodes(const int width, const int height){
		arr = new T[width * height];
	}
	T& operator[](const NodeIdx idx){
		return arr[idx];
	}
	const T& operator[] (const NodeIdx idx) const{
		return arr[idx];
	}
};
struct NodeParts{
	//不变的数据部分
	Array<coord_type> m_xs;
	Array<coord_type> m_ys;
	Array<bool> m_isBlocks;

	//可变的部分
	//Array<dis_type> fs;
    Array<dis_type> gs;
    Array<dis_type> hs;
	//Array<const Node> m_parent;
	Array<NodeIdx> m_parentIdx;
	//Array<Node*>
	void allocateNodes(const int width, const int height);
	const dis_type getF(const NodeIdx idx) const{return gs[idx] + hs[idx];}
	const dis_type getG(const NodeIdx idx) const{return gs[idx];}
	const dis_type getH(const NodeIdx idx) const{return hs[idx];}
	const coord_type getX(const NodeIdx idx) const{return m_xs[idx];}
	const coord_type getY(const NodeIdx idx) const{return m_ys[idx];}
	bool isBlock(const NodeIdx idx){return m_isBlocks[idx];}
	const bool isBlock(const NodeIdx idx) const{return m_isBlocks[idx];}
	NodeIdx getParent(const NodeIdx idx){return m_parentIdx[idx];}
	const NodeIdx getParent(const NodeIdx idx) const {return m_parentIdx[idx];}

	void init(const int idx, const coord_type x, const coord_type y, const bool isBlock){
		setX(idx, x);
		setY(idx, y);
		setIsBlock(idx, isBlock);
		setG(idx, 0);
		setH(idx, 0);
		setParent(idx, -1);
	}
	void setG(const int idx, dis_type value){gs[idx] = value;}
	void setH(const int idx, dis_type value){hs[idx] = value;/*updateF(idx)*/;}
	void setX(const int idx, coord_type coord){m_xs[idx] = coord;}
	void setY(const int idx, coord_type coord){m_ys[idx] = coord;}
	void setIsBlock(const int idx, bool block){m_isBlocks[idx] = block;}
	void setParent(const int idx, NodeIdx parent){m_parentIdx[idx] = parent;}
};
class NodeRect
{
public:
    NodeRect();
    ~NodeRect(){/*delete[] m_nodes;*/}
    void allocateNodes(const int width, const int height);
    /*Node**/NodeIdx getNode(const coord_type x, const coord_type y) const;
	NodeParts& getParts() {return m_nodeParts;}
	const NodeParts& getParts() const {return m_nodeParts;}
    int getWidth() const{return m_width;}
    int getHeight() const{return m_height;}
protected:
    //Node* m_nodes;//节点的二维数组。c++好像不能动态创建，就手动计算下吧
	NodeParts m_nodeParts;
    int m_width;
    int m_height;
};

#endif // NODERECT_H
