#ifndef TASKSYSTEM_H
#define TASKSYSTEM_H

#include "Task.h"
#include "TimeManager.h"

class TaskSystem {
public:
    void createTask(string dataLine);
    void delegateTask(int taskId, string newAssignee);
    void assignTask(int taskId, string newAssignee);
    void doTask(int taskId);
    void viewTasks(string assignee);
private:
    TimeManager timeManager;
};

#endif
