#include "../include/Roles.h"
#include "../include/constants.h"
#include "../include/TaskSystem.h"
#include "../include/PolicyEngine.h"
#include "../include/Utils.h"
#include "../include/Messaging.h"
#include "../include/AuditLogger.h"
#include "../include/Performance.h"
#include "../include/Notifications.h"
#include "../include/Auth.h"
#include <iostream>

Junior::Junior(string uname)
    : User(uname, ROLE_JUNIOR) {}

void Junior::displayMenu() const
{
    cout << endl;
    int choice;
    PolicyEngine policy;
    TaskSystem taskSystem;
    Notifications notifications;
    cout << "[+]═════════════[ Welcome Sir! ]═════════════[+]" << endl << endl;
    printAscii(this->getUsername());
    cout << endl;
    notifications.viewNotifications();
    do
    {
        cout << endl << endl;
        cout << CYAN;
        cout << "┌══════════════════════════════════════┐" << endl;
        cout << "█ 1) View Task                         █\n";
        cout << "█ 2) Mark Task                         █\n"; 
        cout << "█ 3) Send Info Message                 █\n";
        cout << "█ 4) Send Private Message              █\n";
        cout << "█ 5) Receive Message                   █\n";
        cout << "█ 0) Log out                           █\n";
        cout << "└══════════════════════════════════════┘" << endl;
        cout << RESET;
        choice = getErrFreeInt("Enter choice: ", 0, 5);

        string recipientName;
        string message;

        switch (choice)
        {
        case 1: {
            taskSystem.viewTasks(this->getUsername());
            break;
        }
        case 2: {

            int taskId;
            taskId = getErrFreeInt("Enter Task ID to mark as in progress: ", -1, -1);
            if (policy.canDoTask(taskId, (User*)this)) {
                taskSystem.doTask(taskId);
            } else {
                cout << "[!] You do not have permission to mark this task.\n";
            }
            break;
        }
        case 3: {

            recipientName = getErrFreeString("Recipient Username: ");
            message = getErrFreeString("Message: ");
            User* recipient = getUserObject(recipientName);
            if (recipient) {
                Messaging::sendMessage((User*)this, recipient, MSG_INFO, message);
                delete recipient;
            } else {
                cout << "[-] Recipient not found.\n";
            }
            break;
        }
        case 4: {

            recipientName = getErrFreeString("Recipient Username: ");
            message = getErrFreeString("Message: ");
            User* recipient = getUserObject(recipientName);
            if (recipient) {
                Messaging::sendMessage((User*)this, recipient, MSG_PRIVATE, message);
                delete recipient;
            } else {
                cout << "[-] Recipient not found.\n";
            }
            break;
        }
        case 5: {

            Messaging::viewMessages(this->getUsername());
            break;
        }
        case 0:
            logout((User*)this);
            return;
        default:
            cout << "Invalid option.\n";
        }
    } while (choice != 0);
}

Employee::Employee(string uname)
    : User(uname, ROLE_EMPLOYEE) {}

