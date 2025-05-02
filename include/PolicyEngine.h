#ifndef POLICY_ENGINE_H
#define POLICY_ENGINE_H
#include "User.h"

class PolicyEngine {
public:
    bool canDelegateTask(User* sender, User* recipient);
    bool canSendPrivateMessage(User* sender, User* recipient);
    bool canSendAlert(User* sender, User* recipient);
    bool canSendInfo(User* sender, User* recipient);
};

#endif