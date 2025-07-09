#include "../include/Utils.h"
#include <string>
#include <iostream>
#include <fstream>
#include "../include/User.h"
#include "../include/Roles.h"
#include "../include/constants.h"

using namespace std;

string* split(string str, char delimiter) {
    int count = 1;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == delimiter)
            count++;
    }

    string* parts = new string[count];
    int fieldIndex = 0;
    parts[0] = "";

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == delimiter) {
            fieldIndex++;
            parts[fieldIndex] = "";
        } else {
            parts[fieldIndex] = parts[fieldIndex] + str[i];
        }
    }

    return parts;
}

User* getUserObject(const string& username) {
    std::ifstream in("data/users.txt");
    std::string line;
    while (getline(in, line)) {
        string* parts = split(line, '|');
        if (parts[0] == username) {
            int role = atoi(parts[2].c_str());
            User* user = nullptr;
            switch (role) {
                case ROLE_JUNIOR:
                    user = new Junior(username);
                    break;
                case ROLE_EMPLOYEE:
                    user = new Employee(username);
                    break;
                case ROLE_MANAGER:
                    user = new Manager(username);
                    break;
                case ROLE_DIRECTOR:
                    user = new Director(username);
                    break;
                case ROLE_EXECUTIVE:
                    user = new Executive(username);
                    break;
                default:
                    user = nullptr;
            }
            delete[] parts;
            in.close();
            return user;
        }
        delete[] parts;
    }
    in.close();
    return nullptr;
}

string prettyprint(time_t t) {
    struct tm* tm_info = localtime(&t);
    char buf[20];

    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
        tm_info->tm_year + 1900,
        tm_info->tm_mon + 1,
        tm_info->tm_mday,
        tm_info->tm_hour,
        tm_info->tm_min,
        tm_info->tm_sec);
    return string(buf);
}

bool isInt(string str){
    for (int i = 0; i < str.length(); i++){
        if (!isdigit(str[i])){
            return false;
        }
    }
    return true;
}

string trim(string s) {
    int len = s.length();
    int i = len - 1;
    while (i >= 0 && (s[i] == '\n' || s[i] == '\r')) {
        i--;
    }

    string trimmed = "";
    for (int j = 0; j <= i; j++) {
        trimmed = trimmed + s[j];
    }

    return trimmed;
}


string getErrFreeString(const string& prompt) {
    string input;
    while (true) {
        cout << BRIGHT_YELLOW << "[?] " << prompt << RESET;
        getline(cin, input);
        if (!input.empty()) {
            return input;
        }
        cout << RED << "[-] Input cannot be empty. Please try again." << RESET << endl;
    }
}
int getErrFreeInt(const string& prompt, int min, int max) {
    string input;
    while (true) {
        cout << BRIGHT_YELLOW << "[?] " << prompt << RESET;
        getline(cin, input);
        if (input.empty()) {
            cout << RED << "[-] Input cannot be empty. Please try again." << RESET << endl;
            continue;
        }
        if (isInt(input)){
            if (min == -1 && max == -1){
                return stoi(input);
            }
            else if (stoi(input) < min || stoi(input) > max){
                cout << RED << "[-] Input range is invalid" << RESET << endl;
                continue;
            }
            return stoi(input);
        }
        cout << RED << "[-] Input must be a number. Please try again." << RESET << endl;
    }
}

int getAsciiIndex(char c) {
    if (c >= 'a' && c <= 'z') c -= 32;
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= '0' && c <= '9') return 26 + (c - '0');
    return -1;
}


void printAscii(const string& text) {
    for (int row = 0; row < 6; ++row) {
        for (char c : text) {
            int index = getAsciiIndex(c);
            if (index != -1) {
                cout << LETTERS[index][row] << "  ";
            }
        }
        cout << '\n';
    }
}
