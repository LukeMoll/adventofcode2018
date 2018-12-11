#include "aocutils.hpp"
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <tuple>

using namespace std;

typedef pair<int,int> Coord;
struct CoordHash {
    size_t operator()(Coord const& pair) const {
        return pair.first * 301 + pair.second; // works for 300x300 I guess?
    }
};
typedef unordered_map<Coord, int, CoordHash> PowerGrid;

ostream& operator<<(ostream &strm, const Coord &c) {
    return strm << "(" << c.first << ", " << c.second << ")";
}

void assertPower(unsigned short x, unsigned short y, unsigned serial, int expected);
int powerLevel(unsigned short x, unsigned short y, unsigned serial);
PowerGrid makeGrid(unsigned serial);
PowerGrid reduceGridNxN(PowerGrid &grid, unsigned short reduc);
PowerGrid reduceIncremental(PowerGrid &grid, PowerGrid reduced, const unsigned short reduc);
pair<Coord, int> getLargestCoord(PowerGrid &grid);
bool shouldAbandon(const unsigned short reduc, int currentTotal, int maxTotal);
void printGrid(PowerGrid &grid, unsigned short xmin, unsigned short xmax, unsigned short ymin, unsigned short ymax);


int main(void) {
    cout << "Day 11 - Chronal Charge" << endl;

    // const unsigned serial = 18; // TODO actually use day11.txt
    unsigned serial;
    auto input = AOC::getLines(11);
    sscanf(input.front().c_str(), "%u", &serial);

    PowerGrid grid = makeGrid(serial);
    PowerGrid reduced = reduceGridNxN(grid, 3);
    auto l1 = getLargestCoord(reduced);
    cout << "(Part 1) Largest 3x3 has top-left at " << l1.first << " with total power " << l1.second << endl;

    // results tend to be within 30x30
    vector<tuple<Coord,unsigned short, int>> results;
    results.reserve(300);
    int maxTotal = 0;
    for(unsigned short N=1; N<300; N++) { // let's be honest, the answer won't be N=300
        reduced = reduceIncremental(grid, reduced, N);
        auto l2 = getLargestCoord(reduced);
        results.push_back(make_tuple(l2.first, N, l2.second));
        cout << N << "x" << N << ":\t " << l2.first << " with " << l2.second << endl;
        if(l2.second > maxTotal) {maxTotal = l2.second;}
        else if(shouldAbandon(N, l2.second, maxTotal)) {
            cout << "Abandoning - best solution already found!" << endl;
            break;
        }
    }

    return 0;
}

void assertPower(unsigned short x, unsigned short y, unsigned serial, int expected) {
    if(powerLevel(x,y,serial) == expected) {
        cout << "[ok]\tP(" << x << "," << y << "," << serial << ") == " << expected << endl;
    }
    else {
        cout << "[error]\tP" << x << "," << y << "," << serial << ") == " 
             << powerLevel(x,y,serial) << ", expected " << expected << endl;
    }
}

int powerLevel(unsigned short x, unsigned short y, unsigned serial) {
    unsigned short rackID = x + 10;
    int powerLevel = ((rackID * y) +  serial) * rackID;
    powerLevel = (powerLevel % 1000) / 100;
    return powerLevel - 5;
}

PowerGrid makeGrid(unsigned serial) {
    PowerGrid grid;
    for(int x=1; x <= 300; x++) {
        for(int y=1; y <= 300; y++) {
            grid.insert({{x,y}, powerLevel(x,y,serial)});
        }
    }
    return grid;
}

PowerGrid reduceGridNxN(PowerGrid &grid, const unsigned short reduc) {
    PowerGrid reduced;
    for(int x=1; x<=300 - reduc; x++) {
        for(int y=1; y<=300 - reduc; y++) {
            int cellTotal = 0;
            for(int i=0; i<reduc; i++) {
                for(int j=0; j<reduc; j++) {
                    cellTotal += grid[{x+i,y+j}];
                }
            }
            reduced.insert({{x,y}, cellTotal});
        }  
    }
    return reduced;
}

// this is a HUGE improvement -- but we can go better
PowerGrid reduceIncremental(PowerGrid &grid, PowerGrid reduced, const unsigned short reduc) {
    // *** this block grows the fastest / is the slowest part on larger inputs
    // outer: (300-reduc)^2 * inner
    for(int x=1; x<=300 - reduc; x++) {
        for(int y=1; y<=300 - reduc; y++) {
            // inner: linear in `reduc`
            int increase = 0;
            Coord c = {0,y+reduc};
            for(int i=0; i<= reduc; i++) {
                c.first = x+i;
                increase += grid[c];
            }
            c.first = x+reduc;
            for(int j=0; j< reduc; j++) {
                c.second = y+j;
                increase += grid[c];
            }
            reduced[{x,y}] += increase;
        }
    }
    // ***
    Coord c = {0, 301-reduc};
    for(int x=1; x <= 301 - reduc; x++) {
        c.first = x;
        reduced.erase(c);
    }
    c.first = 301-reduc;
    for(int y=1; y <= 301 - reduc; y++) {
        c.second = y;
        reduced.erase(c);
    }
    return reduced;
}

bool shouldAbandon(const unsigned short reduc, int currentTotal, int maxTotal) {
    return (450000 - 3000 * reduc) + currentTotal < maxTotal;
}

pair<Coord, int> getLargestCoord(PowerGrid &grid) {
    return *max_element(grid.begin(), grid.end(), [](const pair<Coord, int> &p1, const pair<Coord, int> &p2) {
        return p1.second < p2.second;
    });
}

void printGrid(PowerGrid &grid, unsigned short xmin, unsigned short ymin, unsigned short xmax, unsigned short ymax) {
    cout << "\t";
    for(unsigned short x=xmin; x<=xmax; x++) {
        cout << x << "\t";
    }
    cout << endl;
    for(unsigned short y=ymin; y<=ymax; y++) {
        cout << y << "\t";
        for(unsigned short x=xmin; x<=xmax; x++) {
            cout << grid[{x,y}] << "\t";
        }
        cout << endl;
    }
}