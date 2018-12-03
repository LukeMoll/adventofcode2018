#include <iostream>
#include "aocutils.hpp"
#include <string>
#include <cstdio>
#include <algorithm>
#include <unordered_set>

using namespace std;

class Point {
    public:
        const int x, y;
        Point(int x, int y) : x(x), y(y) {}
        Point leftMost(Point b) const  {
            return this->x < b.x ? *this : b;
        }
        Point rightMost(Point b) const {
            return this->x >= b.x ? *this : b;
        }
        Point topMost(Point b) const {
            return this->y < b.y ? *this : b;
        }
        Point bottomMost(Point b) const {
            return this->y >= b.y ? *this : b;
        }
        bool operator== (const Point &rhs) const {
            return this->x == rhs.x && this->y == rhs.y;
        }
};
std::ostream& operator<<(std::ostream &strm, const Point p) {
    return strm << "(" << p.x << "," << p.y << ")";
}
template<> struct std::hash<Point> {
    std::size_t operator()(const Point& p) const noexcept {
        return hash<int>{}(p.x) ^ (hash<int>{}(p.y) << 10);
    }
};

class Claim {
    public:
        const int id;
        const Point cornerTL, cornerBR;
        bool overlap(Claim b, unordered_set<Point>& overlaps) {
            bool doesOverlap = false;
            for(int x=this->cornerTL.x; x<=this->cornerBR.x; x++) {
                for(int y=this->cornerTL.y; y<=this->cornerBR.y; y++) {
                    Point p = Point(x,y);
                    if(b.containsPoint(p)) {
                        overlaps.insert(p);
                        doesOverlap = true;
                    }
                }
            }
            return doesOverlap;
        }
        bool containsPoint(Point p) {
            return  this->cornerTL.x <= p.x && p.x <= this->cornerBR.x &&
                    this->cornerTL.y <= p.y && p.y <= this->cornerBR.y;
        }
        static Claim fromLine(string s) {
            int id, x, y, w, h;
            sscanf(s.c_str(), "#%d @ %d,%d: %dx%d", &id, &x, &y, &w, &h);
            return Claim(id, Point(x,y), Point(x+w-1, y+h-1));
        }
        Claim(int id, Point TL, Point BR) : id(id), cornerTL(TL), cornerBR(BR) {}
};

int main(void) {
    cout << "Day 03 - No Matter How You Slice It" << endl;
    const auto lines = AOC::getLines(3);

    unordered_set<Point> overlaps;
    vector<Claim> claims;
    claims.reserve(lines.size());
    for(string line : lines) {
        Claim nC = Claim::fromLine(line);
        for(Claim c : claims) {
            nC.overlap(c, overlaps);
        }
        claims.push_back(nC);
    }
    cout << "(Part 1) " << overlaps.size() << endl;

    for(Claim c : claims) { // this is very bad and slow but shusb I'm depressed
        bool doesOverlap = false;
        for(Claim d : claims) {
            if(c.id == d.id) {continue;}
            // else
            if(c.overlap(d, overlaps)) { // we don't care about storing the result
                doesOverlap = true;
                break;
            }
        }
        if(!doesOverlap) {
            cout << "(Part 2) Claim #" << c.id << " does not overlap" << endl;
        }
    }

    return 0;
}