#include "../include/AuditLogger.h"
#include "../include/Utils.h"
#include <fstream>
#include <ctime>
#include <iostream>
using namespace std;

void AuditLogger::log(const string& username, const string& action, const string& details, const string& status) {
    ofstream out("data/audit.txt", ios::app);
    if (!out.is_open()) return;
    time_t now = time(nullptr);
    out  << now << "|" << username << "|" << action << "|" << details << "|" << status << "\n";
    out.close();
}

ostream& operator<<(ostream& os, const AuditLogger& logger) {
    ifstream in("data/audit.txt");
    if (!in.is_open()) {
        os << "[-] No audit logs found.\n";
        return os;
    }
    string line;
    os << "=== Audit Logs ===\n";
    while (getline(in, line)) {
        string* parts = split(line, '|');
        if (!parts[0].empty() && !parts[1].empty() && !parts[2].empty() && !parts[3].empty() && !parts[4].empty()) {
            time_t t = atol(parts[0].c_str());
            string timestamp = prettyprint(t);
            string username = parts[1];
            string action = parts[2];
            string details = parts[3];
            string status = parts[4];
            os << "-----------------------------\n";
            os << "Time:    " << timestamp << "\n";
            os << "User:    " << username << "\n";
            os << "Action:  " << action << "\n";
            os << "Details: " << details << "\n";
            os << "Status:  " << status << "\n";
        }
        delete[] parts;
    }
    os << "-----------------------------\n";
    in.close();
    return os;
}
