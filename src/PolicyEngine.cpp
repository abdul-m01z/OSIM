#include "../include/PolicyEngine.h"
#include "../include/constants.h"
#include "../include/Utils.h"
#include <fstream>
#include <string>

bool PolicyEngine::canCreateTask(User* sender) {
    return sender->getRole() >= ROLE_MANAGER;
}

bool PolicyEngine::canDoTask(int taskId, User* doer) {
    std::ifstream in("data/tasks.txt");
    std::string line;
    while (getline(in, line)) {
        string* parts = split(line, '|');
        int id = atoi(parts[0].c_str());
        if (id == taskId) {
            bool allowed = (parts[4] == doer->getUsername());
            delete[] parts;
            in.close();
            return allowed;
        }
        delete[] parts;
    }
    in.close();
    return false;
}

bool PolicyEngine::canAssignTask(User* sender, User* recipient) {
    return sender->getRole() >= ROLE_MANAGER && recipient->getRole() < sender->getRole();
}

bool PolicyEngine::canDelegateTask(User* sender, User* recipient) {
    return sender->getRole() >= ROLE_EMPLOYEE && recipient->getRole() > sender->getRole();
}

bool PolicyEngine::canSendPrivateMessage(User* sender, User* recipient) {
    return true;
}

bool PolicyEngine::canSendAlert(User* sender, User* recipient) {
    return sender->getRole() >= ROLE_MANAGER &&
           recipient->getRole() <= sender->getRole();
}

bool PolicyEngine::canSendInfo(User* sender, User* recipient) {
    return true;
}