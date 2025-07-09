#include "../include/Messaging.h"
#include "../include/constants.h"
#include "../include/PolicyEngine.h"
#include "../include/Utils.h"
#include "../include/AuditLogger.h"
#include <fstream>
#include <ctime>
#include <iostream>
#include <iomanip>
using namespace std;

string xorEncryptHex(const string& msg, const string& key) {
    if (msg.empty() || key.empty()) {
        return "";
    }

    string hexStr;
    for (int i = 0; i < msg.size(); i++) {
        char xored = msg[i] ^ key[i % key.size()];
        unsigned char byte = static_cast<unsigned char>(xored);
        char buf[3];

        int high = (byte >> 4) & 0xF;
        int low = byte & 0xF;
        buf[0] = (high < 10) ? ('0' + high) : ('a' + high - 10);
        buf[1] = (low < 10) ? ('0' + low) : ('a' + low - 10);
        buf[2] = '\0';
        hexStr += buf;
    }
    return hexStr;
}

string xorDecryptHex(const string& hexMsg, const string& key) {
    if (hexMsg.empty() || key.empty()) {
        return "";
    }

    if (hexMsg.length() % 2 != 0) {
        cout << "[-] Error: Invalid hex message length" << endl;
        return "";
    }

    string result;
    int len = hexMsg.length();
    for (int i = 0; i < len; i += 2) {
        char high = hexMsg[i];
        char low = hexMsg[i + 1];

        if (!isxdigit(high) || !isxdigit(low)) {
            cout << "[-] Error: Invalid hex characters in message" << endl;
            return "";
        }

        int highVal = (high >= '0' && high <= '9') ? (high - '0') : (high - 'a' + 10);
        int lowVal = (low >= '0' && low <= '9') ? (low - '0') : (low - 'a' + 10);
        unsigned char byte = (highVal << 4) | lowVal;
        char orig = byte ^ key[(i/2) % key.size()];
        result += orig;
    }
    return result;
}

bool Messaging::sendMessage(User* sender, User* recipient, int type, const string& message) {
    if (!sender || !recipient) {
        cout << "[-] Error: Invalid sender or recipient" << endl;
        return false;
    }

    if (message.empty()) {
        cout << "[-] Error: Empty message content" << endl;
        return false;
    }

    if (type != MSG_INFO && type != MSG_PRIVATE && type != MSG_ALERT) {
        cout << "[-] Error: Invalid message type" << endl;
        return false;
    }

    PolicyEngine policy;
    AuditLogger auditLogger;
    bool allowed = false;
    switch (type) {
        case MSG_INFO:
            allowed = policy.canSendInfo(sender, recipient);
            break;
        case MSG_PRIVATE:
            allowed = policy.canSendPrivateMessage(sender, recipient);
            break;
        case MSG_ALERT:
            allowed = policy.canSendAlert(sender, recipient);
            break;
        default:
            allowed = false;
    }

    if (!allowed) {
        auditLogger.log(sender->getUsername(), "MESSAGE_SEND", "To: " + recipient->getUsername() + " Type: " + to_string(type), "DENIED");
        cout << "[-] Policy does not allow sending this message type." << endl;
        return false;
    }

    string inboxPath = "data/inbox/" + recipient->getUsername() + ".txt";
    ofstream out(inboxPath, ios::app);
    if (!out) {
        auditLogger.log(sender->getUsername(), "MESSAGE_SEND", "To: " + recipient->getUsername() + " Type: " + to_string(type), "FAIL");
        cout << "[-] Could not open inbox for writing." << endl;
        return false;
    }

    string encMsg = xorEncryptHex(message, recipient->getHashedPass());
    if (encMsg.empty()) {
        cout << "[-] Error: Failed to encrypt message" << endl;
        out.close();
        return false;
    }

    time_t now = time(nullptr);
    if (now == -1) {
        cout << "[-] Error: Failed to get current time" << endl;
        out.close();
        return false;
    }

    out << now << "|" << type << "|" << sender->getUsername() << "|" << encMsg << "\n";
    if (out.fail()) {
        cout << "[-] Error: Failed to write message" << endl;
        out.close();
        return false;
    }
    out.close();

    auditLogger.log(sender->getUsername(), "MESSAGE_SEND", "To: " + recipient->getUsername() + " Type: " + to_string(type), "SUCCESS");
    cout << "[+] Message sent to " << recipient->getUsername() << "." << endl;
    return true;
}

void Messaging::viewMessages(const string& username) {
    if (username.empty()) {
        cout << "[-] Error: Empty username" << endl;
        return;
    }

    string inboxPath = "data/inbox/" + username + ".txt";
    ifstream in(inboxPath);
    if (!in) {
        cout << "[-] No messages found." << endl;
        return;
    }

    User* recipient = nullptr;
    recipient = getUserObject(username);
    if (!recipient) {
        cout << "[-] Could not load user for message decryption." << endl;
        in.close();
        return;
    }

    bool hasMessages = false;
    string line;
    cout << "=== Inbox for " << username << " ===" << endl;

    while (getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        string* parts = nullptr;
        parts = split(line, '|');
        if (!parts || parts[0].empty() || parts[1].empty() || parts[2].empty() || parts[3].empty()) {
            cout << "[-] Warning: Invalid message format" << endl;
            if (parts) delete[] parts;
            continue;
        }

        time_t t = atol(parts[0].c_str());
        if (t <= 0) {
            cout << "[-] Warning: Invalid timestamp in message" << endl;
            delete[] parts;
            continue;
        }

        string timestamp = prettyprint(t);
        int type = atoi(parts[1].c_str());
        string sender = parts[2];
        string encMsg = parts[3];

        if (type != MSG_INFO && type != MSG_PRIVATE && type != MSG_ALERT) {
            cout << "[-] Warning: Unknown message type" << endl;
            delete[] parts;
            continue;
        }

        string msg = xorDecryptHex(encMsg, recipient->getHashedPass());
        if (msg.empty()) {
            cout << "[-] Warning: Failed to decrypt message" << endl;
            delete[] parts;
            continue;
        }

        string typeStr;
        switch (type) {
            case MSG_INFO: typeStr = "Info"; break;
            case MSG_PRIVATE: typeStr = "Private"; break;
            case MSG_ALERT: typeStr = "Alert"; break;
            default: typeStr = "Unknown";
        }

        cout << "[" << timestamp << "] (" << typeStr << ") From: " << sender << endl;
        cout << "  " << msg << endl;
        hasMessages = true;
        delete[] parts;
    }

    if (!hasMessages) {
        cout << "No messages to display." << endl;
    }
    cout << "---------------------" << endl;

    delete recipient;
    in.close();
}