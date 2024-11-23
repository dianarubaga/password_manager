#include "manager.h"
#include <sstream>
#include <iostream>
#include <limits>
#include <fstream>
#include <algorithm> // For std::any_of and std::remove_if

namespace PasswordNS {

// Implementation of encrypt
void PasswordManager::encrypt(const std::string &data) const {
    std::cout << "Encrypting data: " << data << std::endl;
}

// Implementation of validate
bool PasswordManager::validate(const std::string &password) const {
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
    std::cout << "PasswordManager destroyed for user: " << username << std::endl;
}

// Set credentials for testing
void PasswordManager::setTestCredentials(const std::string &testUsername, const std::string &testPassword) {
    this->username = testUsername;
    this->mainPassword = testPassword;
}

// Save user credentials to file
void PasswordManager::saveUserCredentialsToFile() {
    std::ofstream file("user_credentials.csv", std::ios::app); // Append mode
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
    std::ifstream file("user_credentials.csv");
    if (file.is_open()) {
        std::string line, file_username, file_password;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::getline(ss, file_username, ',');
            std::getline(ss, file_password, ',');

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
    std::ifstream file(username + "_passwords.dat"); // Load from user-specific password file
    if (file.is_open()) {
        std::string serviceName, password;
        while (file >> serviceName >> password) {
            credentials.emplace_back(serviceName, password);
        }
        file.close();
    }
}

void PasswordManager::saveCredentialsToFile() {
    // Open the file in truncate mode to overwrite existing contents
    std::ofstream file(username + "_passwords.dat", std::ios::trunc);
    if (file.is_open()) {
        for (const auto &entry : credentials) {
            file << entry.first << " " << entry.second << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Error: Unable to open " << username << "_passwords.dat for writing." << std::endl;
    }
}


// Add a new password for a service
void PasswordManager::addNewPassword(std::string serviceName, std::string serviceUsername, std::string password) {
    if (validate(password)) {
        encrypt(password);
        credentials.emplace_back(serviceName, password);
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
        for (const auto &entry : credentials) {
            std::cout << "Service: " << entry.first << ", Password: " << entry.second << std::endl;
        }
    }
}

// Delete a password by service name
void PasswordManager::deletePassword(std::string serviceName) {
    auto it = std::remove_if(credentials.begin(), credentials.end(),
                             [&serviceName](const auto &entry) { return entry.first == serviceName; });
    if (it != credentials.end()) {
        credentials.erase(it, credentials.end());
        saveCredentialsToFile();
        std::cout << "Password for " << serviceName << " has been deleted." << std::endl;
    } else {
        std::cout << "Service " << serviceName << " not found." << std::endl;
    }
}

// Generate a simple random password
std::string PasswordManager::generatePassword(int length) {
    std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    std::string password;
    for (int i = 0; i < length; ++i) {
        password += characters[rand() % characters.size()];
    }
    return password;
}

// Use generated password to create a new entry
void PasswordManager::useGeneratedPasswordForNewEntry(const std::string &generatedPassword) {
    std::string serviceName, serviceUsername;
    std::cout << "Enter the service name: ";
    std::getline(std::cin, serviceName);
    std::cout << "Enter the username for this service: ";
    std::getline(std::cin, serviceUsername);
    addNewPassword(serviceName, serviceUsername, generatedPassword);
}

// Use optional for returning a credential
std::optional<std::string> PasswordManager::getCredential(const std::string &serviceName) const {
    for (const auto &entry : credentials) {
        if (entry.first == serviceName) {
            return entry.second;
        }
    }
    return std::nullopt;
}

// Check if a password exists
bool PasswordManager::hasPassword(const std::string &serviceName) const {
    return std::any_of(credentials.begin(), credentials.end(),
                       [&serviceName](const auto &entry) { return entry.first == serviceName; });
}

} // namespace PasswordNS