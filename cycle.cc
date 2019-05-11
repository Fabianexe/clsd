#include "cycle.h"
#include "dfs.h"
#include "detect.h"

#include <stack>

#include <iostream>

struct itobject {
	Vertex* pos;
	Vertex** children;
	Vertex** end;
};

long unsigned int cycle_distance(Vertex* first, Vertex* second, long unsigned int k ) {
	if (first->getCyclePos() < second->getCyclePos() ) {
		return second->getCyclePos() - first->getCyclePos();
	}
	else {
		return (k- first->getCyclePos()) + second->getCyclePos();
	}
}

std::vector<Vertex*> get_cycle(Vertex* start, Vertex* end) {
	std::vector<Vertex*> cycle;
	cycle.push_back(start);
	while (start != end) {
		start = start->getNext();
		cycle.push_back(start);
	}
	long unsigned int count = 0;
	for (std::vector<Vertex*>::reverse_iterator it2 = cycle.rbegin(); it2 != cycle.rend(); ++it2) { 
		(*it2)->stat = Vertex::CYCLE;
		(*it2)->setCyclePos(count++);
	}
	return cycle;
}

bool calc_cover_cut(std::vector<Vertex*>& cycle, Vertex** coververtex) {
	long unsigned int max = 0;
	long unsigned int n = 0;
	for (long unsigned int i = 0; i< cycle.size(); ++i) {
		Vertex* u =  cycle[i];
		if ( cycle_distance(u, u->getCmax(), cycle.size()) > max) {
			max = cycle_distance(u, u->getCmax(), cycle.size());
			n = i;
		}
	}
	Vertex* w = cycle[n];
	long unsigned int i = (n==0?cycle.size()-1: n-1);
	while (true) {
		Vertex* u = cycle[i];
		if (cycle_distance(w, u, cycle.size()) == max ) {
			//Cut point found
			*coververtex = cycle[i];
			return false;
		}
		if (cycle_distance(w, u, cycle.size()) > cycle_distance(w, u->getCmax(), cycle.size())) {
			//Cover found
			*coververtex = w->getCmax();
			return true;
		}
		if ( cycle_distance(w, u->getCmax(), cycle.size()) > max) {
			max =  cycle_distance(w, u->getCmax(), cycle.size());
		}
		i = (i==0?cycle.size()-1: i-1);
	}
}

void finish_subtree(Vertex* current) {
	std::stack<itobject> nexts;
	itobject it;
	it.pos = current;
	Vertex* root = current;
	it.children = root->succIt().begin();
	it.end = root->succIt().end();
	nexts.push(it);
	
	while(!nexts.empty()) {
		if (nexts.top().children != nexts.top().end) {
			//Check next
			if ((*nexts.top().children)->stat == Vertex::PATH) {
				//Befor childreen
				(*nexts.top().children)->stat = Vertex::PATHFIN;
				(*nexts.top().children)->setCmax(root->getCmax());
				(*nexts.top().children)->setCmin(root->getCmin());
				//Put on stack
				itobject it2;
				it2.pos = *nexts.top().children;
				it2.children = (*nexts.top().children)->succIt().begin();
				it2.end = (*nexts.top().children)->succIt().end();
				nexts.top().children++;
				nexts.push(it2);
			}
			else {
				nexts.top().children++;
			}
		}
		else {
			//After childreen
			nexts.pop();
		}
	}
}

