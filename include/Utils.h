#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "User.h"
using namespace std;

string* split(string str, char delimiter = '|');
User* getUserObject(const string& username);
string prettyprint(time_t t);
bool isInt(string str);
int getErrFreeInt(const string& prompt, int min = -1, int max = -1);
string getErrFreeString(const string& prompt);
int getAsciiIndex(char c);
void printAscii(const string& text);
string trim(string s);
#endif