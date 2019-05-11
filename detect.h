#ifndef ___DETECT_H___
#define ___DETECT_H___

#include <vector>
#include "graph.h"

void detect(Vertex*, Vertex*, Config& );
void detect(Vertex*, Vertex*, long unsigned int, Config&);
void finish(Vertex*, Vertex*, bool );

#endif