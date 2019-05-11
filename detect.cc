#include "detect.h"
#include <stack>
#include <iostream>


Vertex* out_child(Vertex* current, bool& dis) {
	if (dis || current->isSink()) {
		dis = true;
		return 0;
	}
	Vertex* minimum = NULL;
	for (Vertex*  v: current->succIt() ) {
		if (v->stat != Vertex::DETECT || v->getPostorderPos() >= current->getPostorderPos()) {
			dis = true;
			return 0;
		}
		if (minimum == NULL || v->getPostorderPos() < minimum->getPostorderPos()) {
			minimum = v;
		}
	}
	return minimum;
}

Vertex* out_parent(Vertex* current, bool& dis) {
	if (current->isSource()) {
		dis = true;
		return 0;
	}
	Vertex* maximum = NULL;
	for (Vertex* v: current->preIt() ) {
		if (v->stat != Vertex::DETECT || v->getPostorderPos() <= current->getPostorderPos()) {
			dis = true;
			return 0;
		}
		if (maximum == NULL || v->getPostorderPos() > maximum->getPostorderPos()) {
			maximum = v;
		}
	}
	return maximum;
}


void detect(Vertex* start , Vertex* end, Config& conf ) {
	std::stack<Vertex*> endstack;
	Vertex* last;
	bool dis = false;
	
	while (true) {
		//Calculate outchild value
		Vertex* c = out_child(start, dis);
		//discard any possible end
		if (dis) {
			while (! endstack.empty()) {
				endstack.pop();
			}
			dis = false;
			
		}
		//check end
		else {
			//possible exit
			if ( c->getPostorderPos() == last->getPostorderPos() ) {
				endstack.push(last);
			}
			else {
				while (!endstack.empty() && endstack.top()->getPostorderPos()> c->getPostorderPos() ) {
					Vertex* max = endstack.top()->getmaxParent();
					endstack.pop();
					if (!endstack.empty() && endstack.top()->getmaxParent()->getPostorderPos() < max->getPostorderPos() ) {
						endstack.top()->setmaxParent(max);
					}
				}
			}
			//validate entrance
			if (!endstack.empty() && endstack.top()->getmaxParent() == start) {
				bool weak = false;
				for (Vertex* v: endstack.top()->succIt() ) {
					if (start == v) {
						weak = true;
						break;
					}
				} 
				if (!weak) {
					//report
					conf.addSuperbubble(start, endstack.top());
//					std::cout << "<" << start->getID() << "," << endstack.top()->getID() << ">\n";
				}
				endstack.pop();
			}
			
		}
		//update parent value
		Vertex* p = out_parent(start, dis);
		//discard any possible end
		if (!dis) {
			start->setmaxParent(p);
			if (!endstack.empty() && endstack.top()->getmaxParent()->getPostorderPos() < p->getPostorderPos() ) {
				endstack.top()->setmaxParent(p);
			}
		}
		
		//when complete postorder seen		
		if (start == end) {
			break;
		}
		//save vertex for later
		last = start;
		//get next vertex
		start = start->getNext();
	}
}



long unsigned int get_firstpos(Vertex* current, Vertex* end, long unsigned int altPos) {
	if (current == end) {
		return altPos;
	}
	return current->getPostorderPos();
}


Vertex* out_child(Vertex* current, bool& dis, Vertex* end, bool first, long unsigned int altPos) {
	if (dis || current->isSink()) {
		dis = true;
		return 0;
	}
	long unsigned int currentPos;
	if (first) {
		currentPos = get_firstpos(current, end, altPos);
	}
	else {
		currentPos = current->getPostorderPos();
	}
	Vertex* minimum = NULL;
	for (Vertex* v: current->succIt() ) {
		long unsigned int pos = get_firstpos(v, end, altPos);
		if (v->stat != Vertex::DETECT || pos >= currentPos) {
			dis = true;
			return 0;
		}
		if (minimum== NULL || pos < get_firstpos(minimum, end, altPos)) {
			minimum = v;
		}
	}
	return minimum;
}

Vertex* out_parent(Vertex* current, bool& dis, Vertex* end, bool first, long unsigned int altPos) {
	long unsigned int currentPos;
	if (first ) {
		currentPos = get_firstpos(current, end, altPos);
	}
	else {
		currentPos = current->getPostorderPos();
	}
	Vertex* maximum = NULL;
	for (Vertex* v: current->preIt() ) {
		if (v->stat != Vertex::DETECT || v->getPostorderPos() <= currentPos) {
			dis = true;
			return 0;
		}
		if (maximum== NULL || v->getPostorderPos() > maximum->getPostorderPos()) {
			maximum = v;
		}
	}
	return maximum;
}


void detect(Vertex* start , Vertex* end, long unsigned int altPos, Config& conf ) {
	std::stack<Vertex*> endstack;
	bool first = true;
	Vertex* last = 0;
	bool dis = false;
	while (true) {
		//Calculate outchild value
		Vertex* c = out_child(start, dis, end, first, altPos);
		//discard any possible end
		if (dis) {
			while (! endstack.empty()) {
				endstack.pop();
			}
			dis = false;
		}
		//check end
		else {
			//possible exit
			if ( get_firstpos(c, end, altPos) == get_firstpos(last, end, altPos) ) {
				endstack.push(last);
			}
			else {
				while (!endstack.empty() && get_firstpos(endstack.top(),end,altPos) > get_firstpos(c, end, altPos)) {
					Vertex* max = endstack.top()->getmaxParent();
					endstack.pop();
					if (!endstack.empty() && get_firstpos(endstack.top()->getmaxParent(),end,altPos) < get_firstpos(max, end, altPos) ) {
						endstack.top()->setmaxParent(max);
					}
				}
			}
			
			//validate entrance
			if (!endstack.empty()) {
				if ( endstack.top()->getmaxParent() == start) {
					if (start != endstack.top()) {
						bool weak = false;
						for (Vertex* v: endstack.top()->succIt() ) {
							if (start == v) {
								weak = true;
								break;	
							}
						} 
						if (!weak) {
							//report
							conf.addSuperbubble(start, endstack.top());
//							std::cout << "<" << start->getID() << "," << endstack.top()->getID() << ">\n";
						}
					}
					endstack.pop();
				}
			}
			
		}
		
		//update parent value
		Vertex* p = out_parent(start, dis, end, first, altPos);
		//discard any possible end
		if (!dis) {
			start->setmaxParent(p);
			if (! endstack.empty()) {
				if (endstack.top()->getmaxParent()->getPostorderPos() < p->getPostorderPos() ) {
					endstack.top()->setmaxParent(p);
				}
			}
		}
		
		//when complete postorder seen		
		if (start == end) {
			if (first) {
				first = false;
			}
			else {
				break;
			}
		}
		//save vertex for later
		last = start;
		//get next vertex
		start = start->getNext();
	}
}

void finish(Vertex* start , Vertex* end, bool first) {
	while (true) {
		start->stat = Vertex::FINISHED;
		if (start == end) {
			if (first) {
				first = false;
			}
			else {
				break;
			}
		}
		//get next vertex
		start = start->getNext();
	}
}
