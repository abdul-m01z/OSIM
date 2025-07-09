#ifndef AUDITLOGGER_H
#define AUDITLOGGER_H

#include <string>
#include <iostream>
using namespace std;

class AuditLogger {
public:

    void log(const string& username, const string& action, const string& details, const string& status);
    friend ostream& operator<<(ostream& os, const AuditLogger& logger);
};

#endif