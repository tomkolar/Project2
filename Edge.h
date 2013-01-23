#ifndef EDGE_H
#define EDGE_H

using namespace std;
class Edge
{
public:
	Edge();
	virtual ~Edge();

private:
	string label;
	int weight;
	Vertex* start;
	Vertex* end;

};
#endif // EDGE_H

