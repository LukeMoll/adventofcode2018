#include "aocutils.hpp"
#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

class Node {
    public:
        Node * parent;
        vector<unsigned short> data;
        vector<Node*> children;
        Node(Node *parent) : parent(parent), data({}), children({}) {}
        int value() {
            int total = 0;
            if(children.size() > 0) {
                for(unsigned short i : data) {
                    if(i<=children.size() && i > 0) {
                        auto val = children.at(i-1)->value(); // Poor optimisation for repeated data
                        total += val;
                    }
                }
            }
            else {
                for(auto i : data) {total += i;}
            }
            return total;
        }
        ~Node() {
            for(Node* n : children) {
                delete n;
            }
        }
};
Node* parseNodes(string&, int&, Node*);
void walkChildren(Node*);
void sumMetadata(Node*, int&);

int main(void) {
    cout << "Day 8 - Memory Maneuver" << endl;
    vector<string> input_lines = AOC::getLines(8);
    string input = input_lines.front();

    input += " "; // make sure we've got a trailing space
    int pos = 0;
    Node *n = parseNodes(input, pos, nullptr);

    int metasum = 0;
    sumMetadata(n, metasum);
    cout << "(Part 1) Metadata sum: " << metasum << endl;
    cout << "(Part 2) Root value: " << flush;
    cout << n->value() << endl;
    delete n;
    return 0;
}

Node* parseNodes(string &s, int &pos, Node *parent) {
    unsigned short num_child, num_data;
    int offset;
    if(sscanf(s.c_str() + pos, "%hu %hu %n", &num_child, &num_data, &offset) < 2) {
        throw runtime_error("Could not read header at position " + pos);
    }
    else {
        pos += offset;
        Node *n = new Node(parent);
        for(size_t i=0; i<num_child; i++) {
            n->children.push_back(parseNodes(s, pos, n));
        }
        for(size_t i=0; i<num_data; i++) {
            short data;
            sscanf(s.c_str() + pos, "%hd %n", &data, &offset);
            n->data.push_back(data);
            pos += offset;
        }
        return n;        
    }
}

void walkChildren(Node *n) {
    cout << n << " " << n->children.size() << " children, " << n->data.size() << " data: ";
    for(auto i : n->data) {cout << i << " ";}
    cout << endl;
    for(auto c : n->children) {
        walkChildren(c);
    }
}

void sumMetadata(Node *n, int &result) {
    for(auto i : n->data) {result += i;}
    for(auto c : n->children) {
        sumMetadata(c, result);
    }
}