#include <iostream>
#include "astar.h"
#include <fstream>
#include "map.h"
#include "node.h"
using namespace std;

int main(int argc, char** argv)
{
    AStar astar;
    astar.setDistanceFunc(&Diagonal);
    Map map;
    map.setMap(argv[1]);
    astar.setMap(&map);
    Node* start = map.getNode(0,0);
    Node* end = map.getNode(4,4);
//    astar.setStardAndEnd(*start, *end);
//    if(astar.findPath(*start, *end) == AStar::fp_FoundRoute){
//        std::vector<const Node*> path = astar.getPath();
//        for(std::vector<const Node*>::const_iterator it = path.begin(); it != path.end(); ++it){
//            cout << "(" <<(*it)->getX() << "," << (*it)->getY() << ")->";
//        }
//    }
    map.dumpMap();
    return 0;
}

