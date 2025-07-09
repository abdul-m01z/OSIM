#include "../include/Auth.h"
#include "../include/User.h"
#include "../include/constants.h"
#include "../include/Roles.h"
#include "../include/Utils.h"
#include "../include/AuditLogger.h"
#include <cstdlib>
#include <ctime>

string customHash(string input) {
    unsigned long long hash = 5381;
    int c;

    for (int i = 0; i < input.length(); ++i) {
        c = static_cast<char>(input[i]);
        hash = (hash * 33) + c;
    }

    return to_string(hash);
}

User* login(string username, string password) {
    ifstream usersFileHandle("data/users.txt");
    AuditLogger auditLogger;

    if (!usersFileHandle.is_open()) {
        cout << RED << "[-] Error: Could not open users.txt" << RESET << endl;
        return nullptr;
    }

    string fileUsername, filePassword, fileRole;
    string userInputtedPasswordHash = customHash(password);

    string line;
    while (getline(usersFileHandle, line)) {
        string* fields = split(line);
        fileUsername = fields[0];
        filePassword = fields[1];
        fileRole = fields[2];

        if (fileUsername == username && filePassword == userInputtedPasswordHash) {

            srand((unsigned int)time(0));
            int otp = 100000 + rand() % 900000;
            time_t otpGenTime = time(0);
            int userOtp;
            cout << YELLOW << endl << "[OTP] Your one-time password is: " << otp << RESET << endl << endl;
            userOtp = getErrFreeInt("Enter OTP: ");
            time_t otpCheckTime = time(0);
            if (otpCheckTime - otpGenTime > 60) {
                cout << endl << RED << "[-] OTP expired. Please try logging in again." << RESET << endl;
                auditLogger.log(username, "LOGIN", "OTP expired", "FAIL");
                delete[] fields;
                usersFileHandle.close();
                return nullptr;
            }
            if (userOtp != otp) {
                cout << RED << "[-] Invalid OTP." << RESET << endl;
                auditLogger.log(username, "LOGIN", "Invalid OTP", "FAIL");
                delete[] fields;
                usersFileHandle.close();
                return nullptr;
            }
            string roleStr = fileRole;
            string status = "SUCCESS";
            auditLogger.log(username, "LOGIN", "Role: " + trim(roleStr), status);
            if (stoi(fileRole) == ROLE_JUNIOR){
                usersFileHandle.close();
                delete[] fields;
                return new Junior(fileUsername);
            }
            else if (stoi(fileRole) == ROLE_EMPLOYEE){
                usersFileHandle.close();
                delete[] fields;
                return new Employee(fileUsername);
            }
            else if (stoi(fileRole) == ROLE_MANAGER){
                usersFileHandle.close();
                delete[] fields;
                return new Manager(fileUsername);
            }
            else if (stoi(fileRole) == ROLE_DIRECTOR){
                usersFileHandle.close();
                delete[] fields;
                return new Director(fileUsername);
            }
            else if (stoi(fileRole) == ROLE_EXECUTIVE){
                usersFileHandle.close();
                delete[] fields;
                return new Executive(fileUsername);
            }
        }
        delete[] fields;
    }
    auditLogger.log(username, "LOGIN", "Invalid credentials", "FAIL");
    usersFileHandle.close();
    return nullptr;
}

User* registerUser(string username, string password, int role, string favoriteMovie) {
    ifstream usersFileHandle("data/users.txt");
    AuditLogger auditLogger;

    if (!usersFileHandle.is_open()) {
        cout << RED << "[-] Error: Could not open users.txt" << RESET << endl;
        return nullptr;
    }

    string line;
    string userInputtedPasswordHash = customHash(password);
    string hashedFavoriteMovie = customHash(favoriteMovie);

    while (getline(usersFileHandle, line)) {
        string* fields = split(line);
        string fileUsername = fields[0];

        if (fileUsername == username) {
            cout << RED << "[-] Error: User already exists" << RESET << endl;
            delete[] fields;
            usersFileHandle.close();
            auditLogger.log(username, "REGISTER", "User already exists", "FAIL");
            return nullptr;
        }
        delete[] fields;
    }

    usersFileHandle.close();

    ofstream usersFile("data/users.txt", ios::app);
    if (!usersFile.is_open()) {
        cout << RED << "[-] Error: Could not open users.txt" << RESET << endl;
        return nullptr;
    }

    usersFile << username + "|" + userInputtedPasswordHash + "|" + to_string(role) + "|" + hashedFavoriteMovie << endl;
    usersFile.close();
    auditLogger.log(username, "REGISTER", "Role: " + to_string(role), "SUCCESS");

    if (role == ROLE_JUNIOR)
        return new Junior(username);
    else if (role == ROLE_EMPLOYEE)
        return new Employee(username);
    else if (role == ROLE_MANAGER)
        return new Manager(username);
    else if (role == ROLE_DIRECTOR)
        return new Director(username);
    else if (role == ROLE_EXECUTIVE)
        return new Executive(username);

    return nullptr;
}

bool verifySecurityQuestion(string username, string favoriteMovie) {
    ifstream usersFileHandle("data/users.txt");
    AuditLogger auditLogger;

    if (!usersFileHandle.is_open()) {
        cout << RED << "[-] Error: Could not open users.txt" << RESET << endl;
        return false;
    }

    string line;
    string hashedFavoriteMovie = customHash(favoriteMovie);

    while (getline(usersFileHandle, line)) {
        string* fields = split(line);
        if (fields[0] == username) {
            bool verified = (fields[3] == hashedFavoriteMovie);
            delete[] fields;
            usersFileHandle.close();
            auditLogger.log(username, "SECURITY_VERIFY", verified ? "Success" : "Failed", verified ? "SUCCESS" : "FAIL");
            return verified;
        }
        delete[] fields;
    }

    usersFileHandle.close();
    auditLogger.log(username, "SECURITY_VERIFY", "User not found", "FAIL");
    return false;
}

bool resetPassword(string username, string newPassword) {
    ifstream usersFileHandle("data/users.txt");
    AuditLogger auditLogger;
    ofstream tempFile("data/temp_users.txt");
    bool found = false;

    if (!usersFileHandle.is_open() || !tempFile.is_open()) {
        cout << RED << "[-] Error: Could not open users file" << RESET << endl;
        return false;
    }

    string line;
    string newHashedPassword = customHash(newPassword);

    while (getline(usersFileHandle, line)) {
        string* fields = split(line);
        if (fields[0] == username) {

            tempFile << fields[0] + "|" + newHashedPassword + "|" + fields[2] + "|" + fields[3] << endl;
            found = true;
        } else {
            tempFile << line << endl;
        }
        delete[] fields;
    }
    usersFileHandle.close();
    tempFile.close();

    if (!found) {
        remove("data/temp_users.txt");
        auditLogger.log(username, "PASSWORD_RESET", "User not found", "FAIL");
        return false;
    }

    remove("data/users.txt");
    rename("data/temp_users.txt", "data/users.txt");

    auditLogger.log(username, "PASSWORD_RESET", "Password changed", "SUCCESS");
    return true;
}

void logout(User* user) {
    AuditLogger auditLogger;
    if (user) {
        auditLogger.log(user->getUsername(), "LOGOUT", "", "SUCCESS");
        cout << GREEN << "[+] Successfully logged out." << RESET << endl;
        cout << endl << "Returning to main menu..." << endl << endl << endl << endl;

    } else {
        cout << RED << "[-] Error: No user is currently logged in." << RESET << endl;
    }
}