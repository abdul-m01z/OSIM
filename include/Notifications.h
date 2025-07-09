#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <string>
using namespace std;

class Notifications {
public:

    void createNotification(const string& content, int type);
    void viewNotifications();
};

#endif
