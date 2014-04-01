#include <iostream>
#include "astar.h"
#include <fstream>
#include "map.h"
#include "node.h"
#include <string>
#include "Timer.h"
#include "JpsNeighbourExpander.h"
#include "NeighbourExpander.h"
using namespace std;

int main(int argc, char** argv)
{
    AStar astar;
    astar.setDistanceFunc(&Diagonal);
	NeighbourExpander ne(&astar);
	JpsNeighbourExpander jne(&astar);
	astar.setNeighbourExpander(&ne);

    

	string path = argv[1];
	string mapName = path + string("map.txt");
	string result = path + string("result.txt");

	Map map;
    map.setMap(mapName.c_str());
	
    astar.setMap(&map);
    NodeSavingF* start = map.getNode(4,0);
    NodeSavingF* end = map.getNode(500,511);
    astar.setStardAndEnd(*start, *end);

	ofstream os(result.c_str());
	Timer timer;
	timer.start();
	AStar::FindPathResult fpResult = astar.findPath(*start, *end);
	timer.end();
	os << "findPath time: "<< timer.getTimer() << endl;
    map.dumpMap(os, astar.getPath());
	
    
	getchar();
    return 0;
}

