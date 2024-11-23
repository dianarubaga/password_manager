#include "manager.h"
#include <sstream>
#include <iostream>
#include <limits>
#include <fstream>
#include <algorithm> // For std::any_of and std::remove_if

namespace PasswordNS {

// Implementation of encrypt
void PasswordManager::encrypt(const string &data) const {
    cout << "Encrypting data: " << data << endl;
}

// Implementation of validate
bool PasswordManager::validate(const string &password) const {
    return password.length() > 8; // Password must be longer than 8 characters
}

// Rule of 3 and 5 implementations
PasswordManager::PasswordManager() : username(""), mainPassword("") {}

PasswordManager::PasswordManager(const PasswordManager &other)
    : username(other.username), mainPassword(other.mainPassword), credentials(other.credentials) {}

PasswordManager::PasswordManager(PasswordManager &&other) noexcept
    : username(std::move(other.username)), mainPassword(std::move(other.mainPassword)), credentials(std::move(other.credentials)) {}

PasswordManager &PasswordManager::operator=(const PasswordManager &other) {
    if (this != &other) {
        username = other.username;
        mainPassword = other.mainPassword;
        credentials = other.credentials;
    }
    return *this;
}

PasswordManager &PasswordManager::operator=(PasswordManager &&other) noexcept {
    if (this != &other) {
        username = std::move(other.username);
        mainPassword = std::move(other.mainPassword);
        credentials = std::move(other.credentials);
    }
    return *this;
}

PasswordManager::~PasswordManager() noexcept {
    cout << "PasswordManager destroyed for user: " << username << endl;
}

// Set credentials for testing
void PasswordManager::setTestCredentials(const string &testUsername, const string &testPassword) {
    this->username = testUsername;
    this->mainPassword = testPassword;
}

// Save user credentials to file
void PasswordManager::saveUserCredentialsToFile() {
    std::ofstream file("user_credentials.csv", ios::app); // Append mode
    if (file.is_open()) {
        file << username << "," << mainPassword << endl;
        cout << "Saving credentials: " << username << ", " << mainPassword << endl;
        file.close();
    } else {
        cerr << "Error: Unable to open user_credentials.csv for writing." << endl;
    }
}

// Load user credentials from file
bool PasswordManager::loadUserCredentialsFromFile() {
    ifstream file("user_credentials.csv");
    if (file.is_open()) {
        string line, file_username, file_password;
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, file_username, ',');
            getline(ss, file_password, ',');

            if (file_username == username && file_password == mainPassword) {
                return true; // Credentials match
            }
        }
        file.close();
    }
    return false;
}

// Load credentials from a user-specific file
void PasswordManager::loadCredentialsFromFile() {
    ifstream file(username + "_passwords.dat"); // Load from user-specific password file
    if (file.is_open()) {
        string serviceName, serviceUsername, password;
        while (file >> serviceName >> serviceUsername >> password) {
            credentials.emplace_back(serviceName, password); // Using vector container
        }
        file.close();
    }
}

// Save credentials to a user-specific file
void PasswordManager::saveCredentialsToFile() {
    ofstream file(username + "_passwords.dat"); // User-specific password file
    if (file.is_open()) {
        for (const auto &entry : credentials) {
            file << entry.first << " " << entry.second << endl;
        }
        file.close();
    } else {
        cerr << "Error: Unable to open " << username << "_passwords.dat for writing." << endl;
    }
}

// Add a new password for a service
void PasswordManager::addNewPassword(string serviceName, string serviceUsername, string password) {
    if (validate(password)) {
        encrypt(password);
        credentials.emplace_back(serviceName, password); // Add to vector
        saveCredentialsToFile();
        cout << "Password successfully added for " << serviceName << " (Username: " << serviceUsername << ")." << endl;
    } else {
        cout << "Password is too weak!" << endl;
    }
}

// Show all stored passwords
void PasswordManager::showAllPasswords() {
    if (credentials.empty()) {
        cout << "No passwords stored." << endl;
    } else {
        for (const auto &[service, password] : credentials) {
            cout << "Service: " << service << ", Password: " << password << endl;
        }
    }
}

// Delete a password by service name
void PasswordManager::deletePassword(string serviceName) {
    auto it = remove_if(credentials.begin(), credentials.end(),
                        [&serviceName](const auto &entry) { return entry.first == serviceName; });
    if (it != credentials.end()) {
        credentials.erase(it, credentials.end());
        saveCredentialsToFile();
        cout << "Password for " << serviceName << " has been deleted." << endl;
    } else {
        cout << "Service " << serviceName << " not found." << endl;
    }
}

// Generate a simple random password
string PasswordManager::generatePassword(int length) {
    string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    string password = "";
    for (int i = 0; i < length; i++) {
        password += characters[rand() % characters.size()];
    }
    return password;
}

// Use generated password to create a new entry
void PasswordManager::useGeneratedPasswordForNewEntry(const string &generatedPassword) {
    string serviceName, serviceUsername;
    cout << "Enter the service name: ";
    getline(cin, serviceName);
    cout << "Enter the username for this service: ";
    getline(cin, serviceUsername);
    addNewPassword(serviceName, serviceUsername, generatedPassword);
}

// Use optional for returning a credential
std::optional<string> PasswordManager::getCredential(const string &serviceName) {
    for (const auto &[service, password] : credentials) {
        if (service == serviceName) {
            return password;
        }
    }
    return std::nullopt;
}

// Check if a password exists
bool PasswordManager::hasPassword(const string &serviceName) {
    return any_of(credentials.begin(), credentials.end(),
                  [&serviceName](const auto &entry) { return entry.first == serviceName; });
}

}