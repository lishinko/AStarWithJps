#include "map.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cassert>
using namespace std;
Map::Map()
{
}

void Map::setMap(const char *mapString)
{
    ifstream is(mapString);
    if(!is.is_open())
        return;
    string widthHeight;
    getline(is, widthHeight);

    int width = 0;
    int height = 0;
    sscanf(widthHeight.c_str(), "%d %d\n",&width, &height);
    m_nodes.allocateNodes(width, height);

    for(int y = 0; y < height; y++){
        string tmp;
        getline(is, tmp);
        for(int x = 0; x < width; x++){
            char c = tmp.at(x);
            if(c == ' '){
                /*Node**/NodeIdx node = m_nodes.getNode(x, y);
				assert(node != -1);
                //node->init(x,y,false);
				m_nodes.getParts().init(node, x, y, false);
            }
            else if(c == '*'){
                /*Node**/NodeIdx node = m_nodes.getNode(x, y);
				assert(node != -1);
                //node->init(x,y,true);
				m_nodes.getParts().init(node, x, y, true);
            }
            else{
                assert(0);
            }
        }
    }
	is.close();
}

void Map::dumpMap(ofstream& os, const std::vector<const /*Node**/NodeIdx>& path)
{
	for(std::vector<const /*Node**/NodeIdx>::const_iterator it = path.begin(); it != path.end(); ++it){
        //os << "(" <<(*it)->getX() << "," << (*it)->getY() << ")->";
		os << "(" << getRect().getParts().getX(*it) << "," << getRect().getParts().getY(*it) << ")->";
    }
	os << endl;

    for(int y = 0; y < m_nodes.getHeight(); y++){
        for(int x = 0; x < m_nodes.getWidth(); x++){
            const /*Node**/NodeIdx node = getNode(x,y);
			/*if(node->getParent() != NULL)*/if(m_nodes.getParts().getParent(node) != -1)
            {
                os << "-";
            }
			else /*if(node->isBlock())*/if(m_nodes.getParts().isBlock(node)){
                os << "*";
            }
            else{
                os << " ";
            }
        }
        os << endl;
    }
}
