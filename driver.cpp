/*
 * driver.cpp
 *
 *  Created on: Jan 23, 2013
 *      Author: tomkolar
 */
#include "WDAGraph.h"
#include "FastaFile.h"
#include <string>
#include <iostream>
using namespace std;

int main( int argc, char *argv[] ) {

/*	// Check that file names were entered as arguments
	if (argc < 3) {
		cout << "Invalid # of arguments\n";
		cout << "usage:  lcp fastaFile1 fastaFile2 <directory>\n";
		return -3;
	}
*/

	// Unconstrained Test Graph
	string graphFileName = "c:/Users/kolart/Documents/Genome540/Assignment2/example_graph.txt";

	WDAGraph* aGraph =  new WDAGraph(graphFileName);
	aGraph->findHighestWeightPath();

	cout << aGraph->resultString();

	// Constrained Test Graph
	graphFileName = "c:/Users/kolart/Documents/Genome540/Assignment2/example_constraint_graph.txt";

	aGraph = new WDAGraph(graphFileName);
	aGraph->findHighestWeightPath();

	cout << aGraph->resultString();

	// Create fasta graph file and runt it
	graphFileName = "c:/Users/kolart/Documents/Genome540/Assignment2/fasta_graph.txt";
	string weightFileName = "c:/Users/kolart/Documents/Genome540/Assignment2/weights.txt";
	string fastaFileName = "NC_003413.fna";
	string fastaFileDirectory = "c:/Users/kolart/Documents/Genome540/Assignment2/";

	FastaFile fastaFile(fastaFileDirectory, fastaFileName);
	fastaFile.buildGraphFile(graphFileName, weightFileName);

	aGraph = new WDAGraph(graphFileName);
	aGraph->findHighestWeightPath();

	cout << fastaFile.firstLineResultString();
	cout << aGraph->resultString();

}



