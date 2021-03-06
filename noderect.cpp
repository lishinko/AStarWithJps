#include "noderect.h"
#include <cassert>
NodeRect::NodeRect()
: m_nodes(NULL)
{
}

void NodeRect::allocateNodes(const int width, const int height)
{
    assert(width > 0 && height > 0);
    m_width = width;
    m_height = height;
	m_nodes = new NodeSavingF[m_width * m_height];
}

NodeSavingF* NodeRect::getNode(const int x, const int y) const
{
//    assert(x < m_width && y < m_height);
//    assert(x >= 0 && y >= 0);
    if(x >= 0 && y >= 0
       && x < m_width && y < m_height)
    {
		//手动计算这个真的很容易失误啊。。
        //return &m_nodes[x * m_height + y];
		return &m_nodes[y * m_width + x];
    }
    return NULL;
}