void Employee::displayMenu() const
{
    cout << endl;
    int choice;
    PolicyEngine policy;
    TaskSystem taskSystem;
    Notifications notifications;
    cout << "[+]═════════════[ Welcome Sir! ]═════════════[+]" << endl << endl;
    printAscii(this->getUsername());
    cout << endl;
    notifications.viewNotifications();
    do
    {
        cout << endl << endl;
        cout << CYAN;
        cout << "┌══════════════════════════════════════┐" << endl;
        cout << "█ 1) View Task                         █\n";
        cout << "█ 2) Create New Task                   █\n";
        cout << "█ 3) Delegate Task                     █\n";
        cout << "█ 4) Mark Task                         █\n";
        cout << "█ 5) Send Info Message                 █\n";
        cout << "█ 6) Send Private Message              █\n";
        cout << "█ 0) Log out                           █\n";
        cout << "└══════════════════════════════════════┘" << endl;
        cout << RESET;
        choice = getErrFreeInt("Enter choice: ", 0, 6);

        string recipientName;
        string message;

        switch (choice)
        {
        case 1: {

            taskSystem.viewTasks(this->getUsername());
            break;
        }
        case 2: {

            string title, creator, assignee;
            int status = TASK_CREATED;
            int priority, ttlSeconds;
            title = getErrFreeString("Enter task title: ");
            creator = this->getUsername();
            assignee = getErrFreeString("Enter assignee username: ");
            priority = getErrFreeInt("Enter priority (int): ", -1, -1);
            ttlSeconds = getErrFreeInt("Enter TTL seconds: ", -1, -1);

            string dataLine = title + "|" + to_string(status) + "|" + creator + "|" + assignee + "|" + to_string(priority) + "|" + to_string(ttlSeconds);
            if (policy.canCreateTask((User*)this)) {
                taskSystem.createTask(dataLine);
            } else {
                cout << "[!] You do not have permission to create tasks.\n";
            }
            break;
        }
        case 3: {

            int taskId;
            taskId = getErrFreeInt("Enter Task ID to delegate: ", -1, -1);
            recipientName = getErrFreeString("Delegate task to (Username): ");
            User* recipient = getUserObject(recipientName);
            if (recipient && policy.canDelegateTask((User*)this, recipient)) {
                taskSystem.delegateTask(taskId, recipientName);
                cout << "[*] Task delegated.\n";
            } else {
                cout << "[!] You do not have permission to delegate tasks to this user.\n";
            }
            delete recipient;
            break;
        }
        case 4: {

            int taskId;
            taskId = getErrFreeInt("Enter Task ID to mark as in progress: ", -1, -1);
            if (policy.canDoTask(taskId, (User*)this)) {
                taskSystem.doTask(taskId);
            } else {
                cout << "[!] You do not have permission to mark this task.\n";
            }
            break;
        }
        case 5: {

            recipientName = getErrFreeString("Recipient Username: ");
            message = getErrFreeString("Message: ");
            User* recipient = getUserObject(recipientName);
            if (recipient) {
                Messaging::sendMessage((User*)this, recipient, MSG_INFO, message);
                delete recipient;
            } else {
                cout << "[-] Recipient not found.\n";
            }
            break;
        }
        case 6: {

            recipientName = getErrFreeString("Recipient Username: ");
            message = getErrFreeString("Message: ");
            User* recipient = getUserObject(recipientName);
            if (recipient) {
                Messaging::sendMessage((User*)this, recipient, MSG_PRIVATE, message);
                delete recipient;
            } else {
                cout << "[-] Recipient not found.\n";
            }
            break;
        }
        case 0:
            logout((User*)this);
            return; 
        default:
            cout << "Invalid option.\n";
        }
    } while (choice != 0);
}

Manager::Manager( string uname)
    : User(uname, ROLE_MANAGER) {}

