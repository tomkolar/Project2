/*
 * FastaFile.cpp
 *
 *	This is the cpp file for the FastaFile object. The FastaFile object
 *  is a utility object designed to read in a Fasta File and keep the 
 *  information for the file in memory.
 *
 *  ************* WARNING ***********************************
 *  *  There is no error handling in place for this object! *
 *  *  This means that if the file does not exist or is     *
 *  *  formatted incorrectly, you will get an error and     *
 *  *  will not be able to use this object.                 *
 *  *                                                       *
 *  *  If this code gets moved to a production setting      *
 *  *  appropriate error handling should be implemented!    *
 *  *********************************************************
 *
 * Typical use for the file would be to use the FastaFile(pathName, fileName)
 * constructor to create the object.  This will automatically open the
 * Fasta File specified by the pathName and fileName, and read its contents
 * storing them in the firstLine, and dnaSequence attributes.
 *
 * buildGraphFile(graphFileName, wieghtFileName) is a convenience method that
 * will create a sequence graph file for the dnaSequence.  See the method for
 * more details on what is created.
 *
 *  Created on: 1-10-13
 *	  Modified: 1-26-13
 *      Author: tomkolar
 */

#include "FastaFile.h"
#include "StringUtilities.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

// Constuctors
// ==============================================
FastaFile::FastaFile() {
}


FastaFile::FastaFile(string path, string name) {
        filePath = path;
        fileName = name;
        populate();
    }

// Destructor
// ==============================================
FastaFile::~FastaFile() {
}

// Public Methods
// =============================================

// buildGraphFile(string& graphFileName, string& weightFileName)
//  Purpose: 
//		Build a sequence graph file from this fasta file.  The graph defined
//		will essentially be a linked list with vertecies specified inbetween
//		each nucleotide in the dnaSequence and edges being the nucleotide
//		with a weight as specified in the weightFileName.
//
//		The file will be created such that all verticies are listed first
//		followed by all edges.
//
//		Vertex format: 
//
//			V <sequential number as identifier>
//
//		Edge format:
//
//			E <nucleotide> <start vertex id> <end vertex id> <weight>
//
//  Preconditions:
//		Fasta File has been read and dnaSequence has been populated
//  Postconditions:
//		File named aGraphFileName will be populated with the sequence graph 
//		associated with the dnaSequence from the fasta file.
void FastaFile::buildGraphFile(string& graphFileName, string& weightFileName) {

	// Build weight map from weight file
	map<char, double> edgeWeights;
	ifstream weightFile(weightFileName);
	string line;

	while(getline(weightFile, line)) {
		vector<string> tokens;
		StringUtilities::split(line, ' ', tokens);

		edgeWeights[tokens.at(0).at(0)] = atof(tokens.at(1).c_str());
	}

	weightFile.close();

	// Create the vertices and edges
	stringstream vertices;
	stringstream edges;

	int sequenceLength = dnaSequence.length();
	for (int i = 0; i < sequenceLength; i++) {
		// Add vertex info to vertices stream
		vertices << "V " << i << "\n";

		// Add edge info to edges stream
		char nucleotide = dnaSequence.at(i);
		edges
			<< "E "
			<< nucleotide << " " 
			<< i << " "
			<< i+1 << " "
			<< edgeWeights.at(nucleotide) << "\n";
	}
	// Add last vertex
	vertices << "V " << sequenceLength << "\n";

	// Write vertices and edges to file
	ofstream graphFile(graphFileName);
	graphFile << vertices.rdbuf();
	graphFile << edges.rdbuf();

	graphFile.close();
}

// string firstLineResultString()
//  Purpose:
//		Returns the string value of an XML element representing the first line of 
//		the Fasta file.
//
//		format:
//			<result type='first line' file='<<fileName>>' >
//				<<firstLine>>
//			</result>
//  Preconditions:
//		Fasta File has been read and firstLine has been populated
string FastaFile::firstLineResultString() {
	stringstream ss;

	ss << "    <result type='first line' file='" << fileName << "'>\n";
	ss << "      " << firstLine << "\n";
	ss << "    </result>\n";

	return ss.str();
}

