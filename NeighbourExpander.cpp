#include "NeighbourExpander.h"
#include "astar.h"
#include "map.h"
void NeighbourExpander::expandSuccessors(const NodeSavingF* node){
	for(NeighbourIterator it = m_holder->getMap()->begin(node);*it != NULL;++it)
    {
		m_holder->insertNodeToOpen(*it, node);
    }
}