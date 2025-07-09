#include "../include/Performance.h"
#include "../include/Utils.h"
#include "../include/constants.h"
#include <fstream>
#include <iostream>
using namespace std;

PerformanceSystem::PerformanceSystem() {
    userCount = 0;
    loadScores();
}

int PerformanceSystem::findUserIndex(const string& username) const {
    if (username.empty()) {
        return -1;
    }
    for (int i = 0; i < userCount; ++i) {
        if (userScores[i].username == username)
            return i;
    }
    return -1;
}

void PerformanceSystem::loadScores() {
    userCount = 0;
    ifstream in("data/performance_report.txt");
    if (!in) {
        cout << "[-] Warning: Could not open performance report file" << endl;
        return;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        string* parts = nullptr;
        parts = split(line, '|');
        if (!parts || parts[0].empty()) {
            cout << "[-] Warning: Invalid performance data format" << endl;
            if (parts) delete[] parts;
            continue;
        }

        if (userCount >= 1000) {
            cout << "[-] Warning: Maximum user limit reached" << endl;
            delete[] parts;
            break;
        }

        userScores[userCount].username = parts[0];
        userScores[userCount].score = atoi(parts[1].c_str());
        userCount++;
        delete[] parts;
    }
    in.close();
}

void PerformanceSystem::saveScores() {
    ofstream out("data/performance_report.txt");
    if (!out) {
        cout << "[-] Error: Could not open performance report file for writing" << endl;
        return;
    }

    for (int i = 0; i < userCount; ++i) {
        if (userScores[i].username.empty()) {
            cout << "[-] Warning: Skipping user with empty username" << endl;
            continue;
        }
        out << userScores[i].username << "|" << userScores[i].score << "\n";
    }
    out.close();
}

void PerformanceSystem::updateScores() {
    userCount = 0;
    bool hasError = false;

    ifstream in("data/tasks.txt");
    if (!in) {
        cout << "[-] Error: Could not open tasks file" << endl;
        return;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        string* parts = nullptr;
        parts = split(line, '|');
        if (!parts || parts[0].empty()) {
            cout << "[-] Warning: Invalid task data format" << endl;
            hasError = true;
            if (parts) delete[] parts;
            continue;
        }

        string assignee = parts[4];
        if (assignee.empty()) {
            cout << "[-] Warning: Task with empty assignee" << endl;
            hasError = true;
            delete[] parts;
            continue;
        }

        int status = atoi(parts[2].c_str());
        int priority = atoi(parts[5].c_str());

        if (status < TASK_CREATED || status > TASK_EXPIRED) {
            cout << "[-] Warning: Invalid task status" << endl;
            hasError = true;
            delete[] parts;
            continue;
        }

        if (priority < 1 || priority > 3) {
            cout << "[-] Warning: Invalid task priority" << endl;
            hasError = true;
            delete[] parts;
            continue;
        }

        int idx = findUserIndex(assignee);
        if (idx == -1) {
            if (userCount >= 1000) {
                cout << "[-] Warning: Maximum user limit reached" << endl;
                delete[] parts;
                break;
            }
            idx = userCount++;
            userScores[idx].username = assignee;
            userScores[idx].score = 0;
        }

        if (status == TASK_COMPLETED) {
            if (priority == 1)
                userScores[idx].score += SCORE_TASK_LOW_COMPLETED;
            else if (priority == 2)
                userScores[idx].score += SCORE_TASK_MEDIUM_COMPLETED;
            else if (priority == 3)
                userScores[idx].score += SCORE_TASK_HIGH_COMPLETED;
        } else if (status == TASK_EXPIRED) {
            if (priority == 1)
                userScores[idx].score += SCORE_TASK_LOW_EXPIRED;
            else if (priority == 2)
                userScores[idx].score += SCORE_TASK_MEDIUM_EXPIRED;
            else if (priority == 3)
                userScores[idx].score += SCORE_TASK_HIGH_EXPIRED;
        }
        delete[] parts;
    }
    in.close();

    ifstream users("data/users.txt");
    if (!users) {
        cout << "[-] Error: Could not open users file" << endl;
        return;
    }

    string userLine;
    while (getline(users, userLine)) {
        if (userLine.empty() || userLine[0] == '#') {
            continue;
        }

        string* parts = nullptr;
        parts = split(userLine, '|');
        if (!parts || parts[0].empty()) {
            cout << "[-] Warning: Invalid user data format" << endl;
            hasError = true;
            if (parts) delete[] parts;
            continue;
        }

        string uname = parts[0];
        if (uname.empty()) {
            cout << "[-] Warning: User with empty username" << endl;
            hasError = true;
            delete[] parts;
            continue;
        }

        string inboxPath = "data/inbox/" + uname + ".txt";
        ifstream inbox(inboxPath);
        if (!inbox) {
            delete[] parts;
            continue;
        }

        string msgLine;
        while (getline(inbox, msgLine)) {
            if (msgLine.empty() || msgLine[0] == '#') {
                continue;
            }

            string* msgParts = nullptr;
            msgParts = split(msgLine, '|');
            if (!msgParts || msgParts[0].empty()) {
                cout << "[-] Warning: Invalid message data format" << endl;
                hasError = true;
                if (msgParts) delete[] msgParts;
                continue;
            }

            string sender = msgParts[2];
            if (sender.empty()) {
                cout << "[-] Warning: Message with empty sender" << endl;
                hasError = true;
                delete[] msgParts;
                continue;
            }

            if (sender != uname) {
                int idx = findUserIndex(sender);
                if (idx == -1) {
                    if (userCount >= 1000) {
                        cout << "[-] Warning: Maximum user limit reached" << endl;
                        delete[] msgParts;
                        break;
                    }
                    idx = userCount++;
                    userScores[idx].username = sender;
                    userScores[idx].score = 0;
                }
                userScores[idx].score += SCORE_MSG_SENT;
            }
            delete[] msgParts;
        }
        inbox.close();
        delete[] parts;
    }
    users.close();

    if (hasError) {
        cout << "[!] Some data could not be processed" << endl;
    }

    saveScores();
}

int PerformanceSystem::getScore(const string& username) {
    if (username.empty()) {
        cout << "[-] Error: Empty username" << endl;
        return 0;
    }
    int idx = findUserIndex(username);
    if (idx != -1) return userScores[idx].score;
    return 0;
}

ostream& operator<<(ostream& os, const PerformanceSystem& ps) {
    if (ps.userCount == 0) {
        os << "[-] No performance data available" << endl;
        return os;
    }

    os << "=== Performance Report ===\n";
    for (int i = 0; i < ps.userCount; ++i) {
        if (ps.userScores[i].username.empty()) {
            continue;
        }
        os << "User: " << ps.userScores[i].username << "\n";
        os << "Score: " << ps.userScores[i].score << "\n";
        os << "-------------------------\n";
    }
    return os;
}