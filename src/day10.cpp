#include "aocutils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <regex>
#include <algorithm>

using namespace std;

const static regex line_regex ("position=<\\s*(-?\\d+),\\s*(-?\\d+)> velocity=<\\s*(-?\\d+),\\s*(-?\\d+)>");
class Point {
    public:
        const long initial_x, initial_y;
        long int x, y;
        short dx, dy;
        static Point parse(string line) {
            smatch match;
            if(!regex_match(line, match, line_regex)) {
                throw runtime_error("Line didn't match: " + line);
            }

            long int x, y;
            short dx, dy;
            x =  stoi(match[1].str());
            y =  stoi(match[2].str());
            dx = stoi(match[3].str());
            dy = stoi(match[4].str());
            return Point(x,y,dx,dy);
        }
        void move() {
            this->x += this->dx;
            this->y += this->dy;
        }
        void move(size_t times) {
            this->x += times * this->dx;
            this->y += times * this->dy;
        }
        void reset() {
            x = initial_x;
            y = initial_y;
        }
    private:
        Point(long int x, long int y, short dx, short dy) :
            initial_x(x), initial_y(y), x(x), y(y), dx(dx), dy(dy) 
            {}
};
ostream& operator<< (ostream& strm, Point &p) {
    return strm << "(" << p.x << "," << p.y << ")\t@ <" << p.dx << "," << p.dy << ">";
}

void boundingBox(vector<Point> &points, long int &minX, long int &minY, long int &maxX, long int &maxY);
size_t toIndex(unsigned long xrel, unsigned long yrel, long width);
string toString(vector<Point> &points);

int main(void) {
    cout << "Day 10 - The Stars Align" << endl;
    auto input = AOC::getLines(10);
    vector<Point> points;
    points.reserve(input.size());
    transform(input.begin(), input.end(), back_inserter(points), Point::parse);

    long int minX, maxX, minY, maxY;
    boundingBox(points, minX, minY, maxX, maxY);
    long int minArea = (maxX-minX) * (maxY-minY), area;
    size_t minTime;
    for(size_t i=0; i<50000; i++) {
        boundingBox(points, minX, minY, maxX, maxY);
        area = (maxX-minX) * (maxY-minY);
        if(area < minArea) {
            minArea = area;
            minTime = i;
        }
        for(Point &p : points) {p.move();} // have this last so i is "time waited"
    }
    cout << "(Part 1) Message is most likely to be:" << endl;
    for(Point &p : points) {p.reset(); p.move(minTime);}
    cout << toString(points) << endl;
    cout << "(Part 2) Smallest area found at t=" << minTime << endl;
    return 0;
}

void boundingBox(vector<Point> &points, long int &minX, long int &minY, long int &maxX, long int &maxY) {
    minX = numeric_limits<long int>::max();
    minY = numeric_limits<long int>::max();
    maxX = numeric_limits<long int>::min();
    maxY = numeric_limits<long int>::min();
    for(Point p : points) {
        if(p.x < minX) {minX = p.x;}
        if(p.y < minY) {minY = p.y;}
        if(p.x > maxX) {maxX = p.x;}
        if(p.y > maxY) {maxY = p.y;}
    }
}

size_t toIndex(unsigned long xrel, unsigned long yrel, long width) {
    return (width + 1) * yrel + xrel;
}

string toString(vector<Point> &points) {
    long int minX, maxX, minY, maxY;
    boundingBox(points, minX, minY, maxX, maxY);
    const unsigned long  width = maxX - minX + 1,
                        height = maxY - minY + 1;
    size_t len = height * (width + 1);
    string s (len, ' ');
    for(size_t y=0; y<height; y++) {
        s[toIndex(width, y, width)] = '\n';
    }
    for(Point &p : points) {
        s[toIndex(p.x - minX, p.y - minY, width)] = '#';
    }
    return s;
}