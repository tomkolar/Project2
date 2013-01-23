//*  is used to demonstate adjacentcy maxtrices and the
//*  dijkstra's shortest path alogrithm.
//*
//*  ******** WARNING *****WARNING ***** WARNING ************************
//*  *																    *
//*  *  This is not a fully implented class!                            *
//*  *  Due to time constraints, and the fact that this is              *
//*  *  written mostly for demonstration purposes, the following        *
//*  *  methods have not been implemented: Copy Constructor, =, ==, !=. *
//*  *																    *
//*  ******** WARNING *****WARNING ***** WARNING ************************
//*
//*  Use buildGraph(istream& infile) to create a graph from
//*  a text file.  See notes on buildGraph method for expected
//*  format of data in the file.
//*
//*  Use findShortestPath() to run dijkstra's algorithm and determine
//*  the shortest path from all nodes to all nodes in the graph.
//*
//*  Displaying Path:  You must call findShortestPath() before using
//*  either of the display options
//*
//*		- Use displayAll() to display all vertices in the graph and the
//*		  shortest path as determined by dijsktra's algorithm.
//*
//*		- Use display(int from, int to) to display the shortest path
//*       as determined by dijkstra's algorithm from a particular node
//*       to a particular node.
//*
//*  Author:  Tom Kolar
//*
//*  Created: 1-21-12
//*
//****************************************************
#include "Graph.h"
#include "StringUtilities.h"
#include <limits>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;


// Graph()
//  Purpose: Create a Graph
//	Preconditons:
//	Postconditions:
Graph::Graph() {
	startNode = NULL;
	endNode = NULL;
}

// ~Graph(void)
//  Purpose: Destructor for Graph Object
//  Preconditions:
//  Postcondtions:
Graph::~Graph(){

}

// buildGraph(istream& infile)
//   Purpose: Build a graph from an input file
//		- NO ERROR CHECKING FOR INCORRECT DATA FORMATS!
//		  The data in the input file is assumed to be in the
//        correct format as specified in the precondtions. This
//		  is done so as to make the code more readable. If this
//        code changes to production, error checking will need
//        to be implemented within the framework of the application.
//	 Precondtions:
//		File must follow predfined format as follows:
//		  - The first line tells the number of nodes, say n.
//		  - Following is a text description of each of the 1 through n
//          nodes, one description per line (max length of 50).
//		  - After that, each line consists of 3 ints representing an
//			edge.  If there is an edge from node 1 to node 2 with
//			a label of 10, the data is: 1 2 10.
//		  - A zero for the first integer signifies the end of the data
//          for that one graph.
//   Postcondtions:
//		- The object will be popultated with the graph data defined
//		  by the infile
void Graph::buildGraph(string& fileName) {

	ifstream inFile(fileName);
	string line;
	bool edgesInit = false;

	while(getline(inFile, line)) {
		vector<string> tokens;
		StringUtilities::split(line, ' ', tokens);

		// Add vertices
		if (tokens.at(0) == "V") 
			addVertex(tokens);
		// Add Edges
		else if (tokens.at(0) == "E") {
			// Initialize edges if first time encountered an edge
			if (!edgesInit) {
				numVertices = vertices.size();
				for (Vertex* vertex : vertices)
					edges[vertex->label] = vector<Edge*>();
				edgesInit = true;
			}
			addEdge(tokens);
		}
	}

}

void Graph::addVertex(vector<string>& tokens) {

	// Create vetex and intialize 
	Vertex* vertex;
	vertex->weight = INT_MIN;
	vertex->previous = NULL;

	// Populate from tokens
	vertex->label = tokens.at(1);
	if (tokens.size() > 2)
		if (tokens.at(2) == "START")
			startNode = vertex;
		else if (tokens.at(2) == "END")
			endNode = vertex;

	// Add to collections
	vertices.push_back(vertex);
	verticeMap[vertex->label] = vertex;;
}

void Graph::addEdge(vector<string>& tokens) {

	// Create Edge and populate from tokens
	Edge* edge;
	edge->label = tokens.at(1);
	edge->start = verticeMap.find(tokens.at(2))->second;
	edge->end = verticeMap.find(tokens.at(3))->second;
	edge->weight = atoi(tokens.at(4).c_str());

	// Add to edges collection
	vector<Edge*>& startEdges = edges.find((edge->start)->label)->second;
	startEdges.push_back(edge);
	vector<Edge*>& endEdges = edges.find((edge->start)->label)->second;
	endEdges.push_back(edge);
}

