#include "aocutils.hpp"
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

bool isUnit(char, char);
bool react(string&);
bool stripUnits(string&, char);

int main(void) {
    const string input = "dabAcCaCBAcCcaDA"; // replace this with puzzle input -- file-based to follow

    string p1_input = input;
    while(react(p1_input)) {
    }
    cout << "(Part 1) " << input.length() << " units remain." << endl;
    
    // let's try to do this with as few copies in memory as possible
    string p2_input;
    for(char c = 'A'; c <= 'Z'; c++) {
        p2_input = input;
        if(stripUnits(p2_input, c)) {
            cout << " " << c << ": " << p2_input.length() << " (-" << input.length() - p2_input.length() << ")";
            while(react(p2_input)) {};
            cout << " -> " << p2_input.length() << " units" << endl;
        }
    }
    return 0;
}

bool react(string &input) {
    for(size_t i=1; i<input.length(); i++) {
        char prev = input.c_str()[i-1];
        if(isUnit(prev,input.c_str()[i])) {
            input = input.substr(0, i-1) + input.substr(i+1);
            return true; 
            // could continue to do the rest of the pass but this makes the loop easier
        }
    }
    return false;
}

bool stripUnits(string &input, char lowerUnit) {
    size_t start_size = input.length();
    size_t start_pos = 0;
    string toRemove = string(1, lowerUnit);
    while((start_pos = input.find(toRemove, start_pos)) != std::string::npos) {
        input.replace(start_pos, 1, "");
    }
    if(lowerUnit < 'a') {
        stripUnits(input, lowerUnit + 32);
    }
    return input.length() < start_size;
}

bool isUnit(char prev, char current) {
    return  (prev - current ==  32) ||
            (prev - current == -32);
}