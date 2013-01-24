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
//*  Use buildWDAGraph(istream& infile) to create a WDAGraph from
//*  a text file.  See notes on buildWDAGraph method for expected
//*  format of data in the file.
//*
//*  Use findShortestPath() to run dijkstra's algorithm and determine
//*  the shortest path from all nodes to all nodes in the WDAGraph.
//*
//*  Displaying Path:  You must call findShortestPath() before using
//*  either of the display options
//*
//*		- Use displayAll() to display all vertices in the WDAGraph and the
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
#include "WDAGraph.h"
#include "StringUtilities.h"
#include <limits>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cstddef>
#include <sstream>
using namespace std;


// WDAGraph()
//  Purpose: Create a WDAGraph
//	Preconditons:
//	Postconditions:
WDAGraph::WDAGraph() {
	startNode = NULL;
	endNode = NULL;
	highestWeightNode = NULL;
}

// ~WDAGraph(void)
//  Purpose: Destructor for WDAGraph Object
//  Preconditions:
//  Postcondtions:
WDAGraph::~WDAGraph(){

}

// buildWDAGraph(istream& infile)
//   Purpose: Build a WDAGraph from an input file
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
//          for that one WDAGraph.
//   Postcondtions:
//		- The object will be popultated with the WDAGraph data defined
//		  by the infile
void WDAGraph::buildWDAGraph(string& fileName) {

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

void WDAGraph::addVertex(vector<string>& tokens) {

	// Create vetex and intialize 
	Vertex* vertex =  new Vertex();
	vertex->weight = INT_MIN;
	vertex->edgeForHWPath = NULL;

	// Populate from tokens
	vertex->label = tokens.at(1);
	if (tokens.size() > 2) {
		if (tokens.at(2) == "START")
			startNode = vertex;
		else if (tokens.at(2) == "END")
			endNode = vertex;
	}

	// Add to collections
	vertices.push_back(vertex);
	verticeMap[vertex->label] = vertex;;
}

void WDAGraph::addEdge(vector<string>& tokens) {

	// Create Edge and populate from tokens
	Edge* edge = new Edge();
	edge->label = tokens.at(1);
	edge->start = verticeMap.find(tokens.at(2))->second;
	edge->end = verticeMap.find(tokens.at(3))->second;
	edge->weight = atoi(tokens.at(4).c_str());

	// Add to edges collection
	vector<Edge*>& startEdges = edges.find((edge->start)->label)->second;
	startEdges.push_back(edge);
	vector<Edge*>& endEdges = edges.find((edge->end)->label)->second;
	endEdges.push_back(edge);
}

// findShortestPath()
//   Purpose: Use the dijkstra algorithm to find the shortest path
//			  for all the vertices in the WDAGraph
//	 Precondtions: WDAGraph should be fully formed with appropriate costs
//			       in the cost array
//   Postcondtions:
//		- the shortestPath array will be populated with the data
//		  corresponding to the shortest path for each vertext in the
//		  WDAGraph
void WDAGraph::findHighestWeightPath() {

	bool startFound = false;

	// Iterate through the vertices
	for (Vertex* vertex : vertices) {
		// Check to for start constraints
		if (isStartConstrained() && !startFound) {
			if (vertex->label == startNode->label)
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
			continue;
		}

		// Find the path with the highest weight to this vertex
		vertex->weight = 0;
		vertexEdges = edges.find(vertex->label)->second;
		for (Edge* edge : vertexEdges) {
			// Find edges where this vertex is the end node
			if (edge->end->label == vertex->label) {
				// Calculate weight (parent node weight plus edge weight)
				int pathWeight = edge->start->weight + edge->weight;

				// If path weight bigger than any other found so far then
				// it becomes the new weight for the vertex
				if (pathWeight > vertex->weight) {
					vertex->weight = pathWeight;
					vertex->edgeForHWPath = edge;
				}
			}
		}

		// Check for end constraint
		if (isEndConstrained()) {
			if (vertex->label == endNode->label) {
				highestWeightNode = vertex;
				break;
			}
			// Continue looking for the end vertex as the highest weight path must end there
			continue;
		}

		// Set highestWeightNode
		if (highestWeightNode == NULL || vertex->weight > highestWeightNode->weight)
			highestWeightNode = vertex;

	}  // end vertices for loop
	
}

bool WDAGraph::isStartConstrained() {
	return startNode != NULL;
}

bool WDAGraph::isEndConstrained() {
	return endNode != NULL;
}

string WDAGraph::resultString() {
	stringstream ss;
	if (highestWeightNode == NULL)
		ss << "No Path Found!";
	else {
		ss
			<< "Score: " << highestWeightNode->weight << "\n"
			<< "Start: " << getPathStartNodeLabel() << "\n"
			<< "End: " << highestWeightNode->label << "\n"
			<< "Path: " << getPath() << "\n";
	}

	return ss.str();
}

string WDAGraph::getPathStartNodeLabel() {

	// Base Case
	if (highestWeightNode == NULL)
		return "";

	// Search for start node (previous is NULL)
	Vertex* aNode = highestWeightNode;
	while (true) {
		// Done if this is start node
		if (isStartConstrained()) {
			if (aNode->label == startNode->label)
				break;
		}
		else if (aNode->weight == 0)
			break;

		// Walk backwards until find the start node
		Edge* anEdge = aNode->edgeForHWPath;
		aNode = anEdge->start;
	}

	return aNode->label;
}

string WDAGraph::getPath() {

	// Base Case
	if (highestWeightNode == NULL)
		return "";

	stringstream ss;

	// Walk path backwards and build string
	Vertex* aNode = highestWeightNode;
	while (true) {
		// Check for null node
		if (aNode == NULL)
			break;

		// Add the label from the edge to the string stream
		Edge* anEdge = aNode->edgeForHWPath;
		if (anEdge == NULL)
			break;
		ss << anEdge->label;

		// Done if this is start node
		if (isStartConstrained()) {
			if (aNode->label == startNode->label)
				break;
		}
		else if (aNode->weight == 0)
			break;

		// Walk backwards one node
		aNode = anEdge->start;
	}

	// Return reverse of stringstream (since we built the string backwards)
	string reversePath = ss.str();
	return string(reversePath.rbegin(), reversePath.rend());

}
