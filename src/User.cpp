#include "../include/User.h"
#include <fstream>
#include "../include/Utils.h"

User::User(string uname, int r)
    : username(uname), role(r), hashedPass("") {
    if (!uname.empty()) loadUserData(uname);
}

void User::loadUserData(const string& uname) {
    std::ifstream in("data/users.txt");
    std::string line;
    while (getline(in, line)) {
        string* parts = split(line, '|');
        if (parts[0] == uname) {
            hashedPass = parts[1];
            role = atoi(parts[2].c_str());
            delete[] parts;
            in.close();
            return;
        }
        delete[] parts;
    }
    in.close();
}

string User::getUsername() const {
    return username;
}

int User::getRole() const {
    return role;
}

string User::getHashedPass() const {
    return hashedPass;
}

void User::setUsername(string uname) {
    username = uname;
    loadUserData(uname);
}

void User::setRole(int r) {
    role = r;
}

bool User::isValidUsername(const string& uname) {
    if (uname.length() < 3 || uname.length() > 20) {
        return false;
    }
    for (char c : uname) {
        if (!isalnum(c) && c != '_') {
            return false;
        }
    }
    return true;
}

