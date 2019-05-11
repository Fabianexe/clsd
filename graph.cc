#include "graph.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

Vertex** Neighbor::begin() {
	return start;
}
Vertex** Neighbor::end() {
	return stop;
}
void Neighbor::add(Vertex *a) {
	*stop = a;
	stop++;
}

void Neighbor::init(Vertex **a) {
	start = a;
	stop = a;
}

//status
const short Vertex::CLEAN = 0;
const short Vertex::CYCLESEARCH = 1;
const short Vertex::CYCLEFIN = 2;
const short Vertex::CYCLE = 3;
const short Vertex::PATH = 4;
const short Vertex::PATHFIN = 5;
const short Vertex::DETECT = 6;
const short Vertex::FINISHED = 7;
const short Vertex::CC = 8;
const short Vertex::SCC = 9;
const short Vertex::SCCFin = 10;


//init
void Vertex::init(std::pair<std::string, long unsigned int*> ele) {
	id = ele.first;
	suc.init( new Vertex*[ele.second[1]]);
	pre.init( new Vertex*[ele.second[2]]);
	stat = Vertex::CLEAN;
	counter = -1;
	next = NULL;
	other = NULL;
	entrance = false;
	exit = false;
}

void Vertex::addSuc(Vertex* t) {
	suc.add(t);
}

void Vertex::addPre(Vertex* s) {
	pre.add(s);
}

void Vertex::resetCounter() {
	counter = -1;
	next = NULL;
	other = NULL;
}

//access data
void Vertex::print() {
	std::cout << "Vertex " << id << "\t:";
	std::cout << "pre: ";
	for (Vertex* i: pre) {
		std::cout << (*i).getID();
		std::cout << ",";
	}
	std::cout << "\t;";
	std::cout << "suc: ";
	
	for (Vertex* i: suc) {
		std::cout << (*i).getID();
		std::cout << ",";
	}
	std::cout << "\t;";
	std::cout << "stat: " << stat << "\t;";
	std::cout << "counter: " << counter << "\t;";
	if (next == NULL) {
		std::cout << "next: NULL\t;";
	}
	else {
		std::cout << "next: " << next->id << "\t;";
	}
	if (other == NULL) {
		std::cout << "other: NULL\n";
	}
	else {
		std::cout << "other: " << other->id << "\n";
	}

}

void Vertex::printE() {
	std::cerr << "Vertex " << id << "\t:";
	std::cerr << "pre: ";
	for (Vertex* i: pre) {
		std::cerr << (*i).getID();
		std::cerr << ",";
	}
	std::cerr << "\t;";
	std::cerr << "suc: ";
	
	for (Vertex* i: suc) {
		std::cerr << (*i).getID();
		std::cerr << ",";
	}
	std::cerr << "\t;";
	std::cerr << "stat: " << stat << "\t;";
	std::cerr << "counter: " << counter << "\t;";
	if (next == NULL) {
		std::cerr << "next: NULL\t;";
	}
	else {
		std::cerr << "next: " << next->id << "\t;";
	}
	if (other == NULL) {
		std::cerr << "other: NULL\n";
	}
	else {
		std::cerr << "other: " << other->id << "\n";
	}
}

bool Vertex::isSource() {
	return pre.begin() == pre.end();
}

bool Vertex::isSink() {
	return suc.begin() == suc.end();
}

Neighbor Vertex::preIt() {
	return pre;
}

Neighbor Vertex::succIt() {
	return suc;
}

std::string Vertex::getID() {
	return id;
}

//change and access vertex data
void Vertex::setNext(Vertex* n) {
	next = n;
}

Vertex* Vertex::getNext() {
	return next;
}

void Vertex::setmaxParent(Vertex* par) {
	other = par;
}

Vertex* Vertex::getmaxParent() {
	return other;
}

void Vertex::setCmin(Vertex* pos) {
	next = pos;
}

Vertex* Vertex::getCmin() {
	return next;
}

void Vertex::setCmax(Vertex* pos) {
	other = pos;
}

Vertex*Vertex::getCmax() {
	return other;
}

//change and access vertex integer data
void Vertex::setPostorderPos(long unsigned int pos) {
	counter = pos;
}

long unsigned int Vertex::getPostorderPos() {
	return counter;
}

void Vertex::setCyclePos(long unsigned int pos) {
	counter = pos;
}

long unsigned int Vertex::getCyclePos() {
	return counter;
}

void Vertex::setLowlink(long unsigned int pos) {
	counter = pos;
}

long unsigned int Vertex::getLowlink() {
	return counter;
}

void Vertex::setPaths(long unsigned int number) {
	counter = number;
}

long unsigned int Vertex::getPaths() {
	return counter;
}



std::vector<Vertex> readEdgeList( Config& conf ) {
	std::string line;
	std::ifstream myfile (conf.getPath());
	std::vector<std::string*> edges;
	std::unordered_map<std::string, long unsigned int*> str2int;
	long unsigned int elemetcount = 0;
	long unsigned int multiedges = 0;
	std::unordered_map<std::string, std::unordered_set<std::string>> univertex;
	
	if (myfile.is_open()) {
		while ( getline (myfile,line) ) {
			//skip emtpy lines
			if (line.empty() || line[0] == '#') {
				continue;
			}
			//clean line end
			if ( line[line.size() - 1] == '\r') {
				line.erase(line.size() - 1);
			}
			//find spaces or tabs
			int pos = line.find(" ");
			if (pos <0) {
				pos = line.find("\t");
			}
			int pos2 = line.find(" ", pos+1);
			if (pos2 <0) {
				pos2 = line.find("\t", pos+1);
			}
			if (pos2 <0) {
				pos2 = line.length();
			}
			//generate strings
			std::string* vs = new std::string[2];
			vs[0] = line.substr(0, pos);
			vs[1] = line.substr(pos+1, pos2-pos-1);
			if ( !univertex[vs[0]].insert(vs[1]).second) {
				multiedges++;
			}
			else {
				//save edge
				edges.push_back(vs);
				//add tail of edge
				if (str2int.count(vs[0]) == 0) {
					long unsigned int* ele = new long unsigned int[3];
					ele[0] = elemetcount;
					ele[1] = 1;
					ele[2] = 0;
					str2int[vs[0]] = ele;
					elemetcount++;
				}
				else {
					str2int[vs[0]][1]++;
				}
				//add head of edge
				if (str2int.count(vs[1]) == 0) {
					long unsigned int* ele = new long unsigned int[3];
					ele[0] = elemetcount;
					ele[1] = 0;
					ele[2] = 1;
					str2int[vs[1]] = ele;
					elemetcount++;
				}
				else {
					str2int[vs[1]][2]++;
				}
			}
		}
		myfile.close();
	}
	else {
		std::cerr << "Error: Unable to open file"; 
		exit(1);
	}
	
	std::vector<Vertex> vertices(str2int.size());
	
	for (std::pair<std::string, long unsigned int*> element : str2int) {
		vertices[element.second[0]].init(element);
	}
	conf.setVertices(vertices.size());
	conf.setEdges(edges.size());
	conf.setMultiedges(multiedges);
	
	for (std::string* e: edges ) {
		long unsigned int s = str2int[e[0]][0];
		long unsigned int t = str2int[e[1]][0];
		vertices[t].addPre(&vertices[s]);
		vertices[s].addSuc(&vertices[t]);
	}
	
	for (Vertex& ele: vertices) {
		ele.resetCounter();
	}
	
	for(auto& p : str2int) {
		delete p.second;
	}
	
	return vertices;
	
}
