#ifndef ___CONFIG_H___
#define ___CONFIG_H___

#include <vector>
#include <ctime>
#include <fstream>
#include <string>

class Config;

#include "graph.h"

struct orderElement {
	Vertex* start;
	Vertex* end;
	bool cut;
};

struct superbubble {
	Vertex* entrance;
	Vertex* exit;
};

class Config {
	private:
		std::vector<orderElement> orders;
		std::vector<superbubble> bublles;
		long unsigned int vertices;
		long unsigned int edges;
		long unsigned int multiedges;
		std::clock_t time;
		std::clock_t runtime;
		std::string path; 
		bool noout; 
		std::string outpath; 
		bool order; 
		std::string orderpath; 
		bool stats; 
		std::string statspath; 
		bool trees; 
		std::string treespath; 
		bool debug; 
		bool undirected; 
		bool directed; 
	public:
		void addOrder(Vertex*, Vertex*, bool);
		std::vector<orderElement>::iterator begin();
		std::vector<orderElement>::iterator end();
		void addSuperbubble(Vertex*, Vertex*);
		void sort();
		void startClock();
		void endClock();
		void setVertices(long unsigned int);
		void setEdges(long unsigned int);
		void setMultiedges(long unsigned int);
		void writeComplete();
		void writeOrder();
		void writeStats(std::vector<Vertex>&);
		void writeTree();
		void parseParameter(int, char*[]);
		void printdebug(std::string);
		std::string getPath();
};

#endif