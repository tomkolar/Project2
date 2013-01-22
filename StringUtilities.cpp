/*
 * StringUtilties.cpp
 *
 *  Created on: Jan 21, 2013
 *      Author: tomkolar
 */

#include "StringUtilities.h"
#include <string>
#include <sstream>
#include <vector>
using namespace std;

StringUtilities::StringUtilities() {
	// TODO Auto-generated constructor stub

}

StringUtilities::~StringUtilities() {
	// TODO Auto-generated destructor stub
}

vector<string>& StringUtilities::split(const string& s, char delim, vector<string>& elems) {
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