// findShortestPath()
//   Purpose: Use the dijkstra algorithm to find the shortest path
//			  for all the vertices in the graph
//	 Precondtions: Graph should be fully formed with appropriate costs
//			       in the cost array
//   Postcondtions:
//		- the shortestPath array will be populated with the data
//		  corresponding to the shortest path for each vertext in the
//		  graph
void Graph::findHighestWeightPath() {

	bool startFound = false;

	// Iterate through the vertices
	for (Vertex* vertex : vertices) {
		// Check to for start constraints
		if (isStartConstrained() && !startFound) {
			if (vertex->isStart) 
				startFound = true;
			else
				// Haven't found start yet, so no need to calculate weight
				continue;
		}

		// Check for depth 0 (no edges leading to this vertex)
		bool isDepthZero = true;
		vector<Edge*>& vertexEdges = edges.find(vertex->label)->second;
		for (Edge* edge : vertexEdges) {
			if (edge->end->label == vertex->label) {
				isDepthZero = false;
				break;
			}
		}
		if (isDepthZero) {
			vertex->weight = 0;
		}




	
	}
	// Intialize the shortestPath matrix
	for (int i=1; i <= numberOfVertices; i++) {
		for (int j=1; j <= numberOfVertices; j++) {
			shortestPath[i][j].dist = INT_MAX;
			shortestPath[i][j].visited = false;
			shortestPath[i][j].path = 0;
		}
	}

	// Iterate through the vertices
	for (int i=1; i <= numberOfVertices; i++) {
		// Intialize shortestPath to 0 for this vertex
		shortestPath[i][i].dist = 0;

		// Find the shortest paths for this vertex
		shortestPathHelper(i);
	}
}

void Graph::shortestPathHelper(int current){

	// Find the next adjacent vertex with the lowest cost
	int nextLowestCost = INT_MAX;
	int next = 0;
	for (int j = 1; j <= numberOfVertices; j++)
		if (!shortestPath[current][j].visited
			&& shortestPath[current][j].dist < nextLowestCost) {
				nextLowestCost = shortestPath[current][j].dist;
				next = j;
		}


	// Base Case - There are no other adjacent nodes to check
	if (nextLowestCost == INT_MAX)
		return;

	// Add next lowest to vertexSet
	shortestPath[current][next].visited = true;

	// Update costs for all adjacent vertices that have not been visited
	for (int j = 1; j <= numberOfVertices; j++)
		if (cost[next][j] != INT_MAX && !shortestPath[current][j].visited) {
			int costThroughNext = shortestPath[current][next].dist + cost[next][j];
			// Update cost if path is shorter through next node
			if (shortestPath[current][j].dist > costThroughNext){
				shortestPath[current][j].dist = costThroughNext;
				shortestPath[current][j].path = next;
			}
		}

	// Recursivley visit rest of edges
	shortestPathHelper(current);
}

// displayAll()
//   Purpose: Display the graph to cout
//	 Precondtions:
//   Postcondtions:
//		- cout will dispaly a text description of the graph
void Graph::displayAll() {
	// Write header
	cout << "Description            ";
	writeFromToHeader();

	// Iterate through vertices
	for (int i=1; i <= numberOfVertices; i++) {
		// Write vertex name
		cout << data[i] << endl;

		// Iterate through all ajdacent vertices
		for (int j=1; j <= numberOfVertices; j++) {
			// Don't write anything for same vertex
			if (i == j)
				continue;

			// Write line header spacing
			cout << "                       ";

			// Write the from to details
			writeFromToDetails(i, j);
		}
	}

	cout << endl;
}

void Graph::writeFromToHeader(){
	cout << "From node    To node    Dijkstra's  Path"
		 << endl;
}

void Graph::writeFromToDetails(int from, int to) {
	// Write from node and to node
	cout << "    " << from << "          " << to;

	// Write shortest distance and path
	cout << "        ";
	if (shortestPath[from][to].dist == INT_MAX)
		cout << "---";
	else {
		cout << shortestPath[from][to].dist
				<< "          ";
		writeShortestPath(from, to);
	}

	// End the line
	cout << endl;
}

void Graph::writeShortestPath(int from, int to) {
	//Base case - past beginning node
	if (to == 0)
		return;

	// Walk the path backwards
	writeShortestPath(from, shortestPath[from][to].path);
	cout << to << " ";
}

// display(int from, int to)
//   Purpose: Display the graph to cout
//	 Precondtions: 0 < to, from <= numberOfVertices
//   Postcondtions:
//		- cout will dispaly a text description of the graph
void Graph::display(int from, int to)  {
	// Check if within range of number of vertices
	if (from <= 0 || from > numberOfVertices
		|| to <= 0 || to > numberOfVertices){
		cout << "Vertext not in graph" << endl << endl;
		return;
	}

	// Write the path
	writeFromToHeader();
	writeFromToDetails(from, to);
	if (shortestPath[from][to].dist != INT_MAX)
		writeShortestPathVertices(from, to);

	cout << endl;
}

void Graph::writeShortestPathVertices(int from, int to){
	//Base case - past beginning node
	if (to == 0)
		return;

	// Walk the path backwards
	writeShortestPathVertices(from, shortestPath[from][to].path);
	cout << data[to] << endl;
}

