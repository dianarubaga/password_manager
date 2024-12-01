#include "manager.h"
#include "encryption.h"
#include <sstream>
#include <random>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <filesystem> // For checking file existence
#include "Huffman-Encoding/Huffman_C/huffman.h"

namespace PasswordNS {

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
    handleExit();
    std::cout << "PasswordManager destroyed for user: " << username << std::endl;
}

// Encrypt Functionality (Placeholder)
void PasswordManager::encrypt(const std::string &data) const {
    std::cout << "Encrypting data with key: " << "my_secure_key" << std::endl;
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
    auto encryptedPassword = EncryptionNS::encrypt(password, "my_secure_key");

    // Convert encrypted password to hex
    std::string encryptedPasswordHex;
    for (unsigned char c : encryptedPassword) {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(c);
        encryptedPasswordHex += oss.str();
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
        std::string password = EncryptionNS::decrypt(encryptedPassword, "my_secure_key");

        std::cout << std::setw(20) << service
                  << std::setw(20) << username
                  << password << std::endl;
    }
}

// Other functions (e.g., saveUserCredentialsToFile, loadCredentialsFromFile) are handled similarly.

} // namespace PasswordNS
