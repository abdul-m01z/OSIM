#include "include/User.h"
#include "include/Roles.h"
#include "include/Utils.h"
#include "include/Auth.h"
#include "include/PolicyEngine.h"
#include "include/Task.h"
#include "include/Messaging.h"
#include "include/TimeManager.h"
#include "include/constants.h"
#include <iostream>
#include <fstream>
#include <string>



bool isValidUsername(const string& username) {
    if (username.length() < 3 || username.length() > 20) {
        cout << RED << "[-] Username must be between 3 and 20 characters." << RESET << endl;
        return false;
    }
    for (char c : username) {
        if (!isalnum(c) && c != '_') {
            cout << RED << "[-] Username can only contain letters, numbers, and underscores." << RESET << endl;
            return false;
        }
    }
    return true;
}

bool isStrongPassword(const string& password) {
    if (password.length() < 8) {
        cout << RED << "[-] Password must be at least 8 characters long." << RESET << endl;
        return false;
    }
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        else if (islower(c)) hasLower = true;
        else if (isdigit(c)) hasDigit = true;
        else hasSpecial = true;
    }
    if (!hasUpper || !hasLower || !hasDigit || !hasSpecial) {
        cout << RED << "[-] Password must contain uppercase, lowercase, numbers, and special characters." << RESET << endl;
        return false;
    }
    return true;
}

int main() {
    int choice;
    bool running = true;
    system("clear");
    while(running) {
        cout << endl;
        cout << " ██████╗ ███████╗██╗███╗   ███╗" << endl;
        cout << "██╔═══██╗██╔════╝██║████╗ ████║" << endl;
        cout << "██║   ██║███████╗██║██╔████╔██║" << endl;
        cout << "██║   ██║╚════██║██║██║╚██╔╝██║" << endl;
        cout << "╚██████╔╝███████║██║██║ ╚═╝ ██║" << endl;
        cout << " ╚═════╝ ╚══════╝╚═╝╚═╝     ╚═╝" << endl;
        cout << endl << endl;
        cout << "[+]═════════════[ Organizational Simulation and Internal Management System ]═════════════[+]" << endl << endl;
        cout << "[ Powered by C++ ]" << endl;

        cout << endl;
        cout << CYAN;
        cout << "┌══════════════════════════════════════┐" << endl;
        cout << "█ 1) Login                             █" << endl;
        cout << "█ 2) Register                          █" << endl;                
        cout << "█ 3) Forgot Password                   █" << endl;
        cout << "█ 0) Exit                              █" << endl;
        cout << "└══════════════════════════════════════┘" << endl;
        cout << RESET;

        choice = getErrFreeInt("Enter choice[0-3]: ",0, 3);
        cout << endl;

        switch (choice) {
            case 0: {
                cout << GREEN << "Thank you for using the system. Goodbye!" << RESET << endl;
                running = false;
                break;
            }
            case 1: {
                string username, password;
                int loginAttempts = 0;
                User* user = nullptr;

                while (loginAttempts < 3 && !user) {
                    username = getErrFreeString("Enter username: ");
                    password = getErrFreeString("Enter password: ");

                    if (!isValidUsername(username)) {
                        cout << RED << "[-] Invalid username format." << RESET << endl;
                        loginAttempts++;
                        continue;
                    }

                    user = login(username, password);
                    if (!user) {
                        cout << RED << "[-] Invalid credentials" << RESET << endl;
                        loginAttempts++;
                        if (loginAttempts < 3) {
                            cout << YELLOW << "[!] Attempts left: " << (3 - loginAttempts) << endl << endl;
                            
                        }
                    }
                }

                if (user) {
                    cout << GREEN << "[+] Login successful" << RESET << endl;
                    user->displayMenu();
                    delete user;
                    continue;
                } else {
                    cout << RED << "[-] Maximum login attempts reached...Exiting" << RESET << endl << endl << endl << endl;
                }
                break;
            }
            case 2: {
                string newUsername, newPassword, favoriteMovie;
                bool validInput = false;

                while (!validInput) {
                    newUsername = getErrFreeString("Enter new username: ");
                    if (!isValidUsername(newUsername)) {
                        continue;
                    }

                    newPassword = getErrFreeString("Enter new password: ");
                    if (!isStrongPassword(newPassword)) {
                        continue;
                    }

                    favoriteMovie = getErrFreeString("Enter your favorite movie (for password recovery): ");
                    if (favoriteMovie.length() < 2) {
                        cout << RED << "[-] Movie title must be at least 2 characters long." << RESET << endl;
                        continue;
                    }

                    validInput = true;
                }

                int role = getErrFreeInt("Enter role (1 for Junior, 2 for Employee, 3 for Manager): ",1, 3);

                User* newUser = registerUser(newUsername, newPassword, role, favoriteMovie);
                if (newUser) {
                    cout << GREEN << "[+] User registered successfully" << RESET << endl;
                    delete newUser;
                } else {
                    cout << RED << "[-] Registration failed" << RESET << endl;
                }
                break;
            }
            case 3: {
                string username = getErrFreeString("Enter username: ");
                if (!isValidUsername(username)) {
                    cout << RED << "[-] Invalid username format." << RESET << endl;
                    break;
                }

                string favoriteMovie = getErrFreeString("Enter your favorite movie: ");
                if (favoriteMovie.length() < 2) {
                    cout << RED << "[-] Movie title must be at least 2 characters long." << RESET << endl;
                    break;
                }

                if (verifySecurityQuestion(username, favoriteMovie)) {
                    string newPassword;
                    bool validPassword = false;
                    while (!validPassword) {
                        newPassword = getErrFreeString("Enter new password: ");
                        if (isStrongPassword(newPassword)) {
                            validPassword = true;
                        }
                    }

                    if (resetPassword(username, newPassword)) {
                        cout << GREEN << "[+] Password reset successful" << RESET << endl;
                    } else {
                        cout << RED << "[-] Password reset failed" << RESET << endl;
                    }
                } else {
                    cout << RED << "[-] Security question verification failed" << RESET << endl;
                }
                break;
            }
            default: {
                cout << RED << "[-] Invalid choice. Please try again." << RESET << endl;
                break;
            }
        }

    }
    return 0;
}