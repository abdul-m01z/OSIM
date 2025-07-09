#include "../include/TimeManager.h"
#include "../include/Task.h"
#include "../include/Utils.h"
#include "../include/constants.h"
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

void TimeManager::expireOverdueTasks() {
    ifstream in("data/tasks.txt");
    if (!in) {
        cout << "[-] Error: Could not open tasks.txt file" << endl;
        return;
    }

    ofstream temp("data/temp.txt");
    if (!temp) {
        cout << "[-] Error: Could not create temporary file" << endl;
        in.close();
        return;
    }

    string line;
    bool anyExpired = false;
    int lineCount = 0;
    bool hasError = false;

    while (getline(in, line)) {
        lineCount++;

        if (line.empty() || line[0] == '#') {
            temp << line << "\n";
            continue;
        }

        string* parts = nullptr;
        parts = split(line, '|');
        if (!parts || parts[0].empty()) {
            cout << "[-] Error: Invalid task data at line " << lineCount << endl;
            hasError = true;
            temp << line << "\n";
            continue;
        }

        Task task(parts);

        if (task.getTaskId() <= 0) {
            cout << "[-] Error: Invalid task ID at line " << lineCount << endl;
            hasError = true;
            temp << line << "\n";
            delete[] parts;
            continue;
        }

        if (task.getStatus() < TASK_CREATED || task.getStatus() > TASK_EXPIRED) {
            cout << "[-] Error: Invalid task status at line " << lineCount << endl;
            hasError = true;
            temp << line << "\n";
            delete[] parts;
            continue;
        }

        if (task.isExpired() && task.getStatus() != TASK_EXPIRED) {
            task.setStatus(TASK_EXPIRED);
            anyExpired = true;
        }
        temp << task.toString() << "\n";
        delete[] parts;
    }

    in.close();
    temp.close();

    if (hasError) {
        cout << "[!] Some tasks could not be processed. Check the error messages above." << endl;
        remove("data/temp.txt");
        return;
    }

    if (remove("data/tasks.txt") != 0) {
        cout << "[-] Error: Could not remove original tasks file" << endl;
        remove("data/temp.txt");
        return;
    }

    if (rename("data/temp.txt", "data/tasks.txt") != 0) {
        cout << "[-] Error: Could not rename temporary file" << endl;
        rename("data/temp.txt", "data/tasks.txt");
        return;
    }

    if (anyExpired) {
        expireOverdueTasks();
    }
}