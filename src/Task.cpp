#include <fstream>
#include <cstdlib>
#include "../include/Task.h"
#include "../include/Utils.h" 
#include "../include/constants.h"
#include <iostream>
using namespace std;

Task::Task(string dataLine) {
    if (dataLine.empty()) {
        throw runtime_error("Empty task data");
    }

    string* fields = nullptr;
    fields = split(dataLine, '|');
    if (!fields || fields[0].empty()) {
        throw runtime_error("Invalid task data format");
    }

    if (fields[0].empty()) {
        throw runtime_error("Task title cannot be empty");
    }
    title = fields[0];

    status = atoi(fields[1].c_str());
    if (status < TASK_CREATED || status > TASK_EXPIRED) {
        throw runtime_error("Invalid task status");
    }

    if (fields[2].empty()) {
        throw runtime_error("Task creator cannot be empty");
    }
    creator = fields[2];

    if (fields[3].empty()) {
        throw runtime_error("Task assignee cannot be empty");
    }
    assignee = fields[3];

    priority = atoi(fields[4].c_str());
    if (priority < 1 || priority > 3) {
        throw runtime_error("Invalid task priority");
    }

    ttlSeconds = atoi(fields[5].c_str());
    if (ttlSeconds <= 0) {
        throw runtime_error("Task TTL must be positive");
    }

    createdTimestamp = time(0);
    expiryTimestamp = createdTimestamp + ttlSeconds;

    ifstream in("data/tasks.txt");
    if (!in) {
        throw runtime_error("Could not open tasks.txt for reading");
    }

    string line;
    int lastId = 0;

    while (getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        string* parts = nullptr;
        parts = split(line, '|');
        if (!parts || parts[0].empty()) {
            delete[] parts;
            continue;
        }

        int id = atoi(parts[0].c_str());
        if (id > lastId) lastId = id;
        delete[] parts;
    }
    in.close();

    taskId = lastId + 1;
    delete[] fields;
}

Task::Task(string* fields) {
    if (!fields) {
        throw runtime_error("Null fields array");
    }

    taskId = atoi(fields[0].c_str());
    if (taskId <= 0) {
        throw runtime_error("Invalid task ID");
    }

    if (fields[1].empty()) {
        throw runtime_error("Task title cannot be empty");
    }
    title = fields[1];

    status = atoi(fields[2].c_str());
    if (status < TASK_CREATED || status > TASK_EXPIRED) {
        throw runtime_error("Invalid task status");
    }

    if (fields[3].empty()) {
        throw runtime_error("Task creator cannot be empty");
    }
    creator = fields[3];

    if (fields[4].empty()) {
        throw runtime_error("Task assignee cannot be empty");
    }
    assignee = fields[4];

    priority = atoi(fields[5].c_str());
    if (priority < 1 || priority > 3) {
        throw runtime_error("Invalid task priority");
    }

    createdTimestamp = atol(fields[6].c_str());
    if (createdTimestamp <= 0) {
        throw runtime_error("Invalid creation timestamp");
    }

    ttlSeconds = atoi(fields[7].c_str());
    if (ttlSeconds <= 0) {
        throw runtime_error("Task TTL must be positive");
    }

    expiryTimestamp = createdTimestamp + ttlSeconds;
}

string Task::toString() {
    if (taskId <= 0 || title.empty() || creator.empty() || assignee.empty()) {
        throw runtime_error("Invalid task data for string conversion");
    }
    return to_string(taskId) + "|" + title + "|" + to_string(status) + "|" + creator + "|" +
           assignee + "|" + to_string(priority) + "|" + to_string(createdTimestamp) + "|" +
           to_string(ttlSeconds);
}

int Task::getTaskId() { 
    if (taskId <= 0) {
        throw runtime_error("Invalid task ID");
    }
    return taskId; 
}

string Task::getAssignee() { 
    if (assignee.empty()) {
        throw runtime_error("Invalid assignee");
    }
    return assignee; 
}

int Task::getStatus() { 
    if (status < TASK_CREATED || status > TASK_EXPIRED) {
        throw runtime_error("Invalid task status");
    }
    return status; 
}

void Task::setAssignee(string newAssignee) { 
    if (newAssignee.empty()) {
        throw runtime_error("Assignee cannot be empty");
    }
    assignee = newAssignee; 
}

void Task::setStatus(int newStatus) { 
    if (newStatus < TASK_CREATED || newStatus > TASK_EXPIRED) {
        throw runtime_error("Invalid task status");
    }
    status = newStatus; 
}

bool Task::isExpired() {
    time_t currentTime = time(0);
    if (currentTime == -1) {
        throw runtime_error("Failed to get current time");
    }
    return currentTime > expiryTimestamp;
}