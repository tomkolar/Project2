//****************************************************
//* WDAGraph.h
//*
//*  This is the header file for the WDAGraph class which
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
#ifndef WDAGraph_H
#define WDAGraph_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <climits>
using namespace std;

class WDAGraph {

	public:
		// WDAGraph()
		//  Purpose: Create a WDAGraph
		//	Preconditons:
		//	Postconditions:
		WDAGraph();
		WDAGraph(string& aGraphFileName, string& aWeightFileName);

		// ~WDAGraph(void)
		//  Purpose: Destructor for WDAGraph Object
		//  Preconditions:
		//  Postcondtions:
		~WDAGraph();

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

		// findShortestPath()
		//   Purpose: Use the dijkstra algorithm to find the shortest path
		//			  for all the vertices in the WDAGraph
		//	 Precondtions: WDAGraph should be fully formed with appropriate costs
		//			       in the cost array
		//   Postcondtions:
		//		- the shortestPath array will be populated with the data
		//		  corresponding to the shortest path for each vertext in the
		//		  WDAGraph
		void findHighestWeightPath();

		string resultString();

private:

		struct Edge;

		struct Vertex {
			string label;  // name of vertex
			double weight; // highest path weight to get to the vertex
			Edge* edgeForHWPath; // edge that was used for the highest weight path
		};

		struct Edge {
			string label;
			double weight;
			Vertex* start;
			Vertex* end;
		};

		string graphFileName;
		string weightFileName;
		vector<Vertex*> vertices; // data for WDAGraph vertices
		map<string, vector<Edge*>> edges; // Collection of edges
		int numVertices; // number of vertices in the WDAGraph
		map<string, Vertex*> verticeMap;
		Vertex* startNode;
		Vertex* endNode;
		Vertex* highestWeightNode;
		map<string, double> edgeWeights;
		map<string, int> edgeFrequencies;

		void buildGraph();
		void buildWeightMap();
		bool isStartConstrained();
		bool isEndConstrained();
		bool weightsAreSpecifiedExternally();
		void addVertex(vector<string>& tokens);
		void addEdge(vector<string>& tokens);
		string getEdgeWeights();
		string getEdgeFrequencies();
		string getPathStartNodeLabel();
		string getPath();


};
#endif
