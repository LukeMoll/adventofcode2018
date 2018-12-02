#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include "aocutils.hpp"

using namespace std;

void countLines(vector<string> lines, int &twoLetters, int &threeLetters);
map<char, int> toCharFreq(string line);
int countFreq(map<char, int> &cF, int number);
int countCharDiffs(string a, string b);
string sharedChars(string a, string b);

int main(void) {
    cout << "Day 02 - Inventory Management System" << endl;
    const auto &lines = AOC::getLines(2); 
    
    int twoLetters = 0, threeLetters = 0;
    countLines(lines, twoLetters, threeLetters);
    cout << "(Part 1) Checksum = " << twoLetters << " x " << threeLetters << " = " << twoLetters * threeLetters << endl;

    for(string a : lines) {
        for(string b : lines) {
            if(countCharDiffs(a,b) == 1) {
                // O(n²⨉w) makes JJ cry but for n=250 and w=26 it's not /too/ bad
                cout << "(Part 2) " << a << " and " << b << " differ by only one character" << endl;
                cout << "         Shared characters: " << sharedChars(a,b) << endl;
                return 0;
            }
        }
    }

    return 0;
}

void countLines(vector<string> lines, int &twoLetters, int &threeLetters) {
    map<char, int> cF;
    for (string line : lines) {
        cF = toCharFreq(line);
        twoLetters   += countFreq(cF, 2) > 0 ? 1 : 0; // hopefully compiler optimises
        threeLetters += countFreq(cF, 3) > 0 ? 1 : 0; // this to something sensible
    }
}

map<char, int> toCharFreq(string line) {
    map<char, int> charFreq;
    for(char &c : line) {
        if(charFreq.count(c) > 0) {
            charFreq[c]++;
        }
        else {
            charFreq[c] = 1;
        }
    }
    return charFreq;
}

int countFreq(map<char, int> &cF, int number) {
    int count = 0;
    for (map<char,int>::iterator it = cF.begin(); it != cF.end(); it++) {
        if (it->second == number) count++;
    }
    return count;
}

int countCharDiffs(string a, string b) {
    int count = 0;
    const int len = a.length() < b.length() ? a.length() : b.length();
    // just being safe...
    for(size_t i=0; i<len; i++) {
        if(a[i] != b[i]) count++;
    }
    return count;
}

string sharedChars(string a, string b) { // this method looks familiar
    stringstream ss;
    const int len = a.length() < b.length() ? a.length() : b.length();
    for(size_t i=0; i<len; i++) {
        if(a[i] == b[i]) ss << a[i];
    }
    return ss.str();
}