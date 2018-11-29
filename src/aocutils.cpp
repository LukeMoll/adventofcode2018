#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <exception>

#include "aocutils.hpp"

using namespace std;

namespace AOC {

	ifstream getInputStream(int day) {
		char fn[17];
		snprintf(fn, 16, "input/day%02d.txt", day);
		ifstream is (fn);

		if(!is.is_open()) {
	        throw runtime_error("Could not open puzzle input file");
	    }

		return is;
	}	

	vector<string> getLines(int day) {
		vector<string> lines = {};
		ifstream puzzleInput = getInputStream(day);
		string line;
	    while(getline(puzzleInput, line)) {
	        lines.push_back(line);
	    }
	    return lines;
	}
		
}

