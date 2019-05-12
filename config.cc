#include "config.h"

#include <algorithm>
#include <iostream>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <exception>


void Config::addOrder(Vertex* start, Vertex* end, bool cut) {
	orderElement ord;
	ord.start = start;
	ord.end = end;
	ord.cut = cut;
	orders.push_back(ord);
}
std::vector<orderElement>::iterator Config::begin() {
	return orders.begin();
}
std::vector<orderElement>::iterator Config::end() {
	return orders.end();
}
void Config::addSuperbubble(Vertex* entrance, Vertex* exit) {
	entrance -> entrance = true;
	exit -> exit = true;
	superbubble s;
	s.entrance = entrance;
	s.exit = exit;
	bublles.push_back(s);
}

bool compareBubble(superbubble& first, superbubble& second) {
	int c = first.entrance->getID().compare(second.entrance->getID());
	if (c< 0) {
		return true;
	}
	if (c==0) {
		if (first.exit->getID().compare(second.exit->getID())<0) {
			return true;
		}
	}
	return false;
}

void Config::sort() {
	std::sort(bublles.begin(), bublles.end(), compareBubble);
}
void Config::startClock() {
	time = std::clock();
}
void Config::endClock() {
	time = 1000.0 * (std::clock()-time) / CLOCKS_PER_SEC;
}

void Config::setVertices(long unsigned int size) {
	vertices = size;
} 
void Config::setEdges(long unsigned int size) {
	edges = size;
}
void Config::setMultiedges(long unsigned int size) {
	multiedges = size;
}
void Config::writeComplete() {
	if (noout) {
		return;
	}
	std::ostream* output = &std::cout;
	if (outpath != "-") {
		output = new std::ofstream(outpath);
	}
	sort();
	*output << "Vertices: " << vertices <<"\n";
	*output << "Edges: " << edges <<"\n";
	*output << "Elapsed time for processing: "<< time/1000.0 <<" secs.\n";
	*output << "Number of superbubbles found: " << bublles.size() << ".\n";
	for (superbubble& s: bublles) {
		*output << "<" << s.entrance->getID() << "," <<  s.exit->getID()<< ">\n";
	}
	if (outpath != "-") {
		(*((std::ofstream*)output)).close();
		delete output;
	}
	printdebug("Superbubble written");
}

