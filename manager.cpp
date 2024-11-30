#include "manager.h"
#include <sstream>
#include <random>
#include <algorithm>
#include <iomanip> // For formatting output
#include <stdexcept> // For exceptions
#include <filesystem> // For checking file existence
#include "Huffman-Encoding/Huffman_C/huffman.h" // Include Huffman Encoding library

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
        std::cout << "PasswordManager destroyed for user: " << username << std::endl;
    }

    // Encryption Implementation (Placeholder)
    void PasswordManager::encrypt(const std::string &data) const {
        std::cout << "Encrypting data with key: " << encryptionKey << std::endl;
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

        encrypt(password);
        credentials.emplace_back(serviceName, serviceUsername + ":" + password);
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
            std::string password = username_password.substr(delimiter_pos + 1);

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

    void PasswordManager::useGeneratedPasswordForNewEntry(const std::string &generatedPassword) {
        std::string serviceName, serviceUsername;
        std::cout << "Enter the service name: ";
        std::getline(std::cin, serviceName);
        std::cout << "Enter the username for this service: ";
        std::getline(std::cin, serviceUsername);

        addNewPassword(serviceName, serviceUsername, generatedPassword);
    }

    // Save User Credentials to File
    void PasswordManager::saveUserCredentialsToFile() {
    // Open the file in append mode
    std::ofstream file("user_credentials.csv", std::ios::app);
    if (!file.is_open()) {
        throw std::ios_base::failure("Unable to open 'user_credentials.csv' for writing.");
    }

    // Write the new username and password at the end of the file
    file << username << "," << mainPassword << "\n";
}

    // Load User Credentials from File
    bool PasswordManager::loadUserCredentialsFromFile() {
        if (std::filesystem::exists("user_credentials.huff")) {
            Decompression decompressor;
            if (!decompressor.decompress("user_credentials.huff", "user_credentials.csv")) {
                std::cerr << "Failed to decompress user credentials." << std::endl;
                return false;
            }
        }

        std::ifstream file("user_credentials.csv");
        if (!file.is_open()) {
            std::cerr << "Unable to open 'user_credentials.csv' for reading." << std::endl;
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

    void PasswordManager::compressOnExit() {
        Compression compressor;
        if (compressor.compress("user_credentials.csv", "user_credentials.huff")) {
            std::cout << "Compressed credentials successfully!" << std::endl;
        } else {
            std::cerr << "Failed to compress credentials." << std::endl;
        }
    }

    // Exit Handling - Compress database
    void PasswordManager::handleExit() {
        compressOnExit();
        std::cout << "Exiting Password Manager..." << std::endl;
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

    std::optional<std::string> PasswordManager::getCredential(const std::string &serviceName) const {
        for (const auto &entry : credentials) {
            if (entry.first == serviceName) {
                return entry.second;
            }
        }
        return std::nullopt;
    }

    bool PasswordManager::hasPassword(const std::string &serviceName) const {
        return std::any_of(credentials.begin(), credentials.end(),
                           [&serviceName](const auto &entry) { return entry.first == serviceName; });
    }

} // namespace PasswordNS