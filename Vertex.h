#ifndef VERTEX_H
#define VERTEX_H
#include <string>
using namespace std;
class Vertex
{
public:
	Vertex();
	virtual ~Vertex();

private:
	string label;  // name of vertex
	bool isStart;
	bool isEnd;
	int weight; // highest path weight to get to the vertex
	Vertex* previous; // previous vertex in path of highest weight

};
#endif // VERTEX_H
