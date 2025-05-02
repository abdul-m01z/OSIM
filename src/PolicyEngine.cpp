#include "PolicyEngine.h"
#include "constants.h"

bool PolicyEngine::canDelegateTask(User* sender, User* recipient) {
    return recipient->getClearanceLevel() >= sender->getClearanceLevel();
}

bool PolicyEngine::canSendPrivateMessage(User* sender, User* recipient) {
    return recipient->getClearanceLevel() >= sender->getClearanceLevel();
}

bool PolicyEngine::canSendAlert(User* sender, User* recipient) {
    return sender->getClearanceLevel() >= ROLE_MANAGER &&
           recipient->getClearanceLevel() <= sender->getClearanceLevel();
}

bool PolicyEngine::canSendInfo(User* sender, User* recipient) {
    return true;
}