#ifndef USER_H
#define USER_H

#include <iostream>
using namespace std;

class User {
protected:
    string username;
    string passwordHash[65];
    int clearance;

public:
    User(string uname = "", string pass = "");
    string getUsername() const;
    string getHashedPassword() const;
    int getClearanceLevel() const;
    virtual ~User() {}
};

#endif