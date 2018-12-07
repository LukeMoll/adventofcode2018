#include "aocutils.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <cstdio>
#include <algorithm>


using namespace std;

class Step {
    public:
        char letter;
        vector<char> dependsUpon = {};
        
        Step(char letter) : letter(letter) {}
        bool isAvailable(map<char, Step> &completed) {
            for(char c : dependsUpon) {
                if(completed.count(c) == 0) {
                    return false;
                }
            }
            return true;
        }
        bool isAvailable(vector<char> &completed) {
            for(char c : dependsUpon) {
                if(find(completed.begin(), completed.end(), c) == completed.end()) {
                    return false;
                }
            }
            return true;
        }
        int baseTime() {
            return letter - 64;
        }
};
const Step NULL_STEP = Step('\0'); // ugh
ostream& operator<< (ostream& strm, Step s) {
    strm << "Step " << s.letter;
    if(s.dependsUpon.size() > 0) {
        strm << " depends on ";
        for(char c : s.dependsUpon) {
            strm << c << " ";
        }
    }
    else {
        strm << " has no dependencies";
    }
    return strm;
}
bool operator== (Step &a, const Step &b) {
    return a.letter == b.letter;
}

enum STATE {AVAILABLE, WORKING, DONE};
class Worker {
    public:
        void advance() {
            if(timeRemaining >= 0) {
                timeRemaining--;
            }
            if(timeRemaining == 0) {
                state = DONE;
            }
        }
        void assign(Step s, int t) {
            if(state == AVAILABLE) {
                workingOn = s;
                timeRemaining = t;
                state = WORKING;
            }
            else {
                throw runtime_error("Worker not available!");
            }
        }
        Step recieve() {
            if(state == DONE) {
                state = AVAILABLE;
                return workingOn;
            }
            else {
                throw runtime_error("Worker not done!");
            }
        }
        STATE state;
        Worker() : state(AVAILABLE), workingOn(NULL_STEP), timeRemaining(-1) {}
    private:
        Step workingOn;
        int timeRemaining;
};

vector<Step> addSteps(vector<string> &lines);
bool getNextStep(vector<Step> &remainingSteps, map<char,Step> &completedSteps, Step &out);
void part1(vector<Step> steps);
void part2(vector<Step> steps, size_t num_workers, size_t timeOffset);

int main(void) {
    cout << "Day 07 - The Sum of Its Parts" << endl;
    auto input = AOC::getLines(7);

    auto steps = addSteps(input);
    part1(steps);
    part2(steps, 5, 60);
}

void part1(vector<Step> steps) {
    // pass steps by value to copy it
    map<char, Step> completedSteps;
    vector<char> completionOrder;
    completionOrder.reserve(steps.size());
    
    while(steps.size() > 0) {
        Step nextStep = NULL_STEP;
        if(getNextStep(steps, completedSteps, nextStep)) {
            completedSteps.insert({nextStep.letter, nextStep});
            auto delIt = find(steps.begin(), steps.end(), nextStep);
            if(delIt != steps.end()) {
                steps.erase(delIt);
            }
            completionOrder.push_back(nextStep.letter);
        }
    }
    cout << "(Part 1) ";
    for(char c : completionOrder) {
        cout << c;
    }
    cout << endl;
}

void part2(vector<Step> steps, size_t num_workers, size_t timeOffset) {
    vector<Worker> workers;
    workers.reserve(num_workers);
    for(size_t i=0; i<num_workers; i++) {workers.push_back(Worker());}
    size_t timeElapsed = 0;
    const size_t num_steps = steps.size();
    map<char, Step> completedSteps;
    while(completedSteps.size() < num_steps) { // better to receive all completed tasks first
        for(auto &w : workers) {
            if(w.state == DONE) {
                Step completed = w.recieve();
                completedSteps.insert({completed.letter, completed});
            }
        }
        for(auto &w: workers) {
            if(w.state == AVAILABLE) {
                Step nextStep = NULL_STEP;
                if(getNextStep(steps, completedSteps, nextStep)) {
                    auto delIt = find(steps.begin(), steps.end(), nextStep);
                    if(delIt != steps.end()) {
                        steps.erase(delIt);
                    }
                    w.assign(nextStep, nextStep.baseTime() + timeOffset);
                }
            }
            w.advance();
        }
        timeElapsed++;
    }
    cout << "(Part 2) " << timeElapsed - 1 << " seconds" << endl;
    // There are two hard things in Computer Science...
}

vector<Step> addSteps(vector<string> &lines) {
    char step1, step2;
    vector<Step> steps;
    for(auto &line : lines) {
        if(sscanf(line.c_str(), "Step %c must be finished before step %c can begin", &step1, &step2) == 2) {
            Step s1 = Step(step1), s2 = Step(step2);
            if(find(steps.begin(), steps.end(), s1)  == steps.end()) {
                steps.push_back(s1);
            }
            else {
            }
            auto it = find(steps.begin(), steps.end(), s2);
            if(it == steps.end()) {
                s2.dependsUpon.push_back(step1);
                steps.push_back(s2);
            }
            else {
                it->dependsUpon.push_back(step1);
            }
        }
    }
    return steps;
}

bool getNextStep(vector<Step> &remainingSteps, map<char,Step> &completedSteps, Step &out) {
    vector<Step> possibleSteps;

    for(auto s : remainingSteps) {
        if(s.isAvailable(completedSteps)) {possibleSteps.push_back(s);}
    }

    if(possibleSteps.size() == 0) {return false;}

    sort(possibleSteps.begin(), possibleSteps.end(), [](Step &s1, Step &s2) {
        return s1.letter < s2.letter;
    });

    out = possibleSteps.front();
    return true;
}