void Manager::displayMenu() const
{
    cout << endl;
    char choice;
    PolicyEngine policy;
    TaskSystem taskSystem;
    AuditLogger auditLogger;
    Notifications notifications;
    cout << "[+]═════════════[ Welcome Sir! ]═════════════[+]" << endl << endl;
    printAscii(this->getUsername());
    cout << endl;
    notifications.viewNotifications();

    bool menuActive = true;
    while(menuActive) {
        cout << endl << endl;
        cout << CYAN;
        cout << "┌══════════════════════════════════════┐" << endl;
        cout << "█ 1) View Tasks                        █\n";
        cout << "█ 2) Create New Task                   █\n";
        cout << "█ 3) Assign Task                       █\n";
        cout << "█ 4) Delegate Task                     █\n";
        cout << "█ 5) Message Menu                      █\n";
        cout << "█ 0) Log out                           █\n";
        cout << "└══════════════════════════════════════┘" << endl;
        cout << RESET;

        string inputChoice = getErrFreeString("");
        if (inputChoice.empty()) continue;
        choice = inputChoice[0];

        string recipientName;
        string message;

        switch (choice)
        {
        case '1': {
            taskSystem.viewTasks(this->getUsername());
            break;
        }
        case '2': {

            string title, creator, assignee;
            int status = TASK_CREATED;
            int priority, ttlSeconds;

            title = getErrFreeString("Enter task title: ");
            if (title.empty()) {
                cout << RED << "[-] Task title cannot be empty." << RESET << endl;
                break;
            }

            creator = this->getUsername();

            assignee = getErrFreeString("Enter assignee username: ");
            if (assignee.empty()) {
                cout << RED << "[-] Assignee username cannot be empty." << RESET << endl;
                break;
            }

            priority = getErrFreeInt("Enter priority (1-5): ", 1, 5);

            ttlSeconds = getErrFreeInt("Enter TTL seconds (minimum 30): ", 30, 10000);

            string dataLine = title + "|" + to_string(status) + "|" + creator + "|" + assignee + "|" + to_string(priority) + "|" + to_string(ttlSeconds);
            if (policy.canCreateTask((User*)this)) {
                taskSystem.createTask(dataLine);
                cout << GREEN << "[+] Task created successfully" << RESET << endl;
                auditLogger.log(this->getUsername(), "TASK_CREATE", "Title: " + title + " Assignee: " + assignee, "SUCCESS");
            } else {
                cout << RED << "[!] You do not have permission to create tasks." << RESET << endl;
                auditLogger.log(this->getUsername(), "TASK_CREATE", "Title: " + title + " Assignee: " + assignee, "DENIED");
            }
            break;
        }
        case '3': {

            int taskId;
            taskId = getErrFreeInt("Enter Task ID to assign: ", 1, 10000);

            recipientName = getErrFreeString("Assign task to (Username): ");
            if (recipientName.empty()) {
                cout << RED << "[-] Recipient username cannot be empty." << RESET << endl;
                break;
            }

            User* recipient = getUserObject(recipientName);
            if (recipient && policy.canAssignTask((User*)this, recipient)) {
                taskSystem.assignTask(taskId, recipientName);
                cout << GREEN << "[+] Task assigned successfully" << RESET << endl;
                auditLogger.log(this->getUsername(), "TASK_ASSIGN", "TaskID: " + to_string(taskId) + " To: " + recipientName, "SUCCESS");
            } else {
                cout << RED << "[!] You do not have permission to assign tasks to this user." << RESET << endl;
                auditLogger.log(this->getUsername(), "TASK_ASSIGN", "TaskID: " + to_string(taskId) + " To: " + recipientName, "DENIED");
            }
            delete recipient;
            break;
        }
        case '4': {

            int taskId;
            taskId = getErrFreeInt("Enter Task ID to delegate: ", 1, 10000);

            recipientName = getErrFreeString("Delegate task to (Username): ");
            if (recipientName.empty()) {
                cout << RED << "[-] Recipient username cannot be empty." << RESET << endl;
                break;
            }

            User* recipient = getUserObject(recipientName);
            if (recipient && policy.canDelegateTask((User*)this, recipient)) {
                taskSystem.delegateTask(taskId, recipientName);
                cout << GREEN << "[+] Task delegated successfully" << RESET << endl;
                auditLogger.log(this->getUsername(), "TASK_DELEGATE", "TaskID: " + to_string(taskId) + " To: " + recipientName, "SUCCESS");
            } else {
                cout << RED << "[!] You do not have permission to delegate tasks to this user." << RESET << endl;
                auditLogger.log(this->getUsername(), "TASK_DELEGATE", "TaskID: " + to_string(taskId) + " To: " + recipientName, "DENIED");
            }
            delete recipient;
            break;
        }
        case '5': {

            int msgChoice;
            msgChoice = getErrFreeInt("\n=== Message Menu ===\n1. Send Info Message\n2. Send Private Message\n3. Send Alert Message\n4. View Messages\n0. Back\nEnter choice: ", 0, 4);

            switch (msgChoice) {
                case 1:
                case 2:
                case 3: {
                    recipientName = getErrFreeString("Recipient Username: ");
                    if (recipientName.empty()) {
                        cout << RED << "[-] Recipient username cannot be empty." << RESET << endl;
                        break;
                    }

                    message = getErrFreeString("Message: ");
                    if (message.empty()) {
                        cout << RED << "[-] Message cannot be empty." << RESET << endl;
                        break;
                    }

                    User* recipient = getUserObject(recipientName);
                    if (recipient) {
                        string msgTypeStr = (msgChoice == 1) ? "INFO" : (msgChoice == 2) ? "PRIVATE" : "ALERT";
                        int msgType = (msgChoice == 1) ? MSG_INFO : (msgChoice == 2) ? MSG_PRIVATE : MSG_ALERT;
                        Messaging::sendMessage((User*)this, recipient, msgType, message);
                        cout << GREEN << "[+] Message sent successfully" << RESET << endl;
                        auditLogger.log(this->getUsername(), "MESSAGE_SEND", "To: " + recipientName + " Type: " + msgTypeStr, "SUCCESS");
                        delete recipient;
                    } else {
                        cout << RED << "[-] Recipient not found." << RESET << endl;
                        auditLogger.log(this->getUsername(), "MESSAGE_SEND", "To: " + recipientName, "FAIL");
                    }
                    break;
                }
                case 4:
                    Messaging::viewMessages(this->getUsername());
                    break;
                case 0:
                    break;
                default:
                    cout << RED << "[-] Invalid option." << RESET << endl;
            }
            break;
        }
        case '0': {
            logout((User*)this);
            return;
        }
        default: {
            cout << RED << "[-] Invalid option." << RESET << endl;
            break;
        }
        }

        if (menuActive) {
            cout << "\n" << YELLOW << "Press Enter to continue..." << RESET;
        }
    }
}

