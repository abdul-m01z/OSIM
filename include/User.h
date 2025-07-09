#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
using namespace std;

class User {
protected:
    string username;
    int role;
    string hashedPass; 

    bool isValidUsername(const string& uname); 

    public:
        void loadUserData(const string& uname); 
        User(string uname = "",  int r = 0);
        string getUsername() const;
        int getRole() const;
        string getHashedPass() const; 

        void setUsername(string uname);
        void setRole(int r);

        virtual void displayMenu() const = 0;

        virtual ~User() {}
};

#endif