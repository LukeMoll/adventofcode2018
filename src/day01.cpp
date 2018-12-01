#include "aocutils.hpp"
#include <vector>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

int part1(vector<string>, list<int>&);
bool part2(int, list<int>, int&);

int main(void) {
    vector<string> lines = AOC::getLines(01);
    cout << "Day 01: Chronal Calibration" << endl;
    
    list<int> p2_freqs = {};
    int p1_freq = part1(lines, p2_freqs);
    int p2_freq;

    cout << "(Part 1) Frequency after one iteration: " << p1_freq << endl;
    if(part2(p1_freq, p2_freqs, p2_freq)) {
        cout << "(Part 2) First duplicate frequency: " << p2_freq << endl;
    }
    else {
        cout << "(Part 2) no duplicate found." << endl;
    }
    return 0;
}

int part1(vector<string> lines, list<int>& p2_freqs) {
    int p1_freq = 0;
    for(string line : lines) {
        p2_freqs.push_back(p1_freq);
        if(line.length() < 2) {continue;}

        if(line[0]  == '+') {
            p1_freq += atoi(line.substr(1).c_str());
        }
        else {
            p1_freq -= atoi(line.substr(1).c_str());
        }
    }
    return p1_freq;
}

bool part2(int p1_freq, list<int> p2_freqs, int& p2_freq) {
    bool p2_found = false;
    auto p2_freqs_ordered = p2_freqs;
    p2_freqs.sort();
    int minval = p2_freqs.front(), maxval = p2_freqs.back();

    for(int i=1; i <= (maxval - minval) / p1_freq; i++) {
        for(int freq : p2_freqs_ordered) { 
            // MUST do in order, in case there are two duplicates in the same iteration
            if(binary_search(p2_freqs.begin(), p2_freqs.end(), freq + i*p1_freq)) {
                p2_found = true;
                p2_freq = freq + i*p1_freq;
                break;
            }
        }
        if(p2_found) break;
    }

    return p2_found;
}