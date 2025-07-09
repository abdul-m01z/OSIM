#include "../include/Notifications.h"
#include <fstream>
#include <ctime>
#include <iostream>
#include <iomanip>
#include "../include/Utils.h"
#include "../include/constants.h"

using namespace std;

void Notifications::createNotification(const string& content, int type) {
    if (content.empty()) {
        cout << "[-] Error: Empty notification content" << endl;
        return;
    }

    if (type != NOTIFICATION_WARNING && type != NOTIFICATION_EMERGENCY) {
        cout << "[-] Error: Invalid notification type" << endl;
        return;
    }

    ofstream out("data/notifications.txt", ios::app);
    if (!out) {
        cout << "[-] Error: Could not open notifications file for writing" << endl;
        return;
    }

    time_t now = time(nullptr);
    if (now == -1) {
        cout << "[-] Error: Failed to get current time" << endl;
        out.close();
        return;
    }

    out << now << "|" << type << "|" << content << "\n";
    if (out.fail()) {
        cout << "[-] Error: Failed to write notification" << endl;
    }
    out.close();
}

void Notifications::viewNotifications() {
    ifstream in("data/notifications.txt");
    if (!in) {
        cout << "[-] No notifications found." << endl;
        return;
    }

    bool hasNotifications = false;
    string line;
    cout << "=== Notifications ===" << endl;
    cout << "┌═════════════════════════════════════════════════════════════════════════════════════════════════════════════════┐" << endl;

    while (getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        string* parts = nullptr;
        parts = split(line, '|');


        time_t t = atol(parts[0].c_str());

        string timestamp = prettyprint(t);
        string typeStr;
        int type = atoi(parts[1].c_str());

        if (type == NOTIFICATION_WARNING) {
            typeStr = "Warning";
        } else if (type == NOTIFICATION_EMERGENCY) {
            typeStr = "Emergency";
        } else {
            cout << "[-] Warning: Unknown notification type" << endl;
            delete[] parts;
            continue;
        }





        string message = trim(parts[2]);

        cout << "█ ";
        cout << ((typeStr == "Warning") ? "⚠️ ":"📢 ");
        cout << left << setw(109) << message;
        cout << "█" << endl;



        hasNotifications = true;
        delete[] parts;
    }

    if (!hasNotifications) {
        cout << "No notifications to display." << endl;
    }
    cout << "└═════════════════════════════════════════════════════════════════════════════════════════════════════════════════┘" << endl;
    in.close();
}