// string baseCountsResultString()
//  Purpose:
//		Returns the string value of an XML element representing the base counts 
//		of the dnaSequence.
//
//		format:
//			<result type='nucleotide histogram' file='<<fileName>>' >
//				A=<<baseCountForA>>,C=<<baseCountForC>>,G=<<baseCountForG>>,
//				A=<<baseCountForT>>,N=<<countForOtherChars>>
//			</result>
//  Preconditions:
//		Fasta File has been read and dnaSequence has been populated
string FastaFile::baseCountsResultString() {
	stringstream ss;

	// Header
	ss <<  "    <result type='nucleotide histogram' file='" << fileName << "'>\n";

	// Base Counts
	int baseCounts[5] = {0};
	countBases(baseCounts);

	ss << "      ";
	ss << "A=" << baseCounts[0];
	ss << ",C=" << baseCounts[1];
	ss << ",G=" << baseCounts[2];
	ss << ",T=" << baseCounts[3];
	if (baseCounts[4] > 0)
		ss << ",N=" << baseCounts[4];

	ss << "\n";

	// Footer
	ss << "    </result>\n";

	return ss.str();
}

// Public Accessors
// =============================================
const int FastaFile::getSequenceLength() {
	return dnaSequence.length();
}

string& FastaFile::getFileName() {
    return fileName;
}

string& FastaFile::getDnaSequence() {
	return dnaSequence;
}

// Private Methods
// =============================================

// populate()
//  Purpose:
//		Reads in the Fasta File specified by filePath and fileName and populates
//		the object with its contents
//	Preconditions:
//		fileName and filePath have been set
//  Postconditions:
//		firstLine - populated with first line from file
//		dnaSequence - populated with dnaSequence from file
//		reverseComplement - populated with reverse complement of dnaSequence
void FastaFile::populate() {

    ifstream inputFile(filePath + fileName);
	stringstream ss;
	string line;

	getline(inputFile, firstLine);
		
	while(getline(inputFile, line)) {
		ss << line;
	}

	dnaSequence = ss.str();

	inputFile.close();

	createReverseComplement();
}

// createReverseComplment()
//  Purpose:
//		populates the reverseComplement attribute with the reverse comlpement
//		of the dnaSequence
//	Preconditions:
//		dnaSequence has been set
//  Postconditions:
//		reverseComplement - populated with reverse complement of dnaSequence
void FastaFile::createReverseComplement() {
    stringstream ss;

    // Append the complements in order
    for (string::size_type i = 0; i < dnaSequence.length(); i++) {
            ss << complement(dnaSequence[i]);
    }

    // Reverse the string and set it to the reverseComplement attribute
	string complement = ss.str();
	reverseComplement = string(complement.rbegin(), complement.rend());
}

// char complement(char aChar)
//  Purpose:  returns the dna complement of aChar
char FastaFile::complement(char aChar) {
    if (aChar == 'A')
        return 'T';

    if (aChar == 'T')
        return 'A';

    if (aChar == 'G')
        return 'C';

    if (aChar == 'C')
        return 'G';

    return aChar;
}

// countBases(int counts[])
//  Purpose:
//		populates the counts array with the counts for base occurrences
//		in dnaSequence.  The array is populated with the folllowing 
//		scheme:
//			counts[0] = counts for A
//			counts[1] = counts for C
//			counts[2] = counts for G
//			counts[3] = counts for T
//			counts[4] = counts for other characters encountered
void FastaFile::countBases(int counts[]) {
	for (string::size_type i=0; i < dnaSequence.length(); i++) {
		char currentChar = dnaSequence[i];
		if (currentChar == 'A')
			counts[0]++;
		else if (currentChar == 'C')
			counts[1]++;
		else if (currentChar == 'G')
			counts[2]++;
		else if (currentChar == 'T')
			counts[3]++;
		else
			counts[4]++;
	}

}

