#include "aocutils.hpp"
#include <cstdio>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

size_t play(const size_t num_players, const size_t num_marbles);
list<size_t>::iterator circularAdvance(list<size_t> &l, list<size_t>::iterator it, short offset);

int main(void) {
    cout << "Day 09 - Marble Mania" << endl;
    auto input = AOC::getLines(9);
    size_t players, max_marble;
    if(sscanf(input.front().c_str(), "%lu players; last marble is worth %lu points", &players, &max_marble) == 2) {
        cout << "(Part 1) Winning score: " << play(players, max_marble) << endl;
        cout << "(Part 2) Winning score: " << play(players, max_marble*100) << endl;
        return 0;   
    }
    else {
        cout << "Error reading puzzle input" << endl;
        return 1;
    }
}

size_t play(const size_t num_players, const size_t max_marble) {
    vector<size_t> scores (num_players);
    list<size_t> circle = {0};
    auto it = circle.begin();
    for(size_t marble=1; marble <= max_marble; marble++) { // the second player (index 1) in our list goes first
        if(marble % 23 == 0) {
            // something entirely different happens
            it = circularAdvance(circle, it, -7);
            scores[marble % num_players] += marble + *it;
            it = circle.erase(it); // points to element clockwise of current
            if(it == circle.end()) {it = circle.begin();}
        }
        else if(circle.size() < 2) {
            it++;
            it = circle.insert(it, marble);
        }
        else {
            it = circularAdvance(circle, it, 2);
            it = circle.insert(it, marble);
        }
    }
    sort(scores.begin(), scores.end());
    return scores.back();
}

list<size_t>::iterator circularAdvance(list<size_t> &l, list<size_t>::iterator it, short offset) {
    short direction = offset < 0 ? -1 : 1;
    while(offset != 0) {
        if(direction > 0) {
            if(it == l.end())   {it = next(l.begin());}
            else                {it = next(it);}
        }
        else {
            if(it == l.begin()) {it = prev(l.end());}
            else                {it = prev(it);}}
        offset -= direction;
    }
    return it;
}