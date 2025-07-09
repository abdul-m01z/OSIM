#include "../include/TaskSystem.h"
#include "../include/Utils.h"
#include "../include/constants.h"
#include <fstream>
#include <iostream>
using namespace std;

class TaskViewEntry {
public:
    Task task;
    string* parts;
    TaskViewEntry(Task t, string* p) : task(t), parts(p) {}
};

void TaskSystem::createTask(string dataLine) {
    if (dataLine.empty()) {
        cout << "[-] Error: Empty task data" << endl;
        return;
    }

    ofstream out("data/tasks.txt", ios::app);
    if (!out) {
        cout << "[-] Error: Could not open tasks.txt for writing" << endl;
        return;
    }

    Task task(dataLine);
    out << task.toString() << "\n";
    out.close();
    cout << "[+] Task created. Task ID: " << task.getTaskId() << "\n";
}

void TaskSystem::delegateTask(int taskId, string newAssignee) {
    if (taskId <= 0) {
        cout << "[-] Error: Invalid task ID" << endl;
        return;
    }

    if (newAssignee.empty()) {
        cout << "[-] Error: Invalid assignee" << endl;
        return;
    }

    ifstream in("data/tasks.txt");
    if (!in) {
        cout << "[-] Error: Could not open tasks.txt" << endl;
        return;
    }

    ofstream temp("data/temp.txt");
    if (!temp) {
        cout << "[-] Error: Could not create temporary file" << endl;
        in.close();
        return;
    }

    string line;
    bool taskFound = false;
    bool hasError = false;

    while (getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            temp << line << "\n";
            continue;
        }

        string* parts = nullptr;
        parts = split(line, '|');
        if (!parts || parts[0].empty()) {
            cout << "[-] Error: Invalid task data format" << endl;
            hasError = true;
            temp << line << "\n";
            continue;
        }

        if (atoi(parts[0].c_str()) == taskId) {
            taskFound = true;
            parts[4] = newAssignee;  
            parts[2] = TASK_ASSIGNED; 

            for (int i = 0; i < 8; i++) {
                temp << parts[i];
                if (i != 7) temp << "|";
            }
            temp << "\n";
        } else {
            temp << line << "\n";
        }
        delete[] parts;
    }

    in.close();
    temp.close();

    if (!taskFound) {
        cout << "[-] Error: Task not found" << endl;
        remove("data/temp.txt");
        return;
    }

    if (hasError) {
        cout << "[!] Some tasks could not be processed" << endl;
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

    cout << "[+] Task " << taskId << " delegated to " << newAssignee << endl;
}

void TaskSystem::assignTask(int taskId, string newAssignee) {
    delegateTask(taskId, newAssignee);
}

void TaskSystem::doTask(int taskId) {
    if (taskId <= 0) {
        cout << "[-] Error: Invalid task ID" << endl;
        return;
    }

    ifstream in("data/tasks.txt");
    if (!in) {
        cout << "[-] Error: Could not open tasks.txt" << endl;
        return;
    }

    ofstream temp("data/temp.txt");
    if (!temp) {
        cout << "[-] Error: Could not create temporary file" << endl;
        in.close();
        return;
    }

    string line;
    bool taskFound = false;
    bool hasError = false;

    while (getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            temp << line << "\n";
            continue;
        }

        string* parts = nullptr;
        parts = split(line, '|');
        if (!parts || parts[0].empty()) {
            cout << "[-] Error: Invalid task data format" << endl;
            hasError = true;
            temp << line << "\n";
            continue;
        }

        int id = atoi(parts[0].c_str());
        if (id == taskId) {
            taskFound = true;
            Task task(parts);
            task.setStatus(TASK_INPROGRESS);
            temp << task.toString() << "\n";
        } else {
            temp << line << "\n";
        }
        delete[] parts;
    }

    in.close();
    temp.close();

    if (!taskFound) {
        cout << "[-] Error: Task not found" << endl;
        remove("data/temp.txt");
        return;
    }

    if (hasError) {
        cout << "[!] Some tasks could not be processed" << endl;
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

    cout << "[*] Task " << taskId << " marked in progress." << endl;
}

void TaskSystem::viewTasks(string assignee) {
    if (assignee.empty()) {
        cout << "[-] Error: Invalid assignee" << endl;
        return;
    }

    ifstream in("data/tasks.txt");
    if (!in) {
        cout << "[-] Error: Could not open tasks.txt" << endl;
        return;
    }

    string line;
    TaskViewEntry* taskList[1000];
    int taskCount = 0;
    bool hasError = false;

    while (getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        string* parts = nullptr;
        parts = split(line, '|');
        if (!parts || parts[0].empty()) {
            cout << "[-] Error: Invalid task data format" << endl;
            hasError = true;
            continue;
        }

        Task task(parts);
        if (task.getAssignee() == assignee) {
            if (taskCount >= 1000) {
                cout << "[-] Warning: Maximum task limit reached" << endl;
                delete[] parts;
                break;
            }
            taskList[taskCount++] = new TaskViewEntry(task, parts);
        } else {
            delete[] parts;
        }
    }
    in.close();

    if (hasError) {
        cout << "[!] Some tasks could not be processed" << endl;
    }

    if (taskCount == 0) {
        cout << "[*] No tasks found for " << assignee << endl;
        return;
    }

    for (int i = 0; i < taskCount - 1; ++i) {
        for (int j = i + 1; j < taskCount; ++j) {
            if (taskList[j]->task.getPriority() > taskList[i]->task.getPriority()) {
                TaskViewEntry* tmp = taskList[i];
                taskList[i] = taskList[j];
                taskList[j] = tmp;
            }
        }
    }

    cout << "=== Tasks for " << assignee << " ===\n";
    for (int i = 0; i < taskCount; ++i) {
        Task& task = taskList[i]->task;
        string* parts = taskList[i]->parts;

        string statusStr;
        switch (task.getStatus()) {
            case TASK_CREATED:    statusStr = "Created"; break;
            case TASK_ASSIGNED:   statusStr = "Assigned"; break;
            case TASK_INPROGRESS: statusStr = "In Progress"; break;
            case TASK_COMPLETED:  statusStr = "Completed"; break;
            case TASK_EXPIRED:    statusStr = "Expired"; break;
            default:              statusStr = "Unknown"; break;
        }

        string priorityStr;
        switch (task.getPriority()) {
            case 1: priorityStr = "Low"; break;
            case 2: priorityStr = "Medium"; break;
            case 3: priorityStr = "High"; break;
            default: priorityStr = "Unknown"; break;
        }

        string expiryStr = task.isExpired() ? "Yes" : "No";

        cout << "-----------------------------\n";
        cout << "Task ID:      " << task.getTaskId() << "\n";
        cout << "Title:        " << parts[1] << "\n";
        cout << "Status:       " << statusStr << "\n";
        cout << "Creator:      " << parts[3] << "\n";
        cout << "Assignee:     " << parts[4] << "\n";
        cout << "Priority:     " << priorityStr << "\n";
        cout << "Created At:   " << prettyprint(task.getCreatedTimestamp()) << "\n";
        cout << "TTL (secs):   " << parts[7] << "\n";
        cout << "Expired:      " << expiryStr << "\n";

        delete[] parts;
        delete taskList[i];
    }
    cout << "-----------------------------\n";
}