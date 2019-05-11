#ifndef ___GRAPH_H___
#define ___GRAPH_H___


#include <string>
#include <vector>

class Vertex;

#include "config.h"



class Neighbor {
	private:
		Vertex** start;
		Vertex** stop;
	public:
		void init(Vertex**);
		Vertex** begin();
		Vertex** end();
		void add(Vertex*);
		
};


class Vertex {
	private:
		std::string id;
		Neighbor pre;
		Neighbor suc;
		Vertex* next;
		Vertex* other;
		long unsigned int counter;
		
	public:
		//status
		short stat;
		bool entrance;
		bool exit;
		
		static const short CLEAN;
		static const short CYCLESEARCH;
		static const short CYCLEFIN;
		static const short CYCLE;
		static const short PATH;
		static const short PATHFIN;
		static const short DETECT;
		static const short FINISHED;
		static const short CC;
		static const short SCC;
		static const short SCCFin;
		
		//init
		void init(std::pair<std::string, long unsigned int*> );
		void addSuc(Vertex*);	
		void addPre(Vertex*);
		void resetCounter();
		
		//access data
		void print();
		void printE();
		bool isSource();
		bool isSink();
		Neighbor preIt();
		Neighbor succIt();
		std::string getID();
		
		//change and access vertex data
		void setNext(Vertex*);
		Vertex* getNext();
		void setmaxParent(Vertex*);
		Vertex* getmaxParent();
		void setCmin(Vertex*);
		Vertex* getCmin();
		void setCmax(Vertex*);
		Vertex* getCmax();
		
		//change and access vertex integer data
		void setCyclePos(long unsigned int);
		long unsigned int getCyclePos();
		void setPostorderPos(long unsigned int);
		long unsigned int getPostorderPos();
		void setLowlink(long unsigned int);
		long unsigned int getLowlink();
		void setPaths(long unsigned int);
		long unsigned int getPaths();
		
		
		
		
		
};

std::vector<Vertex> readEdgeList(Config&);

#endif