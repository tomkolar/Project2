/*
 * StringUtilties.h
 *
 *  Created on: Jan 21, 2013
 *      Author: tomkolar
 */

#ifndef STRINGUTILITIES_H_
#define STRINGUTILITIES_H_

#include <string>
#include <vector>
using namespace std;

class StringUtilities {
public:
	StringUtilities();
	virtual ~StringUtilities();

	static vector<string>& split(const string& s, char delim, vector<string>& elems);
};

#endif /* STRINGUTILITIES_H_ */