struct pathitobject {
	Vertex* pos;
	Vertex** children;
	Vertex** end;
	long unsigned int inpos;
};
bool path_search(std::vector<Vertex*>& cycle, Vertex** coververtex) {
	//Some help variables
	std::stack<pathitobject> nexts;
	long unsigned int k = cycle.size();
	for (Vertex* cyclevertex: cycle) {
		//put cycle vertex on stack
		long unsigned int inpos = 0;
		pathitobject it;
		it.pos = cyclevertex;
		it.children = cyclevertex->succIt().begin();
		it.end = cyclevertex->succIt().end();
		it.inpos = inpos++;
		nexts.push(it);
		//Prepartion of cycle vertex:
		cyclevertex->setCmax(NULL);
		
		while(!nexts.empty()) {
			Vertex* v = nexts.top().pos;
			if (nexts.top().children != nexts.top().end) {
				//Check next
				Vertex* suc = *nexts.top().children;
				//sucessor is a cycle vertex
				if (suc->stat == Vertex::CYCLE) {
					//Update max value
					if (v->getCmax()== NULL || cycle_distance(cyclevertex, suc, k) > cycle_distance(cyclevertex, v->getCmax(), k)) {
						v->setCmax(*nexts.top().children);
					}
					//No min value update for cycle vertex
					if (v->stat != Vertex::CYCLE) {
						//Update min value
						if (v->getCmin() == NULL || cycle_distance(cyclevertex, suc, k) < cycle_distance(cyclevertex, v->getCmin(), k)) {
							v->setCmin(*nexts.top().children);
						}
					}
					nexts.top().children++;
				}
				//When result not finished
				else if (suc->stat  == Vertex::PATH) {
					//update lowlink
					if (suc->getLowlink() < v->getLowlink()) {
						v->setLowlink(suc->getLowlink());
					}
					if (suc->getCmax() != NULL ) {
						//Update max value
						if (v->getCmin()== NULL || cycle_distance(cyclevertex, suc->getCmax(), k) > cycle_distance(cyclevertex, v->getCmax(), k)) {
							v->setCmax(suc->getCmax());
						}
						//No min value update for cycle vertex
						if (v->stat != Vertex::CYCLE) {
							//Update min value
							if (v->getCmin()== NULL || cycle_distance(cyclevertex, suc->getCmin(), k) < cycle_distance(cyclevertex, v->getCmin(), k)) {
								v->setCmin(suc->getCmin());
							}
						}
					}
					nexts.top().children++;
				}
				else if (suc->stat  == Vertex::PATHFIN && suc->getCmax() != NULL ) {
					//Check for one vertex cover
					if (cycle_distance(cyclevertex, suc->getCmax(), k) < cycle_distance(cyclevertex, suc->getCmin(), k)) {
						*coververtex = suc->getCmax();
						return true;
					}
					//Update max value
					if (v->getCmin()== NULL || cycle_distance(cyclevertex, suc->getCmax(), k) > cycle_distance(cyclevertex, v->getCmax(), k)) {
						v->setCmax(suc->getCmax());
					}
					//No min value update for cycle vertex
					if (v->stat != Vertex::CYCLE) {
						//Update min value
						if (v->getCmin()== NULL || cycle_distance(cyclevertex, suc->getCmin(), k) < cycle_distance(cyclevertex, v->getCmin(), k)) {
							v->setCmin(suc->getCmin());
						}
					}
					nexts.top().children++;
				}
				//recusive call only when not called
				else if (suc->stat < Vertex::PATH) {
					//Befor childreen
					suc->resetCounter();
					suc->stat = Vertex::PATH;
					suc->setLowlink(inpos);
					//Put on stack
					pathitobject it2;
					it2.pos = *nexts.top().children;
					it2.children = suc->succIt().begin();
					it2.end = suc->succIt().end();
					it2.inpos = inpos++;
					nexts.push(it2);
				}
				else {
					nexts.top().children++;
				}
			}
			else {
				//After childreen
				//SCC root found?
				if (v->stat != Vertex::CYCLE && v->getLowlink() == nexts.top().inpos) {
					v->stat = Vertex::PATHFIN;
					//Finish SCC
					finish_subtree(nexts.top().pos);
				}
				nexts.pop();
			}
		}
		finish_subtree(cyclevertex );
	}
	return false;
}


void cycle_search(Vertex* current, Config& conf ) {
	std::stack<itobject> nexts;
	itobject it;
	it.pos = current;
	current->stat = Vertex::CYCLESEARCH;
	it.children = current->succIt().begin();
	it.end = current->succIt().end();
	nexts.push(it);
	
	while(!nexts.empty()) {
		if (nexts.top().pos->stat != Vertex::CYCLESEARCH) {
			nexts.pop();
			continue;
		}
		if (nexts.top().children != nexts.top().end) {
			//Check next
			if ((*nexts.top().children)->stat == Vertex::CLEAN) {
				//Befor childreen
				(*nexts.top().children)->setNext(nexts.top().pos);
				//Put on stack
				itobject it2;
				it2.pos = *nexts.top().children;
				(*nexts.top().children)->stat = Vertex::CYCLESEARCH;
				it2.children = (*nexts.top().children)->succIt().begin();
				it2.end = (*nexts.top().children)->succIt().end();
				nexts.top().children++;
				nexts.push(it2);
			}
			else if ((*nexts.top().children)->stat == Vertex::CYCLESEARCH) {
				//Get cycle
				std::vector<Vertex*> cycle = get_cycle(nexts.top().pos, *nexts.top().children );
				// Cover or cut vertex
				Vertex* coververtex;
				//generate cycle path
				bool cover = path_search(cycle, &coververtex );
				if (!cover) {
					//Calc cover cut
					cover = calc_cover_cut(cycle, &coververtex );
				}
				
				if (cover) {
					Vertex* start;
					create_postorder(coververtex, &start);
					conf.addOrder(start, coververtex, false);
					detect(start, coververtex, conf );
					finish(start, coververtex, false);
				}
				else {
					Vertex* start;
					long unsigned int altPos = -1;
					
					create_postorder(coververtex, &start, altPos);
					conf.addOrder(start, coververtex, true);
					detect(start, coververtex, altPos, conf );
					finish(start, coververtex, true);
				}
				nexts.pop();
			}
			else {
				nexts.top().children++;
			}
		}
		else {
			//After childreen
			nexts.top().pos->stat = Vertex::CYCLEFIN;
			
			nexts.pop();
		}
	}
}
