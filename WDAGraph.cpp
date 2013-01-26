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
WDAGraph::WDAGraph(string& aGraphFileName, string& aWeightFileName) {

	// Initialize pointers
	startNode = NULL;
	endNode = NULL;
	highestWeightNode = NULL;

	//  Set file names
	graphFileName = aGraphFileName;
	weightFileName = aWeightFileName;
	
	if (weightsAreSpecifiedExternally())
		buildWeightMap();
	buildGraph();
}

// ~WDAGraph(void)
//  Purpose: Destructor for WDAGraph Object
//  Preconditions:
//  Postcondtions:
WDAGraph::~WDAGraph(){

}

void WDAGraph::buildWeightMap() {
	ifstream weightFile(weightFileName);
	string line;

	while(getline(weightFile, line)) {
		vector<string> tokens;
		StringUtilities::split(line, ' ', tokens);

		edgeWeights[tokens.at(0)] = atoi(tokens.at(1).c_str());
	}

	weightFile.close();
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
void WDAGraph::buildGraph() {

	ifstream graphFile(graphFileName);
	string line;

	while(getline(graphFile, line)) {
		vector<string> tokens;
		StringUtilities::split(line, ' ', tokens);

		// Add vertices
		if (tokens.at(0) == "V") 
			addVertex(tokens);
		// Add Edges
		else if (tokens.at(0) == "E") 
			addEdge(tokens);
	}

	graphFile.close();

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
	verticeMap[vertex->label] = vertex;

	// initialize edges entry for this vertex
	edges[vertex->label] = vector<Edge*>();

}

void WDAGraph::addEdge(vector<string>& tokens) {

	// Create Edge and populate from tokens
	Edge* edge = new Edge();
	edge->label = tokens.at(1);
	edge->start = verticeMap.find(tokens.at(2))->second;
	edge->end = verticeMap.find(tokens.at(3))->second;

	// Set weight from weights map if it exists, otherwise set from graph file
	if (weightsAreSpecifiedExternally())
		edge->weight = edgeWeights.find(edge->label)->second;
	else 
		edge->weight = atof(tokens.at(4).c_str());


	// Add to edges collection
	vector<Edge*>& startEdges = edges.find((edge->start)->label)->second;
	startEdges.push_back(edge);
	vector<Edge*>& endEdges = edges.find((edge->end)->label)->second;
	endEdges.push_back(edge);

	// Add to edgeWeights if not externally specified
	if (!weightsAreSpecifiedExternally())
		edgeWeights[edge->label] = edge->weight;

	// Increment edgeFrequencies
	map<string, int>::iterator iter = edgeFrequencies.find(edge->label);
	if (iter != edgeFrequencies.end()) 
		edgeFrequencies[edge->label] = iter->second++;
	else
		edgeFrequencies[edge->label] = 1;

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
		if (isStartConstrained()) {
			if (!startFound ) {
				if (vertex->label == startNode->label) {
					startFound = true;
					vertex->weight = 0;
					highestWeightNode = vertex;
				}
				else // start not found yet
					continue;
			}
		}
		else {
			// Start not constrained - so consider the trivial path of starting here
			vertex->weight = 0;
		}


		// Find the path with the highest weight to this vertex
		vector<Edge*> vertexEdges = edges.find(vertex->label)->second;
		for (Edge* edge : vertexEdges) {
			// Find edges where this vertex is the end node
			if (edge->end->label == vertex->label) {
				// If start constrained make sure edge start is from a valid path
				if  (isStartConstrained()) {
					if (edge->start->weight == INT_MIN)
						continue;
				}
				// Calculate weight (parent node weight plus edge weight)
				double pathWeight = edge->start->weight + edge->weight;

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
			// Is this the end node?
			if (vertex->label == endNode->label) {
				// Found end node, so set it as highest and exit vertex for loop
				highestWeightNode = vertex;
				break;
			}
			// Continue looking for the end vertex as the highest weight path must end there
			continue;
		}

		// Set highestWeightNode (for non end constrained paths)
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

bool WDAGraph::weightsAreSpecifiedExternally() {
	return !weightFileName.empty();
}

string WDAGraph::resultString() {
	stringstream ss;
	// Results header
	ss << "  <results type=\"part?\" file=\"" << graphFileName << "\">\n";

	// Edge Info (Weights and Histogram)
	ss << StringUtilities::xmlResult("edge_weights", getEdgeWeights());
	ss << StringUtilities::xmlResult("edge_histogram", getEdgeFrequencies());

	// Path Info
	if (highestWeightNode == NULL)
		ss << StringUtilities::xmlResult("path", "No Path Found!");
	else {
		ss
			<< StringUtilities::xmlResult("score",  highestWeightNode->weight, 2)
			<< StringUtilities::xmlResult("beginning_vertex",  getPathStartNodeLabel())
			<< StringUtilities::xmlResult("end_vertex", highestWeightNode->label)
			<< StringUtilities::xmlResult("path", getPath());
	}

	return ss.str();
}

string WDAGraph::getEdgeWeights() {
	stringstream ss;
	ss.precision(2);

	// Iterate throght edge weights map
	for (auto edgeWeight : edgeWeights) {
		ss << edgeWeight.first << "=" << edgeWeight.second << ", ";
	}

	// Return all but last 2 char (as there will be an extra ,<space> at end)
	string temp = ss.str();
	return temp.substr(0, temp.length() -2);
}

string WDAGraph::getEdgeFrequencies() {
	stringstream ss;

	// Iterate throght edge weights map
	for (auto edgeFrequency : edgeFrequencies) {
		ss << edgeFrequency.first << "=" << edgeFrequency.second << ", ";
	}

	// Return all but last 2 char (as there will be an extra ,<space> at end)
	string temp = ss.str();
	return temp.substr(0, temp.length() -2);
}

string WDAGraph::getPathStartNodeLabel() {

	// Base Case
	if (highestWeightNode == NULL)
		return "";

	// Search for start node (previous is NULL)
	Vertex* aNode = highestWeightNode;
	while (aNode->edgeForHWPath != NULL) {
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
	while (aNode->edgeForHWPath != NULL) {
		// Add the label from the edge to the string stream
		Edge* anEdge = aNode->edgeForHWPath;
		ss << anEdge->label;

		// Walk backwards one node
		aNode = anEdge->start;
	}

	// Return reverse of stringstream (since we built the string backwards)
	string reversePath = ss.str();
	return string(reversePath.rbegin(), reversePath.rend());

}
