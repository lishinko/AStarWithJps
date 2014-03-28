#ifndef NODERECT_H
#define NODERECT_H

#include "node.h"
class NodeRect
{
public:
    NodeRect();
    ~NodeRect(){delete[] m_nodes;}
    void allocateNodes(const int width, const int height);
    Node *getNode(const int x, const int y) const;
    int getWidth() const{return m_width;}
    int getHeight() const{return m_height;}
protected:
    Node* m_nodes;//节点的二维数组。c++好像不能动态创建，就手动计算下吧
    int m_width;
    int m_height;
};

#endif // NODERECT_H
