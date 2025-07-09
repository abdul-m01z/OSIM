#ifndef MESSAGING_H
#define MESSAGING_H

#include <string>
#include "User.h"

class Messaging {
public:
    static bool sendMessage(User* sender, User* recipient, int type, const std::string& message);
    static void viewMessages(const std::string& username);
};

#endif
