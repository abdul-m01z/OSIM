#ifndef ROLES_H
#define ROLES_H

#include "User.h"


class Junior : public User {
public:
    Junior(string uname);
    void displayMenu() const override;
};

class Employee : public User {
public:
    Employee( string uname);
    void displayMenu() const override;
};

class Manager : public User {
public:
    Manager( string uname);
    void displayMenu() const override;
};

class Director : public User {
public:
    Director( string uname);
    void displayMenu() const override;
};

class Executive : public User {
public:
    Executive( string uname);
    void displayMenu() const override;
};

#endif