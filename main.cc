#include "graph.h"
#include "dfs.h"
#include "detect.h"
#include "cycle.h"
#include "config.h"

#include <iostream>
#include <string>

int main (int argc, char* argv[]) {
	
	Config conf;
	conf.parseParameter(argc,argv);
	
	//Read file
	std::vector<Vertex> vertices = readEdgeList(conf);
	conf.printdebug("Graph loaded");
	
	conf.startClock();
	//Do all sources
	for(Vertex& v : vertices) {
		if (v.isSource()) {
			Vertex* start;
			create_postorder(&v, &start);
			conf.addOrder(start, &v, false);
			detect(start, &v, conf);
			finish(start, &v, false);
		}
	}
	
	conf.printdebug("Sources cleared");
	//Do rest of the graph
	for(Vertex& v: vertices) {
		if (v.stat == Vertex::CLEAN ) {
			cycle_search(&v, conf);
		}
	}
	conf.printdebug("Cycles cleared");
	conf.endClock();
	conf.writeComplete();
	conf.writeOrder();
	conf.writeTree();
	conf.writeStats(vertices);
	
	return 0;
}

