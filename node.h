#ifndef NODE_H
#define NODE_H
//是否包含f的值？f = g+h,可以比较简单的计算出来。我之前自己实现的A*就是用这种计算的方式表示f的。现在，看看能不能提高下性能吧
#include <cstddef>
typedef int coord_type;
class Node
{
public:
    Node();//node是有坐标的，但是没有权重，也就是，每走一步，一定是增加了1（或者1.414）

    void init(const int x, const int y, const bool isBlock){
        m_x = x; m_y = y; m_isBlock = isBlock;
        /*f = */g = h = 0;
        m_parent = NULL;
    }
    void setH(const float h){this->h = h;}
    void setG(const float g){this->g = g; /*updateF();*/}
    void setParent(const Node* parent){m_parent = parent;}

    float getF() const {return g+h;}
    float getG() const {return g;}
    float getH() const {return h;}

    //void updateF(){f = g + h;}
    coord_type getX() const {return m_x;}
    coord_type getY() const {return m_y;}

    bool isBlock() const {return m_isBlock;}

    const Node* getParent() const{return m_parent;}

protected:
    //float f;
    float g;
    float h;

    coord_type m_x;
    coord_type m_y;
    bool m_isBlock;
    const Node* m_parent;
};
class NodeSavingF : public Node
{
public:
    NodeSavingF(){}
    void init(const int x, const int y, const bool isBlock){
        Node::init(x, y, isBlock);
        f = 0;
    }
    float getF() const {return f;}
    void setG(const float g){this->g = g; updateF();}
protected:
    float f;
    void updateF(){f = g + h;}
};

#endif // NODE_H