Director::Director( string uname)
    : User(uname, ROLE_DIRECTOR) {}

void Director::displayMenu() const
{
    cout << endl;
    int choice;
    PolicyEngine policy;
    TaskSystem taskSystem;
    AuditLogger auditLogger;
    PerformanceSystem perfSystem;
    Notifications notifications;
    cout << "[+]═════════════[ Welcome Sir! ]═════════════[+]" << endl << endl;
    printAscii(this->getUsername());
    cout << endl;
    notifications.viewNotifications();
    do
    {
        cout << endl << endl;
        cout << CYAN;
        cout << "┌══════════════════════════════════════┐" << endl;
        cout << "█ 1) View Task                         █\n";
        cout << "█ 2) Assign Task                       █\n";
        cout << "█ 3) Create New Task                   █\n";
        cout << "█ 4) Delegate Task                     █\n";
        cout << "█ 5) Send Info Message                 █\n";
        cout << "█ 6) Send Private Message              █\n";
        cout << "█ 7) Send Alert Message                █\n";
        cout << "█ 8) View Team Performance             █\n";
        cout << "█ 9) View Audit Logs                   █\n";
        cout << "█ 0) Log out                           █\n";
        cout << "└══════════════════════════════════════┘" << endl;
        cout << RESET;
        choice = getErrFreeInt("Enter choice: ", 0, 9);

        string recipientName;
        string message;

        switch (choice)
        {
        case 1: {

            taskSystem.viewTasks(this->getUsername());
            break;
        }
        case 2: {

            int taskId;
            taskId = getErrFreeInt("Enter Task ID to assign: ", -1, -1);
            recipientName = getErrFreeString("Assign task to (Username): ");
            User* recipient = getUserObject(recipientName);
            if (recipient && policy.canAssignTask((User*)this, recipient)) {
                taskSystem.assignTask(taskId, recipientName);
                cout << "[*] Task assigned.\n";
            } else {
                cout << "[!] You do not have permission to assign tasks to this user.\n";
            }
            delete recipient;
            break;
        }
        case 3: {

            string title, creator, assignee;
            int status = TASK_CREATED;
            int priority, ttlSeconds;
            title = getErrFreeString("Enter task title: ");
            creator = this->getUsername();
            assignee = getErrFreeString("Enter assignee username: ");
            priority = getErrFreeInt("Enter priority (int): ", -1, -1);
            ttlSeconds = getErrFreeInt("Enter TTL seconds: ", -1, -1);
            string dataLine = title + "|" + to_string(status) + "|" + creator + "|" + assignee + "|" + to_string(priority) + "|" + to_string(ttlSeconds);
            if (policy.canCreateTask((User*)this)) {
                taskSystem.createTask(dataLine);
            } else {
                cout << "[!] You do not have permission to create tasks.\n";
            }
            break;
        }
        case 4: {

            int taskId;
            taskId = getErrFreeInt("Enter Task ID to delegate: ", -1, -1);
            recipientName = getErrFreeString("Delegate task to (Username): ");
            User* recipient = getUserObject(recipientName);
            if (recipient && policy.canDelegateTask((User*)this, recipient)) {
                taskSystem.delegateTask(taskId, recipientName);
                cout << "[*] Task delegated.\n";
            } else {
                cout << "[!] You do not have permission to delegate tasks to this user.\n";
            }
            delete recipient;
            break;
        }
        case 5: {

            recipientName = getErrFreeString("Recipient Username: ");
            message = getErrFreeString("Message: ");
            User* recipient = getUserObject(recipientName);
            if (recipient) {
                Messaging::sendMessage((User*)this, recipient, MSG_INFO, message);
                delete recipient;
            } else {
                cout << "[-] Recipient not found.\n";
            }
            break;
        }
        case 6: {

            recipientName = getErrFreeString("Recipient Username: ");
            message = getErrFreeString("Message: ");
            User* recipient = getUserObject(recipientName);
            if (recipient) {
                Messaging::sendMessage((User*)this, recipient, MSG_PRIVATE, message);
                delete recipient;
            } else {
                cout << "[-] Recipient not found.\n";
            }
            break;
        }
        case 7: {

            recipientName = getErrFreeString("Recipient Username: ");
            message = getErrFreeString("Message: ");
            User* recipient = getUserObject(recipientName);
            if (recipient) {
                Messaging::sendMessage((User*)this, recipient, MSG_ALERT, message);
                delete recipient;
            } else {
                cout << "[-] Recipient not found.\n";
            }
            break;
        }
        case 8: {

            perfSystem.updateScores();
            cout << perfSystem;
            break;
        }
        case 9: {

            cout << auditLogger;
            break;
        }
        case 0:
            logout((User*)this);
            return; 
        default:
            cout << "Invalid option.\n";
        }
    } while (choice != 0);
}

