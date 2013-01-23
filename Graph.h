//****************************************************
//* Graph.h
//*
//*  This is the header file for the graph class which
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
#ifndef Graph_H
#define Graph_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <climits>
using namespace std;

class Graph {

	public:
		// Graph()
		//  Purpose: Create a Graph
		//	Preconditons:
		//	Postconditions:
		Graph();

		// ~Graph(void)
		//  Purpose: Destructor for Graph Object
		//  Preconditions:
		//  Postcondtions:
		~Graph();

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
		void buildGraph(string& fileName);

		// findShortestPath()
		//   Purpose: Use the dijkstra algorithm to find the shortest path
		//			  for all the vertices in the graph
		//	 Precondtions: Graph should be fully formed with appropriate costs
		//			       in the cost array
		//   Postcondtions:
		//		- the shortestPath array will be populated with the data
		//		  corresponding to the shortest path for each vertext in the
		//		  graph
		void findHighestWeightPath();

		string resultString();

private:


		struct Vertex {
			string label;  // name of vertex
			int weight; // highest path weight to get to the vertex
			Edge* edgeForHWPath; // edge that was used for the highest weight path
		};

		struct Edge {
			string label;
			int weight;
			Vertex* start;
			Vertex* end;
		};


		vector<Vertex*> vertices; // data for graph vertices
		map<string, vector<Edge*>> edges; // Collection of edges
		int numVertices; // number of vertices in the graph
		map<string, Vertex*> verticeMap;
		Vertex* startNode;
		Vertex* endNode;
		Vertex* highestWeightNode;

		bool isStartConstrained();
		bool isEndConstrained();
		void addVertex(vector<string>& tokens);
		void addEdge(vector<string>& tokens);
		string getPathStartNodeLabel();
		string getPath();

};
#endif
