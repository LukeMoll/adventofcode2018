#include "aocutils.hpp"
#include <iostream>
#include <map>
#include <numeric>
#include <vector>
#include <algorithm>
#include <string>
#include <regex>
#include <cstdio>
#include <ctime>

using namespace std;

class Guard {
    public:
        const int id;
        map<int, int> minutes;

        void setAsleep(int from, int to) {
            /* > Note that guards count as asleep on the minute they fall asleep,
             * > and they count as awake on the minute they wake up.
             */
            for(int i=from; i<to; i++) {
                if(minutes[i]) {
                    minutes[i]++;
                }
                else {
                    minutes[i] = 1;
                }
            }
        }

        int getTotalMinutes() { // https://stackoverflow.com/a/14074100
            return accumulate(begin(minutes), end(minutes), 0, 
                [](const int prev, const auto& ele) {
                    return prev + ele.second;
                });
        }

        pair<int,int> getMostAsleepMinute() {
            vector<pair<int,int>> v(minutes.size());
            copy(minutes.begin(), minutes.end(), v.begin());
            sort(v.begin(), v.end(), [](const pair<int,int> &p1, const pair<int,int> &p2) {
                return p1.second > p2.second;
            });
            if(v.size() > 0) {
                return v.front();
            }
            else return pair<int,int>(0,0);
        }

        Guard(int id) : id(id) {}
};

enum ACTION {BEGIN, ASLEEP, WAKE};
std::ostream& operator<<(std::ostream &strm, const ACTION g) {
    switch(g) {
        case BEGIN:
            return strm << "BEGIN";
        case ASLEEP:
            return strm << "ASLEEP";
        case WAKE:
            return strm << "WAKE";
    }
    return strm;
}

class Record {
    public:
        ACTION action;
        int guard_id = -1;
        int midnight_minutes;
        string date_str;

        static Record fromLine(string line) {
            int year, month, day, hour, minute;
            string former = line.substr(0,19);
            sscanf(former.c_str(), "[%04d-%02d-%02d %02d:%02d]", &year, &month, &day, &hour, &minute);

            ACTION a;
            int guard_id = -1;

            string latter = line.substr(19);
            if(latter.compare("falls asleep") == 0) {
                a = ASLEEP;
            }
            else if(latter.compare("wakes up") == 0) {
                a = WAKE;
            }
            else if(latter.substr(0,5).compare("Guard") == 0) {
                a = BEGIN;
                sscanf(latter.c_str(), "Guard #%d begins shift", &guard_id);
            }
            else {
                throw runtime_error("Action \"" + latter + "\" did not match any pattern.");
            }
          
            return Record(a, guard_id, minute, former.substr(1,16));
        }

    private:
        Record(ACTION a, int guard_id, int mid_mins, string date_str) : 
            action(a), guard_id(guard_id), midnight_minutes(mid_mins), date_str(date_str) {}
};
std::ostream& operator<<(std::ostream &strm, const Record r) {
    return strm << "[" << r.date_str << "] " << r.midnight_minutes << " | " << r.action << " " << r.guard_id;
}

int main(void) {
    cout << "Day 04 - Repose Record" << endl;

    auto lines = AOC::getLines(4);

    vector<Record> records;
    transform(lines.begin(), lines.end(), back_inserter(records), Record::fromLine);
    sort(records.begin(), records.end(), [](const Record &r1, const Record &r2) {
        return r1.date_str.compare(r2.date_str) <= 0;
    });

    int currentGuard = -1;
    int previousMinutes;
    map<int,Guard*> guards;
    for(Record r : records) {
        switch(r.action) {
            case BEGIN:
                if(NULL == guards[r.guard_id]) {
                    guards[r.guard_id] = new Guard(r.guard_id);
                }
                currentGuard = r.guard_id;
                break;
            case ASLEEP:
                // not much to do
                break;
            case WAKE:
                guards[currentGuard]->setAsleep(previousMinutes,r.midnight_minutes);
                break;
        }
        previousMinutes = r.midnight_minutes;
    }

    for(pair<int,Guard*> p : guards) {
        auto MostAsleepMinute = p.second->getMostAsleepMinute();
        cout << "Guard #" << p.first << ":\tTime asleep: " << p.second->getTotalMinutes() << " min\tMost asleep minute: '" << MostAsleepMinute.first << "\t(" << MostAsleepMinute.second << " times)" <<endl;
    }

    // Need to sort but GCC is bein a bicc
    // I'll do it with my eyes
    cout << "\n(Part 1)\tFind the Guard with the highest Time asleep\n"
                  "\t\tand multiply the #ID with the 'minute" << endl;
    cout << "(Part 2)\tFind the Guard with the most times asleep\n"
                  "\t\tand multiply the #ID with the 'minute" << endl;

    for(pair<int,Guard*> p : guards) {
        delete p.second;
    }
    return 0;
}