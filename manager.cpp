#include "manager.h"
#include "encryption.h"
#include <sstream>
#include <random>
#include <algorithm>
#include <iomanip> // For formatting output
#include <stdexcept> // For exceptions

namespace PasswordNS {

// Encryption Key (32 bytes for AES-256)
constexpr char encryptionKey[] = "my_32_byte_secure_key_for_aes";

// Constructor Definitions
PasswordManager::PasswordManager() : username(""), mainPassword("") {}

PasswordManager::PasswordManager(const PasswordManager &other)
    : credentials(other.credentials), username(other.username), mainPassword(other.mainPassword) {}

PasswordManager::PasswordManager(PasswordManager &&other) noexcept
    : credentials(std::move(other.credentials)), username(std::move(other.username)), mainPassword(std::move(other.mainPassword)) {}

PasswordManager &PasswordManager::operator=(const PasswordManager &other) {
    if (this != &other) {
        credentials = other.credentials;
        username = other.username;
        mainPassword = other.mainPassword;
    }
    return *this;
}

PasswordManager &PasswordManager::operator=(PasswordManager &&other) noexcept {
    if (this != &other) {
        credentials = std::move(other.credentials);
        username = std::move(other.username);
        mainPassword = std::move(other.mainPassword);
    }
    return *this;
}

PasswordManager::~PasswordManager() noexcept {
    std::cout << "PasswordManager destroyed for user: " << username << std::endl;
}

// Password Validation
bool PasswordManager::validate(const std::string &password) const {
    return password.length() > 8;
}

// Set Test Credentials
void PasswordManager::setTestCredentials(const std::string &testUsername, const std::string &testPassword) {
    username = testUsername;
    mainPassword = testPassword;
}

// Add a New Password
void PasswordManager::addNewPassword(std::string serviceName, std::string serviceUsername, std::string password) {
    if (!validate(password)) {
        throw std::invalid_argument("Password is too weak! It must be longer than 8 characters.");
    }

    // Encrypt the password
    auto encryptedPassword = encrypt(password, encryptionKey);

    // Convert encrypted password to hex
    std::string encryptedPasswordHex;
    for (unsigned char c : encryptedPassword) {
        encryptedPasswordHex += sprintf("%02x", c);
    }

    credentials.emplace_back(serviceName, serviceUsername + ":" + encryptedPasswordHex);
    saveCredentialsToFile();

    std::cout << "Password successfully added for service: " << serviceName << std::endl;
}

// Show All Stored Passwords
void PasswordManager::showAllPasswords() {
    if (credentials.empty()) {
        std::cout << "No passwords stored." << std::endl;
        return;
    }

    std::cout << std::setw(20) << std::left << "Service"
              << std::setw(20) << "Username"
              << "Password" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    for (const auto &entry : credentials) {
        std::string service = entry.first;
        std::string username_password = entry.second;

        size_t delimiter_pos = username_password.find(':');
        std::string username = username_password.substr(0, delimiter_pos);
        std::string passwordHex = username_password.substr(delimiter_pos + 1);

        // Convert hex back to binary
        std::vector<unsigned char> encryptedPassword;
        for (size_t i = 0; i < passwordHex.size(); i += 2) {
            unsigned char byte = std::stoi(passwordHex.substr(i, 2), nullptr, 16);
            encryptedPassword.push_back(byte);
        }

        // Decrypt password
        std::string password = decrypt(encryptedPassword, encryptionKey);

        std::cout << std::setw(20) << service
                  << std::setw(20) << username
                  << password << std::endl;
    }
}

// Delete a Password
void PasswordManager::deletePassword(std::string serviceName) {
    auto it = std::remove_if(credentials.begin(), credentials.end(),
                             [&serviceName](const auto &entry) { return entry.first == serviceName; });

    if (it != credentials.end()) {
        credentials.erase(it, credentials.end());
        saveCredentialsToFile();
        std::cout << "Password for service: " << serviceName << " has been deleted." << std::endl;
    } else {
        throw std::invalid_argument("Service not found.");
    }
}

// Save User Credentials to File
void PasswordManager::saveUserCredentialsToFile() {
    std::ofstream file("user_credentials.csv", std::ios::app);
    if (!file.is_open()) {
        throw std::ios_base::failure("Unable to open 'user_credentials.csv' for writing.");
    }

    file << username << "," << mainPassword << std::endl;
}

// Save Credentials to File (Encrypt Passwords)
void PasswordManager::saveCredentialsToFile() {
    std::ofstream file(username + "_passwords.dat", std::ios::trunc);
    if (!file.is_open()) {
        throw std::ios_base::failure("Unable to open '" + username + "_passwords.dat' for writing.");
    }

    for (const auto &entry : credentials) {
        file << entry.first << " " << entry.second << std::endl;
    }
}

// Load Credentials from File (Decrypt Passwords)
void PasswordManager::loadCredentialsFromFile() {
    std::ifstream file(username + "_passwords.dat");
    if (!file.is_open()) {
        throw std::ios_base::failure("Unable to open '" + username + "_passwords.dat' for reading.");
    }

    credentials.clear();
    std::string line;
    while (std::getline(file, line)) {
        size_t spacePos = line.find(' ');
        size_t colonPos = line.find(':');

        std::string serviceName = line.substr(0, spacePos);
        std::string username = line.substr(spacePos + 1, colonPos - spacePos - 1);
        std::string passwordHex = line.substr(colonPos + 1);

        // Convert hex to binary
        std::vector<unsigned char> encryptedPassword;
        for (size_t i = 0; i < passwordHex.size(); i += 2) {
            unsigned char byte = std::stoi(passwordHex.substr(i, 2), nullptr, 16);
            encryptedPassword.push_back(byte);
        }

        // Decrypt password
        std::string password = decrypt(encryptedPassword, encryptionKey);

        credentials.emplace_back(serviceName, username + ":" + password);
    }
}

} // namespace PasswordNS
