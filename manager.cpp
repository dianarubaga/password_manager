#include "manager.h"
#include <sstream>
#include <iostream>
#include <limits>
#include <fstream>

// Encrypts the data (simple demonstration, not real encryption)
void Manager::encrypt(const std::string& data) {
    std::cout << "Encrypting data: " << data << std::endl;
}

// Validates the password (basic validation: check length)
bool Manager::validate(const std::string& password) {
    std::cout << "Validating password: " << password << std::endl;
    return password.length() > 6;  // Valid if the password is longer than 6 characters
}

// PasswordManager constructor
PasswordManager::PasswordManager() {
    // No user interaction here - constructor just initializes the class
}

// Set credentials for testing
void PasswordManager::setTestCredentials(const std::string& testUsername, const std::string& testPassword) {
    this->username = testUsername;
    this->mainPassword = testPassword;
}

// Save user credentials to file
void PasswordManager::saveUserCredentialsToFile() {
    ofstream file("user_credentials.csv", ios::app);  // Append mode
    if (file.is_open()) {
        file << username << "," << mainPassword << std::endl;
        std::cout << "Saving credentials: " << username << ", " << mainPassword << std::endl;
        file.close();
    } else {
        std::cerr << "Error: Unable to open user_credentials.csv for writing." << std::endl;
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
                return true;  // Credentials match
            }
        }
        file.close();
    }
    return false;
}

// Load credentials from a user-specific file
void PasswordManager::loadCredentialsFromFile() {
    ifstream file(username + "_passwords.dat");  // Load from user-specific password file
    if (file.is_open()) {
        string serviceName, serviceUsername, password;
        while (file >> serviceName >> serviceUsername >> password) {
            credentials[serviceName] = make_pair(serviceUsername, password);
        }
        file.close();
    }
}

// Save credentials to a user-specific file
void PasswordManager::saveCredentialsToFile() {
    ofstream file(username + "_passwords.dat");  // User-specific password file
    if (file.is_open()) {
        for (auto &entry : credentials) {
            file << entry.first << " " << entry.second.first << " " << entry.second.second << endl;
        }
        file.close();
    } else {
        std::cerr << "Error: Unable to open " << username << "_passwords.dat for writing." << std::endl;
    }
}

// Add a new password for a service
void PasswordManager::addNewPassword(string serviceName, string serviceUsername, string password) {
    if (validate(password)) {
        encrypt(password);
        credentials[serviceName] = make_pair(serviceUsername, password);
        saveCredentialsToFile();
        std::cout << "Password successfully added for " << serviceName << " (Username: " << serviceUsername << ")." << std::endl;
    } else {
        std::cout << "Password is too weak!" << std::endl;
    }
}

// Show all stored passwords
void PasswordManager::showAllPasswords() {
    if (credentials.empty()) {
        std::cout << "No passwords stored." << std::endl;
    } else {
        for (auto &entry : credentials) {
            std::cout << entry.first << " (Username: " << entry.second.first << "): " << entry.second.second << std::endl;
        }
    }
}

// Delete a password by service name
void PasswordManager::deletePassword(string serviceName) {
    if (credentials.find(serviceName) != credentials.end()) {
        credentials.erase(serviceName);
        saveCredentialsToFile();  // Update the file after deletion
        std::cout << "Password for " << serviceName << " has been deleted." << std::endl;
    } else {
        std::cout << "Service " << serviceName << " not found." << std::endl;
    }
}

// Generate a simple random password
string PasswordManager::generatePassword(int length) {
    std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    std::string password = "";
    for (int i = 0; i < length; i++) {
        password += characters[rand() % characters.size()];
    }
    return password;
}

// Use generated password to create a new entry
void PasswordManager::useGeneratedPasswordForNewEntry(const string& generatedPassword) {
    string serviceName, serviceUsername;
    std::cout << "Enter the service name: ";
    std::getline(cin, serviceName);
    std::cout << "Enter the username for this service: ";
    std::getline(cin, serviceUsername);
    addNewPassword(serviceName, serviceUsername, generatedPassword);
}

// Get the password for a specific service
string PasswordManager::getCredential(const string& serviceName) {
    if (credentials.find(serviceName) != credentials.end()) {
        return credentials[serviceName].second;
    }
    return "";
}

// Check if a password exists for a specific service
bool PasswordManager::hasPassword(const string& serviceName) {
    return credentials.find(serviceName) != credentials.end();
}
