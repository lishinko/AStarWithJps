#include "NeighbourExpander.h"
#include "astar.h"
#include "map.h"
void NeighbourExpander::expandSuccessors(const /*Node**/NodeIdx node){
	for(NeighbourIterator it = m_holder->getMap()->begin(node);*it != -1;++it)
    {
		m_holder->insertNodeToOpen(*it, node);
    }
}