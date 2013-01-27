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

	// Check that file name was  entered as argument
	if (argc < 2) {
		cout << "Invalid # of arguments\n";
		cout << "usage:  hwp graphFileName\n";
		cout << "        hwp -fasta fastaFileName weightFile\n";
		return -1;
	}

	// Check for correct number of args for -fasta flag
	if (argv[1] == "-fasta" && argc < 4) {
		cout << "Invalid # of arguments\n";
		cout << "usage:  hwp graphFileName\n";
		cout << "        hwp -fasta fastaFileName weightFile\n";
		return -1;
	}

	string graphFileName;

	// Create graphFile if fasta file passed in
	if (argv[1] == "-fasta") {
		string fastaFileName = argv[2];
		string fastaFileDirectory = "./";
		string weightFileName = argv[3];
		graphFileName = fastaFileName + "graph.txt";

		FastaFile fastaFile(fastaFileDirectory, fastaFileName);
		fastaFile.buildGraphFile(graphFileName, weightFileName);

		// Print out the first line from the fasta file
		cout << fastaFile.firstLineResultString();
	}
	else {
		graphFileName = argv[1];
	}

	// Create the WDAGraph and find the highest weight path
	WDAGraph* aGraph =  new WDAGraph(graphFileName);
	aGraph->findHighestWeightPath();

	// Print out the result string for the highest weight path
	cout << aGraph->resultString();

}