void Config::writeOrder() {
	if (!order) {
		return;
	}
	std::ostream* output = &std::cout;
	if (orderpath != "-") {
		output = new std::ofstream(orderpath);
	}
	long unsigned int pos = 0;
	for (orderElement o: orders) {
		Vertex* start = o.start;
		bool first = o.cut;
		while (true) {
			*output << pos++ << "\t" << start->getID() << "\t";
			if (start-> entrance) {
				if (start-> exit) {
					*output << "X";
				}
				else {
					*output << ")";
				}
			}
			else if (start-> exit) {
				*output << "(";
			}
			*output << "\n";
			if (start == o.end) {
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
	if (orderpath != "-") {
		(*((std::ofstream*)output)).close();
		delete output;
	}
	printdebug("Order written");
}

struct DEGLIST {
	long unsigned int v;
	long unsigned int r;
	DEGLIST* n;
};

struct dfsstackobject {
	Vertex* v;
	Vertex** children;
	Vertex** end;
	long unsigned int inpos;
};

void Config::writeStats(std::vector<Vertex>& vertexList) {
	if (!stats) {
		return;
	}
	long unsigned int n = vertices;
	long unsigned int m = edges;
	long unsigned int deg;
	float ME ;
	float GD;
	long unsigned int CC;
	double r;
	double ss;
	if (undirected) {
		// Global properties
		deg = 0;
		
		for (Vertex& v: vertexList) {
			long unsigned int ldeg = (v.preIt().end()-v.preIt().begin()) + (v.succIt().end()-v.succIt().begin());
			if (ldeg > deg) {
				deg = ldeg;
			}
		}
		ME = 1.0*multiedges/(multiedges+edges);
		GD = (1.0*m)/(n*(n-1));
			//CC
		CC = 0;
		std::stack<Vertex*> nexts;
		for (Vertex& v: vertexList) {
			if (v.stat != Vertex::CC) {
				CC++;
				nexts.push(&v);
				v.stat = Vertex::CC;
				while(!nexts.empty()) {
					
					Vertex* next = nexts.top();
					nexts.pop();
					for (Vertex* u: next->succIt()) {
						if (u->stat != Vertex::CC) {
							u->stat = Vertex::CC;
							nexts.push(u);
						}
					}
					for (Vertex* u: next->preIt()) {
						if (u->stat != Vertex::CC) {
							u->stat = Vertex::CC;
							nexts.push(u);
						}
					}
				}
			}
		}
		long unsigned int sumj=0;
		long unsigned int sumj2=0;
		long unsigned int sumk=0;
		long unsigned int sumk2=0;
		long unsigned int sumjk=0;
		long unsigned int sums=0;
		std::vector<long unsigned int> degrees;
		for (Vertex& v: vertexList) {
			long unsigned int out = v.succIt().end()-v.succIt().begin();
			long unsigned int degv = out + (v.preIt().end()-v.preIt().begin());
			degrees.push_back(degv);
			for (Vertex* u: v.succIt()) {
				long unsigned int in = u->preIt().end()-u->preIt().begin();
				sumj += in-1;
				sumj2 += std::pow(in-1,2);
				sumk += out-1;
				sumk2 += std::pow(out-1,2);
				sumjk += (in-1)*(out-1);
				long unsigned int degu = in + (u->succIt().end()-u->succIt().begin());
				sums += degv*degu;
			}
		}
		
		std::sort(degrees.begin(),degrees.end(), std::greater<long unsigned int>());
		
		DEGLIST* s = new DEGLIST;
		DEGLIST* startelement = s;
		for (long unsigned int degi: degrees) {
			DEGLIST* a = new DEGLIST;
			a->v  = degi;
			a->r = degi;
			a->n = 0;
			s->n = a;
			s = a;
		}
		
		s = startelement->n;
		delete startelement;
		long unsigned int smax=0;
		while (s) {
			long unsigned int steps = s->r;
			DEGLIST* a = s->n;
			DEGLIST* l = s;
			while(steps != 0 && a != 0) {
				smax += s->v*a->v;
				a->r--;
				if (! a->r) {
					l->n = a->n;
					delete a;
				}
				else {
					l = a;
				}
				a = l->n;
				steps--;
			}
			l = s;
			s = s->n;
			delete l;
		}
		
		r = (sumjk-(sumj*sumk/(1.0*m)))/std::pow((sumj2-std::pow(sumj,2)/(1.0*m))*(sumk2-std::pow(sumk,2)/(1.0*m)),0.5);
		
		ss = 1.0*sums /smax;
		
		printdebug("Stats Undirected calculated");
	}
	
	long unsigned int inDeg;
	long unsigned int outDeg;
	double ajin;
	double ajout;
	double akin;
	double akout;
	long unsigned int SCC;
	double BE;
	double rinin;
	double rinout;
	double routin;
	double routout;
	double H;
	if (directed) {
		// Directed properties
		inDeg=0;
		outDeg=0;
		long unsigned int sumjin=0;
		long unsigned int sumjout=0;
		long unsigned int sumkin=0;
		long unsigned int sumkout=0;
		for (Vertex& v: vertexList) {
			long unsigned int in = v.preIt().end()-v.preIt().begin();
			if (in > inDeg) {
				inDeg = in;
			}
			
			long unsigned int out = v.succIt().end()-v.succIt().begin();
			if (out > outDeg) {
				outDeg = out;
			}
			for (Vertex* v2: v.succIt()) {
				sumjin += in;
				sumjout += out;
				sumkin += v2->preIt().end()-v2->preIt().begin();
				sumkout += v2->succIt().end()-v2->succIt().begin();
			}
		}
		ajin = 1.0*sumjin/m;
		ajout = 1.0*sumjout/m;
		akin = 1.0*sumkin/m;
		akout = 1.0*sumkout/m;
		
			//SCC
		SCC = 0;
		for (Vertex& v: vertexList) {
			if (v.stat < Vertex::SCCFin) {
				long unsigned int inpos = 0;
				std::stack<dfsstackobject> nexts;
				dfsstackobject it;
				it.v = &v;
				v.stat = Vertex::SCC;
				it.inpos = inpos;
				v.setLowlink(inpos++);
				it.children = v.succIt().begin();
				it.end = v.succIt().end();
				nexts.push(it);
				std::vector<Vertex*> unfinished;
				
				while(!nexts.empty()) {
					if (nexts.top().children != nexts.top().end) {
						if ((**nexts.top().children).stat < Vertex::SCC) {
							dfsstackobject it2;
							it2.v = *nexts.top().children;
							(**nexts.top().children).stat = Vertex::SCC;
							it2.inpos = inpos;
							(**nexts.top().children).setLowlink(inpos++);
							it2.children = (**nexts.top().children).succIt().begin();
							it2.end = (**nexts.top().children).succIt().end();
							nexts.push(it2);
						}
						else if ((**nexts.top().children).stat == Vertex::SCC) {
							if ((**nexts.top().children).getLowlink() < nexts.top().v->getLowlink() ) {
								nexts.top().v->setLowlink((**nexts.top().children).getLowlink());
							}
							nexts.top().children++;
						}
						else {
							nexts.top().children++;
						}
					}
					else {
						if (nexts.top().inpos == nexts.top().v->getLowlink()) {
							nexts.top().v->stat = Vertex::SCCFin;
							if (!unfinished.empty()) {
								SCC++;
								for (Vertex* finiv: unfinished) {
									finiv->stat = Vertex::SCCFin;
								}
								unfinished.clear();
							}
						}
						else {
							unfinished.push_back(nexts.top().v);
						}
						nexts.pop();
					}
				}
			}
		}
		
		
		std::unordered_map<Vertex*, std::unordered_set<Vertex*>> univertex;
		
		for (Vertex& v: vertexList) {
			for (Vertex* u: v.succIt()) {
				univertex[&v].insert(u);
			}
		}
		
		long unsigned int bidirection=0 ;
		for (std::pair<Vertex* const, std::unordered_set<Vertex*> > pair : univertex) {
			Vertex* v = pair.first;
			for (Vertex* w: pair.second) {
				if (univertex[w].count(v)>0) {
					bidirection++;
				}
				
			}
		}
		
		BE = 1.0* bidirection/m;
		
		double ojin=0;
		double ojout=0;
		double okin=0;
		double okout=0;
		double suminin=0;
		double suminout=0;
		double sumoutin=0;
		double sumoutout=0;
		
		for (Vertex& v: vertexList) {
			long unsigned int in = v.preIt().end()-v.preIt().begin();
			long unsigned int out = v.succIt().end()-v.succIt().begin();
			for (Vertex* v2: v.succIt()) {
				long unsigned int in2 = v2->preIt().end()-v2->preIt().begin();
				long unsigned int out2 = v2->succIt().end()-v2->succIt().begin();
				
				suminin += (in-ajin)*(in2-akin);
				suminout += (in-ajin)*(out2-akout);
				sumoutin += (out-ajout)*(in2-akin);
				sumoutout += (out-ajout)*(out2-akout);
				
				ojin += std::pow(in-ajin,2);
				ojout += std::pow(out-ajout,2);
				okin += std::pow(in2-akin,2);
				okout += std::pow(out2-akout,2);
			}
		}
		
		rinin = (suminin/m)/(std::pow(ojin/m,0.5)*std::pow(okin/m,0.5));
		rinout = (suminout/m)/(std::pow(ojin/m,0.5)*std::pow(okout/m,0.5));
		routin = (sumoutin/m)/(std::pow(ojout/m,0.5)*std::pow(okin/m,0.5));
		routout = (sumoutout/m)/(std::pow(ojout/m,0.5)*std::pow(okout/m,0.5));
		
		double hsum = 0;
		
		for (Vertex& v: vertexList) {
			long unsigned int out = v.succIt().end()-v.succIt().begin();
			for (Vertex* v2: v.succIt()) {
				long unsigned int in = v2->preIt().end()-v2->preIt().begin();
				
				hsum += 1.0/out+1.0/in- 2.0/(std::pow(out*in,0.5));
			}
		}
		
		H = hsum/(n-2.0*std::pow(n-1,0.5));
		
		printdebug("Stats Directed calculated");
	}
	//Superbubble properties
	
	//reset paths
	for (Vertex& v: vertexList) {
		v.setPaths(0);
	}
	//count edges and vertices
	long unsigned int pos = 0;
	long unsigned int edges = 0;
	//stack of found bubbles
	std::stack<long unsigned int*> bubs;
	
	long unsigned int maxVS=0;
	long unsigned int maxES=0;
	long unsigned int deepC=0;
	long unsigned int S=0 ;
	long unsigned int ms=0 ;
	long unsigned int C=0 ;
	long unsigned int CS=0;
	long unsigned int localCS=0;
	long unsigned int P=0;
	long unsigned int vssum=0 ;
	long unsigned int essum=0 ;
	long unsigned int APsum=0 ;
	double SDsum=0 ;
	
	bool overflow = false;
	for (orderElement o: orders) {
		Vertex* start = o.start;
		bool first = o.cut;
		while (true) {
			edges += start->succIt().end()-start->succIt().begin();
			if (start-> entrance && !(start == o.end && first)) {
				long unsigned int bubblsize = pos- bubs.top()[0]+1;
				long unsigned int bubbledgesize = edges- bubs.top()[1];
				long unsigned int bubblepaths = 0;
				if (!overflow) {
					bubblepaths = start->getPaths()/bubs.top()[2];
					if (P < bubblepaths) {
						P = bubblepaths;
					}
				}
				
				if (maxVS < bubblsize) {
					maxVS = bubblsize;
				}
				if (maxES < bubbledgesize) {
					maxES = bubbledgesize;
				}
				
				if (bubblsize== 2) {
					ms++;
				}
				else {
					APsum += bubblepaths;
					SDsum += 2.0*bubbledgesize/(bubblsize*(bubblsize-1));
				}
				delete bubs.top();
				bubs.pop();
				if (bubs.empty()) {
					C++;
					vssum += bubblsize;
					essum += bubbledgesize;
					if (localCS> CS) {
						CS = localCS;
					}
					if (start-> exit) {
						vssum--;
					}
				}
			}
			if (start-> exit && !(start == o.end && !first)) {
				if (bubs.empty()) {
					start->setPaths(1);
					localCS = 0;
				}
				localCS++;
				long unsigned int* bub = new long unsigned int[3];
				bub[0] = pos;
				bub[1] = edges;
				bub[2] = start->getPaths();
				bubs.push(bub);
				S++;
				if (deepC < bubs.size()) {
					deepC = bubs.size();
				}
			}
			if (!bubs.empty()) {
				if (!overflow ) {
					for (Vertex* v: start->preIt() ) {
						if (v->getPaths()> ( start->getPaths() + v->getPaths() ) ) {
							overflow = true;
						}
						v->setPaths( start->getPaths() + v->getPaths() );
					}
				}
				pos++;
			}
			
			if (start == o.end) {
				if (first) {
					first = false;
					start->setPaths(0);
				}
				else {
					break;
				}
			}
			
			start = start->getNext();
		}
	}
	
	long unsigned int nonmini = S-ms;
	if (nonmini == 0) {
		nonmini = 1;
	}
	
	double vs = 1.0*vssum/n;
	double es = 1.0*essum/n;
	double AP = 1.0*APsum/(nonmini);
	double SD = SDsum/nonmini;
	
	
	//reset paths
	for (Vertex& v: vertexList) {
		v.setPaths(0);
	}
	pos = 0;
	long unsigned int pl = 0;
	long unsigned int aplSum = 0;
	for (orderElement o: orders) {
		Vertex* start = o.start;
		bool first = o.cut;
		while (true) {
			if (!bubs.empty()) {
				for (Vertex* v: start->succIt() ) {
					if (v->getPaths()+1 > start->getPaths()) {
						start->setPaths( 1 + v->getPaths() );
					}
				}
				pos++;
			}
			if (start-> entrance && !(start == o.end && first)) {
				long unsigned int bubblepaths = start->getPaths()-bubs.top()[0];
				long unsigned int bubblsize = pos- bubs.top()[1]+1;
				if (pl < bubblepaths) {
					pl = bubblepaths;
				}
				if (bubblsize != 2) {
					aplSum += bubblepaths;
				}
				delete bubs.top();
				bubs.pop();
			}
			if (start-> exit && !(start == o.end && !first)) {
				if (bubs.empty()) {
					start->setPaths(0);
				}
				long unsigned int* bub = new long unsigned int[2];
				bub[0] = start->getPaths();
				bub[1] = pos;
				bubs.push(bub);
			}
			if (start == o.end) {
				if (first) {
					first = false;
				}
				else {
					break;
				}
			}
			
			start = start->getNext();
		}
	}
	double apl = 1.0*aplSum/nonmini;
	printdebug("Stats Superbubble calculated");
	
	
	std::ostream* output = &std::cout;
	if (statspath != "-") {
		output = new std::ofstream(statspath);
	}
	
	if (undirected) {
		*output << "N & M & ME & deg & GD & CC & R & SS \\\\\n";
		*output << n << " & ";
		*output << m << " & ";
		*output << ME << " & ";
		*output << deg << " & ";
		*output << GD << " & ";
		*output << CC << " & ";
		*output << r << " & ";
		*output << ss << " \\\\\n";

	}
	if (directed) {
		*output << " $deg_\\leftarrow$ & $deg_\\rightarrow$ & SCC & BE & $R_{\\leftarrow\\leftarrow}$ & $R_{\\leftarrow\\rightarrow}$ & $R_{\\rightarrow\\leftarrow}$ & $R_{\\rightarrow\\rightarrow}$ & H \\\\\n";
		*output << inDeg << " & ";
		*output << outDeg << " & ";
		*output << SCC << " & ";
		*output << BE << " & ";
		*output << rinin << " & ";
		*output << rinout << " & ";
		*output << routin << " & ";
		*output << routout << " & ";
		*output << H << " \\\\\n";
	}
	*output << "S & VS & ES & MS & mVS & mES & C & CS & depth & P & PL & aP & aPL & SD  \\\\\n";
	*output << S << " & ";
	*output << vs << " & ";
	*output << es << " & ";
	*output << ms << " & ";
	*output << maxVS << " & ";
	*output << maxES << " & ";
	*output << C << " & ";
	*output << CS << " & ";
	*output << deepC << " & ";
	if (!overflow) {
		*output << P << " & ";
	}
	else {
		*output << "$\\infty$ & ";
	}
	*output << pl << " & ";
	if (!overflow) {
		*output << AP << " & ";
	}
	else {
		*output << "$\\infty$ & ";
	}
	*output << apl << " & ";
	*output << SD << " \\\\";
	*output << "\n";
	
	if (statspath != "-") {
		(*((std::ofstream*)output)).close();
		delete output;
	}
}

struct supertree {
	std::vector<supertree*> childreen;
	Vertex* entrance;
	Vertex* exit;
};

void deletetree (supertree* t ) {
	if (! t->childreen.empty()) {
		for(long unsigned int i = 0; i < t->childreen.size(); i++) {
			deletetree(t->childreen[i]);
		}
	}
	delete t;
}

void outputTree(supertree* t, std::ostream& treefile) {
	if (! t->childreen.empty()) {
		treefile << "(";
		for(long unsigned int i = 0; i < t->childreen.size(); i++) {
			outputTree(t->childreen[i], treefile);
			if (i != t->childreen.size()-1) {
				treefile << ",";
			}
		}
		treefile << ")";
	}
	treefile << "<" << t->entrance->getID();
	treefile << "," << t->exit->getID();
	treefile << ">";
}

void Config::writeTree() {
	if (! trees) {
		return;
	}
	std::stack<supertree*> trees;
	std::vector<supertree*> alltrees;
	for (orderElement o: orders) {
		Vertex* start = o.start;
		bool first = o.cut;
		while (true) {
			if (start-> entrance && !(start == o.end && first)) {
				trees.top()->entrance = start;
				if (trees.size()==1) {
					alltrees.push_back(trees.top());
				}
				trees.pop();
			}
			if (start-> exit && !(start == o.end && !first)) {
				supertree* t = new supertree;
				t->exit = start;
				if (! trees.empty()) {
					trees.top()->childreen.push_back(t);
				}
				trees.push(t);
			}
			
			if (start == o.end) {
				if (first) {
					first = false;
				}
				else {
					break;
				}
			}
			start = start->getNext();
		}
	}
	
	
	std::ostream* output = &std::cout;
	if (treespath != "-") {
		output = new std::ofstream(treespath);
	}
	for (supertree* t: alltrees) {
		if (!t->childreen.empty()) {
			outputTree(t, *output);
			*output << "\n";
			deletetree(t);
		}
	}
	if (treespath != "-") {
		(*((std::ofstream*)output)).close();
		delete output;
	}
	printdebug("Tree written");
}

void writeHelp() {
	std::cerr <<  "usage: clsd path [options]\n\n";
	std::cerr <<  "A simple and fast tool to detect superbubbles.\n";
	std::cerr <<  "Needed argument:\n\tpath\t\t\tThe path to the input file. As a edge list.\n\n";
	std::cerr <<  "options:\n";
	std::cerr <<  "\t-h\t\t\tPrint this help and exit.\n";
	std::cerr <<  "\t-v\t\t\tPrint debug output.\n";
	std::cerr <<  "\t-n\t\t\tWrite no superbubble output. Make only sense when other output is activated.\n";
	std::cerr <<  "\t-o outpath\t\tWrite superbubble to *outpath*. When outpath='-' then write to stdout.\n";
	std::cerr <<  "\t-p outpath\t\tWrite the order with superbubbles as paranteses to *outpath*. When outpath='-' then write to stdout.\n";
	std::cerr <<  "\t-s outpath\t\tWrite the statistics to *outpath*. When outpath='-' then write to stdout.\n";
	std::cerr <<  "\t\t-u\t\tWrite the undirected statistics.\n";
	std::cerr <<  "\t\t-d\t\tWrite the directed statistics.\n";
	std::cerr <<  "\t-t outpath\t\tWrite superbubble trees to *outpath*. When outpath='-' then write to stdout.\n";
	}


void Config::parseParameter(int argc, char* argv[]) {
	order = false;
	stats = false;
	trees = false;
	noout = false;
	debug = false;
	undirected = false;
	directed = false;
	outpath = "-";
	if (argc <2 || std::string(argv[1]).rfind("-", 0) == 0) {
		writeHelp();
		std::cerr << "\n\nError: Give at least a path as input file!\n\n";
		exit(1);
	}
	else {
		path = argv[1] ;
	}
	std::vector<std::string> args(argv, argv+argc);
	for (size_t i = 2; i < args.size(); ++i) {
		if (args[i] == "-p") {
			order = true;
			orderpath = args[++i];
		}
		else if (args[i] == "-s") {
			stats = true;
			statspath = args[++i];
		}
		else if (args[i] == "-t") {
			trees = true;
			treespath = args[++i];
		}
		else if (args[i] == "-o") {
			outpath = args[++i];
		}
		else if (args[i] == "-n") {
			noout = true;
		}
		else if (args[i] == "-u") {
			undirected = true;
		}
		else if (args[i] == "-d") {
			directed = true;
		}
		else if (args[i] == "-h") {
			writeHelp();
			exit(0);
		}
		else if (args[i] == "-v") {
			runtime = std::clock();
			debug = true;		}
		else {
			writeHelp();
			std::cerr << "\n\nError: Unknonwn parameter "  << args[i] << "\n";
			exit(1);
		}
	}
}

std::string Config::getPath() {
	return path;
}

void Config::printdebug(std::string message) {
	if (!debug) {
		return;
	}
	std::cout << (1.0 * (std::clock()-runtime) / CLOCKS_PER_SEC) << "\t" << message << "\n";
	runtime = std::clock();
	
}



