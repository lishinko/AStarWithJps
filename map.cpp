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
                Node* node = m_nodes.getNode(x, y);
				assert(node);
                node->init(x,y,false);
            }
            else if(c == '*'){
                Node* node = m_nodes.getNode(x, y);
				assert(node);
                node->init(x,y,true);
            }
            else{
                assert(0);
            }
        }
    }
	is.close();
}

void Map::dumpMap()
{
    for(int y = 0; y < m_nodes.getHeight(); y++){
        for(int x = 0; x < m_nodes.getWidth(); x++){
            const Node* node = getNode(x,y);
            if(node->getParent() != NULL){
                cout << "-";
            }
            else if(node->isBlock()){
                cout << "*";
            }
            else{
                cout << " ";
            }
        }
        cout << endl;
    }
}
