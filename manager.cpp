#include "manager.h"
#include <sstream>
#include <random>
#include <algorithm>
#include <iomanip> // For formatting output
#include <stdexcept> // For exceptions

namespace PasswordNS
{

    // Constructor Definitions
    PasswordManager::PasswordManager() : username(""), mainPassword("") {}

    PasswordManager::PasswordManager(const PasswordManager &other)
        : credentials(other.credentials), username(other.username), mainPassword(other.mainPassword) {}

    PasswordManager::PasswordManager(PasswordManager &&other) noexcept
        : credentials(std::move(other.credentials)), username(std::move(other.username)), mainPassword(std::move(other.mainPassword)) {}

    PasswordManager &PasswordManager::operator=(const PasswordManager &other)
    {
        if (this != &other)
        {
            credentials = other.credentials;
            username = other.username;
            mainPassword = other.mainPassword;
        }
        return *this;
    }

    PasswordManager &PasswordManager::operator=(PasswordManager &&other) noexcept
    {
        if (this != &other)
        {
            credentials = std::move(other.credentials);
            username = std::move(other.username);
            mainPassword = std::move(other.mainPassword);
        }
        return *this;
    }

    PasswordManager::~PasswordManager() noexcept
    {
        std::cout << "PasswordManager destroyed for user: " << username << std::endl;
    }

    // Encryption Implementation (Placeholder)
    void PasswordManager::encrypt(const std::string &data) const
    {
        // Simulate encryption - replace this with a real encryption algorithm.
        std::cout << "Encrypting data with key: " << encryptionKey << std::endl;
    }

    // Password Validation
    bool PasswordManager::validate(const std::string &password) const
    {
        // Ensure password length is greater than 8 characters.
        return password.length() > 8;
    }

    // Set Test Credentials
    void PasswordManager::setTestCredentials(const std::string &testUsername, const std::string &testPassword)
    {
        // Set the username and main password for testing or initialization.
        username = testUsername;
        mainPassword = testPassword;
    }

    // Add a New Password
    void PasswordManager::addNewPassword(std::string serviceName, std::string serviceUsername, std::string password)
    {
        if (!validate(password))
        {
            throw std::invalid_argument("Password is too weak! It must be longer than 8 characters.");
        }

        encrypt(password); // Simulate encryption before storing the password.

        credentials.emplace_back(serviceName, serviceUsername + ":" + password);
        saveCredentialsToFile(); // Save updated credentials to the file.

        std::cout << "Password successfully added for service: " << serviceName << std::endl;
    }

    // Show All Stored Passwords
    void PasswordManager::showAllPasswords()
    {
        if (credentials.empty())
        {
            std::cout << "No passwords stored." << std::endl;
            return;
        }

        std::cout << std::setw(20) << std::left << "Service"
                  << std::setw(20) << "Username"
                  << "Password" << std::endl;
        std::cout << "-----------------------------------------------" << std::endl;

        for (const auto &entry : credentials)
        {
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
    void PasswordManager::deletePassword(std::string serviceName)
    {
        auto it = std::remove_if(credentials.begin(), credentials.end(),
                                 [&serviceName](const auto &entry)
                                 { return entry.first == serviceName; });

        if (it != credentials.end())
        {
            credentials.erase(it, credentials.end());
            saveCredentialsToFile();
            std::cout << "Password for service: " << serviceName << " has been deleted." << std::endl;
        }
        else
        {
            throw std::invalid_argument("Service not found.");
        }
    }

    // Generate a Random Password
    std::string PasswordManager::generatePassword(int length)
    {
        if (length <= 0)
        {
            throw std::invalid_argument("Password length must be greater than 0.");
        }

        const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
        std::string password;
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> dist(0, characters.size() - 1);

        for (int i = 0; i < length; ++i)
        {
            password += characters[dist(generator)];
        }
        return password;
    }

    // Use Generated Password
    void PasswordManager::useGeneratedPasswordForNewEntry(const std::string &generatedPassword)
    {
        std::string serviceName, serviceUsername;
        std::cout << "Enter the service name: ";
        std::getline(std::cin, serviceName);
        std::cout << "Enter the username for this service: ";
        std::getline(std::cin, serviceUsername);

        addNewPassword(serviceName, serviceUsername, generatedPassword);
    }

    // Save User Credentials to File
    void PasswordManager::saveUserCredentialsToFile()
    {
        std::ofstream file("user_credentials.csv", std::ios::app);
        if (!file.is_open())
        {
            throw std::ios_base::failure("Unable to open 'user_credentials.csv' for writing.");
        }

        file << username << "," << mainPassword << std::endl;
    }

    // Load User Credentials from File
    bool PasswordManager::loadUserCredentialsFromFile()
    {
        std::ifstream file("user_credentials.csv");
        if (!file.is_open())
        {
            throw std::ios_base::failure("Unable to open 'user_credentials.csv' for reading.");
        }

        std::string line, file_username, file_password;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::getline(ss, file_username, ',');
            std::getline(ss, file_password, ',');

            if (file_username == username && file_password == mainPassword)
            {
                return true; // Credentials match.
            }
        }
        return false; // No matching credentials found.
    }

    // Load Credentials from File
    void PasswordManager::loadCredentialsFromFile()
    {
        std::ifstream file(username + "_passwords.dat");
        if (!file.is_open())
        {
            throw std::ios_base::failure("Unable to open '" + username + "_passwords.dat' for reading.");
        }

        credentials.clear();
        std::string serviceName, username_password;
        while (file >> serviceName >> username_password)
        {
            credentials.emplace_back(serviceName, username_password);
        }
    }

    // Save Credentials to File
    void PasswordManager::saveCredentialsToFile()
    {
        std::ofstream file(username + "_passwords.dat", std::ios::trunc);
        if (!file.is_open())
        {
            throw std::ios_base::failure("Unable to open '" + username + "_passwords.dat' for writing.");
        }

        for (const auto &entry : credentials)
        {
            file << entry.first << " " << entry.second << std::endl;
        }
    }

    // Get Credential by Service Name
    std::optional<std::string> PasswordManager::getCredential(const std::string &serviceName) const
    {
        for (const auto &entry : credentials)
        {
            if (entry.first == serviceName)
            {
                return entry.second;
            }
        }
        return std::nullopt;
    }

    // Check if Password Exists for a Service
    bool PasswordManager::hasPassword(const std::string &serviceName) const
    {
        return std::any_of(credentials.begin(), credentials.end(),
                           [&serviceName](const auto &entry)
                           { return entry.first == serviceName; });
    }

} // namespace PasswordNS
