/*
 * driver.cpp
 *
 *  Created on: Jan 23, 2013
 *      Author: tomkolar
 */
#include "WDAGraph.h"
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

	string fileName = "/Users/tomkolar/documents/Genome540/Assignment2/test_graph.txt";

	WDAGraph aGraph = WDAGraph();
	aGraph.buildWDAGraph(fileName);
	aGraph.findHighestWeightPath();

	cout << aGraph.resultString();

}



