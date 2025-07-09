#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <string>
#include <iostream>
using namespace std;

class PerfEntry {
public:
    string username;
    int score;
};

class PerformanceSystem {
public:
    PerformanceSystem();
    void updateScores();
    int getScore(const string& username);
    friend ostream& operator<<(ostream& os, const PerformanceSystem& ps);
private:
    PerfEntry userScores[1000];
    int userCount;
    void loadScores();
    void saveScores();
    int findUserIndex(const string& username) const;
};

#endif