Executive::Executive( string uname)
    : User(uname, ROLE_EXECUTIVE) {}

void Executive::displayMenu() const
{
    cout << endl;
    int choice;
    PolicyEngine policy;
    TaskSystem taskSystem;
    AuditLogger auditLogger;
    PerformanceSystem perfSystem;
    Notifications notifications;
    cout << "[+]═════════════[ Welcome Sir! ]═════════════[+]" << endl << endl;
    printAscii(this->getUsername());
    cout << endl;
    notifications.viewNotifications();
    do
    {
        cout << endl << endl;
        cout << CYAN;
        cout << "┌══════════════════════════════════════┐" << endl;
        cout << "█ 1) Create New Task                   █\n";
        cout << "█ 2) View Private Message              █\n";
        cout << "█ 3) Send Alert Message                █\n";
        cout << "█ 4) View Team Performance             █\n";
        cout << "█ 5) Audit Logs                        █\n";
        cout << "█ 6) Create notification               █\n";
        cout << "█ 0) Log out                           █\n";
        cout << "└══════════════════════════════════════┘" << endl;
        cout << RESET;
        choice = getErrFreeInt("Enter choice: ", 0, 6);

        string recipientName;
        string message;

        switch (choice)
        {
        case 1: {

            string title, creator, assignee;
            int status = TASK_CREATED;
            int priority, ttlSeconds;
            title = getErrFreeString("Enter task title: ");
            creator = this->getUsername();
            assignee = getErrFreeString("Enter assignee username: ");
            priority = getErrFreeInt("Enter priority (int): ", -1, -1);
            ttlSeconds = getErrFreeInt("Enter TTL seconds: ", -1, -1);
            string dataLine = title + "|" + to_string(status) + "|" + creator + "|" + assignee + "|" + to_string(priority) + "|" + to_string(ttlSeconds);
            if (policy.canCreateTask((User*)this)) {
                taskSystem.createTask(dataLine);
            } else {
                cout << "[!] You do not have permission to create tasks.\n";
            }
            break;
        }
        case 2: {

            Messaging::viewMessages(this->getUsername());
            break;
        }
        case 3: {

            recipientName = getErrFreeString("Recipient Username: ");
            message = getErrFreeString("Message: ");
            User* recipient = getUserObject(recipientName);
            if (recipient) {
                Messaging::sendMessage((User*)this, recipient, MSG_ALERT, message);
                delete recipient;
            } else {
                cout << "[-] Recipient not found.\n";
            }
            break;
        }
        case 4: {

            perfSystem.updateScores();
            cout << perfSystem;
            break;
        }
        case 5: {

            cout << auditLogger;
            break;
        }
        case 6:{

            string notification;
            notification = getErrFreeString("Enter notification message: ");

            int not_type;
            not_type = getErrFreeInt("Select notification type (1=warning, 2=emergency): ", 1, 2);
            notifications.createNotification(notification, not_type);
            break;
        }
        case 0:
            logout((User*)this);
            return; 
        default:
            cout << "Invalid option.\n";
        }
    } while (choice != 0);
}