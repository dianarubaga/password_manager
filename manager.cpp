#include "manager.h"
#include "encryption.h"
#include <sstream>
#include <random>
#include <algorithm>
#include <iomanip> // For formatting output
#include <stdexcept> // For exceptions
#include <filesystem> // For checking file existence
#include "Huffman-Encoding/Huffman_C/huffman.h" // Include Huffman Encoding library

namespace PasswordNS {

// Encryption Key (Define here)
const std::string PasswordManager::encryptionKey = "my_secure_key_for_aes_encryption";

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

// Password Validation
bool PasswordManager::validate(const std::string &password) const {
    return password.length() > 8;
}

// Encrypt Functionality
void PasswordManager::encrypt(const std::string &data) const {
    std::cout << "Encrypting data with key: " << encryptionKey << std::endl;
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
    auto encryptedPassword = EncryptionNS::encrypt(password, encryptionKey);

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
        std::string password = EncryptionNS::decrypt(encryptedPassword, encryptionKey);

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

// Check if a service has a password
bool PasswordManager::hasPassword(const std::string &serviceName) const {
    return std::any_of(credentials.begin(), credentials.end(),
                       [&serviceName](const auto &entry) { return entry.first == serviceName; });
}

// Get a credential for a specific service
std::optional<std::string> PasswordManager::getCredential(const std::string &serviceName) const {
    for (const auto &entry : credentials) {
        if (entry.first == serviceName) {
            size_t delimiter_pos = entry.second.find(':');
            if (delimiter_pos != std::string::npos) {
                return entry.second.substr(delimiter_pos + 1);
            }
            return entry.second; // Return the entire value if no delimiter
        }
    }
    return std::nullopt;
}

// Get all credentials
std::vector<std::pair<std::string, std::string>> PasswordManager::getAllCredentials() const {
    return credentials;
}

// Generate a Random Password
std::string PasswordManager::generatePassword(int length) {
    if (length <= 0) {
        throw std::invalid_argument("Password length must be greater than 0.");
    }

    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    std::string password;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> dist(0, characters.size() - 1);

    for (int i = 0; i < length; ++i) {
        password += characters[dist(generator)];
    }
    return password;
}

// Use Generated Password
void PasswordManager::useGeneratedPasswordForNewEntry(const std::string &generatedPassword) {
    std::string serviceName, serviceUsername;
    std::cout << "Enter the service name: ";
    std::getline(std::cin, serviceName);
    std::cout << "Enter the username for this service: ";
    std::getline(std::cin, serviceUsername);

    addNewPassword(serviceName, serviceUsername, generatedPassword);
}

// Save Stored Passwords to File
void PasswordManager::saveCredentialsToFile() {
    std::ofstream file(username + "_passwords.dat", std::ios::trunc);
    if (!file.is_open()) {
        throw std::ios_base::failure("Unable to open '" + username + "_passwords.dat' for writing.");
    }

    for (const auto &entry : credentials) {
        file << entry.first << " " << entry.second << std::endl;
    }
}

// Load Stored Passwords from File
void PasswordManager::loadCredentialsFromFile() {
    std::ifstream file(username + "_passwords.dat");
    if (!file.is_open()) {
        throw std::ios_base::failure("Unable to open '" + username + "_passwords.dat' for reading.");
    }

    credentials.clear();
    std::string serviceName, username_password;
    while (file >> serviceName >> username_password) {
        credentials.emplace_back(serviceName, username_password);
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

// Load User Credentials from File
bool PasswordManager::loadUserCredentialsFromFile() {
    std::ifstream file("user_credentials.csv");
    if (!file.is_open()) {
        return false;
    }

    std::string line, file_username, file_password;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::getline(ss, file_username, ',');
        std::getline(ss, file_password, ',');

        if (file_username == username && file_password == mainPassword) {
            return true;
        }
    }

    return false;
}

// Handle Exit
void PasswordManager::handleExit() {
    compressOnExit();
    std::cout << "Exiting Password Manager..." << std::endl;
}

// Compress Data on Exit
void PasswordManager::compressOnExit() {
    Compression compressor;
    if (compressor.compress("user_credentials.csv", "user_credentials.huff")) {
        std::cout << "Compressed credentials successfully!" << std::endl;
    } else {
        std::cerr << "Failed to compress credentials." << std::endl;
    }
}

} // namespace PasswordNS
