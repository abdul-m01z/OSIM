#ifndef AUTH_H
#define AUTH_H

#include "User.h"
#include <fstream>
#include <iostream>

string customHash(string input);

User* login(string username, string password);
User* registerUser(string username, string password, int role, string favoriteMovie);
void logout(User* user);
bool verifySecurityQuestion(string username, string favoriteMovie);
bool resetPassword(string username, string newPassword);

#endif