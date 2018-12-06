#include "aocutils.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>

using namespace std;

class Coord {
    public:
        int x, y;
        Coord(int x, int y) : x(x), y(y) {}
};

int manhattan(Coord, Coord);
int owns(Coord, vector<Coord>&);
void boundingBox(vector<Coord>&, int&, int&, int&, int&);
bool isInRegion(Coord, vector<Coord>&, int);

int main(void) {
    cout << "Day 06 - Chronal Coordinates" << endl;
    
    vector<string> lines = AOC::getLines(6);
    vector<Coord> coords;
    transform(lines.begin(), lines.end(), back_inserter(coords), [](string &line){
        int x, y;
        sscanf(line.c_str(), "%d, %d", &x, &y);
        return Coord(x,y);
    });

    const int PART2_RADIUS = 10000;

    int minX, minY, maxX, maxY;
    boundingBox(coords, minX, minY, maxX, maxY);

    vector<int> p1_areas;
    p1_areas.assign(coords.size(), 0);

    int p2_area = 0;

    for(int y=minY-1; y<=maxY+1; y++) {
        for(int x=minX-1; x<=maxX+1; x++) {
            Coord c = Coord(x,y);
            if(isInRegion(c, coords, PART2_RADIUS)) {p2_area++;}

            int idx = owns(c, coords);
            if(idx >= 0) {
                // only non-shared areas are counted
                if(x==minX || x==maxX || y==minY || y==maxY) {
                    p1_areas.at(idx) = -1; // using -1 as infinity
                }
                else if(p1_areas.at(idx) >= 0) { 
                    // only increment if coord not infinite
                    p1_areas.at(idx)++;
                }
            }
        }
    }

    sort(p1_areas.begin(), p1_areas.end());
    cout << "(Part 1) Largest isolated area: " << p1_areas.back() << endl;
    cout << "(Part 2) Region size: " << p2_area << endl;
    return 0;
}

int manhattan(Coord p1, Coord p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

bool isInRegion(Coord toTest, vector<Coord> &allCoords, int manRadius) {
    int distance = 0;
    for(auto c : allCoords) {
        distance += manhattan(toTest, c);
    }
    return distance < manRadius;
}

int owns(Coord toTest, vector<Coord> &allCoords) {
    pair<int,int> lowest = pair<int,int>(-1,-1); // first is index, second is length
    bool tied = false;
    for(size_t i=0; i<allCoords.size(); i++) {
        auto nc = allCoords.at(i);
        int man = manhattan(toTest, nc);
        if(man == 0) {
            return i;
        }
        else if(man < lowest.second || lowest.second < 0) {
            tied = false;
            lowest = pair<int,int>(i, man);
        }
        else if (man == lowest.second) {
            tied = true;
        }
    }
    return tied ? -1: lowest.first;
}

void boundingBox(vector<Coord> &allCoords, int &minX, int &minY, int &maxX, int &maxY) {
    maxX = maxY = 0; // only handling +ve x,y apparently
    minX = minY = -1;
    for(auto c : allCoords) {
        if(c.x > maxX) {maxX = c.x;}
        if(c.y > maxY) {maxY = c.y;}
        if(c.x < minX || minX == -1) {minX = c.x;}
        if(c.y < minY || minY == -1) {minY = c.y;}
    }
}