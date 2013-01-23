#include "Vertex.h"


Vertex::Vertex(){
	isStart = false;
	isEnd = false;
	weight = INT_MIN; // highest path weight to get to the vertex
	previous = NULL; // previous vertex in path of highest weight
}


Vertex::~Vertex(){
}
