#ifndef HEAP_H
#define HEAP_H
#include <vector>
#include <algorithm>
#include "node.h"
#include "map.h"
class HeapCompare_f
{
public:
	HeapCompare_f(){}
	void setMap(const Map* map){parent = map;}
    bool operator() ( const /*Node**/NodeIdx x, const /*Node**/NodeIdx y ) const
    {
		return parent->getF(x) > parent->getF(y);
        //return x->getF() > y->getF();
    }
protected:
	const Map* parent;
};

class Heap
{
public:
	void setMap(const Map* map){m_compareFunc.setMap(map);}
    Heap(){}
    void push(const /*Node**/NodeIdx node){
        m_list.push_back(node);
        push_heap(m_list.begin(), m_list.end(),m_compareFunc);
    }
    const /*Node**/NodeIdx pop(){
		if(m_list.empty()){
			return -1;
		}
        const /*Node**/NodeIdx n = m_list.front(); // get pointer to the node
        pop_heap( m_list.begin(), m_list.end(), m_compareFunc );
        m_list.pop_back();
        return n;
    }
    bool empty(){return m_list.empty();}
    bool has(const /*Node**/NodeIdx node) const{return std::find(m_list.begin(), m_list.end(), node) != m_list.end();}
    void sort(){make_heap(m_list.begin(), m_list.end(),m_compareFunc);}
protected:
    std::vector<const /*Node**/NodeIdx> m_list;
	HeapCompare_f m_compareFunc;
};

#endif // HEAP_H
