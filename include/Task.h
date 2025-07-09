#ifndef TASK_H
#define TASK_H

#include <string>
#include <ctime>
using namespace std;

class Task {
private:
    int taskId;
    string title;
    int status;
    string creator;
    string assignee;
    int priority;
    time_t createdTimestamp;
    int ttlSeconds;
    time_t expiryTimestamp;

public:
    Task(string dataLine);
    Task(string* fields);

    string toString();

    int getTaskId();
    string getAssignee();
    int getStatus();
    int getPriority() { return priority; }
    time_t getCreatedTimestamp() { return createdTimestamp; }
    int getTtlSeconds() { return ttlSeconds; }
    time_t getExpiryTimestamp() { return expiryTimestamp; }
    void setAssignee(string newAssignee);
    void setStatus(int newStatus);
    bool isExpired();
};

#endif
