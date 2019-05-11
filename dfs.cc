#include "dfs.h"

#include <stack>

#include "config.h"

#include <iostream>

struct itobject {
	Vertex* pos;
	Vertex** children;
	Vertex** end;
};

void create_postorder(Vertex* current, Vertex** start) {
	std::stack<itobject> nexts;
	bool first = true;
	Vertex* last;
	itobject it;
	it.pos = current;
	(*current).stat = Vertex::DETECT;
	it.children = (*current).succIt().begin();
	it.end = (*current).succIt().end();
	nexts.push(it);
	
	while(!nexts.empty()) {
		if (nexts.top().children != nexts.top().end) {
			if ((**nexts.top().children).stat < Vertex::DETECT) {
				itobject it2;
				it2.pos = *nexts.top().children;
				(**nexts.top().children).stat = Vertex::DETECT;
				it2.children = (**nexts.top().children).succIt().begin();
				it2.end = (**nexts.top().children).succIt().end();
				nexts.top().children++;
				nexts.push(it2);
			}
			else {
				nexts.top().children++;
			}
			
		}
		else {
			if (first) {
				first = false;
				nexts.top().pos->setPostorderPos(0);
				*start = nexts.top().pos;
			}
			else {
				nexts.top().pos->setPostorderPos((*last).getPostorderPos()+1);
				(*last).setNext(nexts.top().pos);
			}
			last = nexts.top().pos;
			nexts.pop();
		}
	}
}

void create_postorder(Vertex* current, Vertex** start, long unsigned int& altpos) {
	std::stack<itobject> nexts;
	bool first = true;
	bool firstAlt = true;
	Vertex* last = NULL;
	itobject it;
	it.pos = current;
	(*current).stat = Vertex::DETECT;
	it.children = (*current).succIt().begin();
	it.end = (*current).succIt().end();
	nexts.push(it);
	while(!nexts.empty()) {
		Vertex& v =(*nexts.top().pos);
		if (nexts.top().children != nexts.top().end ) {
			if ((**nexts.top().children).stat < Vertex::DETECT) {
				itobject it2;
				it2.pos = *nexts.top().children;
				(**nexts.top().children).stat = Vertex::DETECT;
				it2.children = (**nexts.top().children).succIt().begin();
				it2.end = (**nexts.top().children).succIt().end();
				nexts.top().children++;
				nexts.push(it2);
			}
			else {
				if (*nexts.top().children == current && firstAlt) {
					firstAlt = false;
					if (first) {
						first = false;
						(*current).setPostorderPos(0);
						altpos = 0;
						*start = current;
					}
					else {
						(*current).setPostorderPos((*last).getPostorderPos()+1);
						altpos = (*last).getPostorderPos()+1;
						(*last).setNext(current);
					}
					last = current;
				}
				nexts.top().children++;
			}
		}
		else {
			if (first) {
				first = false;
				v.setPostorderPos(0);
				*start = nexts.top().pos;
			}
			else {
				v.setPostorderPos((*last).getPostorderPos()+1);
				(*last).setNext(nexts.top().pos);
			}
			last = nexts.top().pos;
			nexts.pop();
		}
	}
}

