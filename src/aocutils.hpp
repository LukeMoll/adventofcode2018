#ifndef AOC_UTILS_H
#define AOC_UTILS_H
	
	#include <fstream>
	#include <vector>
	#include <string>

	namespace AOC {
		std::ifstream getInputStream(int day);
		std::vector<std::string> getLines(int day);
	}


#endif