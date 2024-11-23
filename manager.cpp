#include "manager.h"
#include <sstream>
#include <iostream>
#include <limits>
#include <fstream>

using namespace std;

namespace PasswordNS
{
    // Static member definition
    const string PasswordManager::encryptionKey = "MySecretKey";

    // Constructures, Rules of 3 and 5
    PasswordManager::PasswordManager() : username(""), mainPassword("") {}

    PasswordManager::PasswordManager(const PasswordManager &other)
        : username(other.username), mainPassword(other.mainPassword), credentials(other.credentials) {}

    PasswordManager::PasswordManager(PasswordManager &&other) noexcept
        : username(std::move(other.username)), mainPassword(std::move(other.mainPassword)), credentials(std::move(other.credentials)) {}

    PasswordManager &PasswordManager::operator=(const PasswordManager &other)
    {
        if (this != &other)
        {
            username = other.username;
            mainPassword = other.mainPassword;
            credentials = other.credentials;
        }
        return *this;
    }

    PasswordManager &PasswordManager::operator=(PasswordManager &&other) noexcept
    {
        if (this != &other)
        {
            username = std::move(other.username);
            mainPassword = std::move(other.mainPassword);
            credentials = std::move(other.credentials);
        }
        return *this;
    }

    PasswordManager::~PasswordManager()
    {
        std::cout << "PasswordManager destroyed for user: " << username << std::endl;
    }

    // Inline function implementation
    inline size_t PasswordManager::getPasswordCount() const
    {
        return credentials.size();
    }

    // PasswordManager constructor
    PasswordManager::PasswordManager()
    {
        // No user interaction here - constructor just initializes the class
    }

    // Set credentials for testing
    void PasswordManager::setTestCredentials(const string &testUsername, const string &testPassword)
    {
        this->username = testUsername;
        this->mainPassword = testPassword;
    }

    // Save user credentials to file
    void PasswordManager::saveUserCredentialsToFile()
    {
        std::ofstream file("user_credentials.csv", ios::app); // Append mode
        if (file.is_open())
        {
            file << username << "," << mainPassword << endl;
            cout << "Saving credentials: " << username << ", " << mainPassword << endl;
            file.close();
        }
        else
        {
            cerr << "Error: Unable to open user_credentials.csv for writing." << endl;
        }
    }

    // Load user credentials from file
    bool PasswordManager::loadUserCredentialsFromFile()
    {
        ifstream file("user_credentials.csv");
        if (file.is_open())
        {
            string line, file_username, file_password;
            while (getline(file, line))
            {
                stringstream ss(line);
                getline(ss, file_username, ',');
                getline(ss, file_password, ',');

                if (file_username == username && file_password == mainPassword)
                {
                    return true; // Credentials match
                }
            }
            file.close();
        }
        return false;
    }

    // Load credentials from a user-specific file
    void PasswordManager::loadCredentialsFromFile()
    {
        ifstream file(username + "_passwords.dat"); // Load from user-specific password file
        if (file.is_open())
        {
            string serviceName, serviceUsername, password;
            while (file >> serviceName >> serviceUsername >> password)
            {
                credentials.emplace_back(serviceName, password); // Using vector container
            }
            file.close();
        }
    }

    // Save credentials to a user-specific file
    void PasswordManager::saveCredentialsToFile()
    {
        ofstream file(username + "_passwords.dat"); // User-specific password file
        if (file.is_open())
        {
            for (const auto &entry : credentials)
            {
                file << entry.first << " " << entry.second << endl;
            }
            file.close();
        }
        else
        {
            cerr << "Error: Unable to open " << username << "_passwords.dat for writing." << endl;
        }
    }

    // Add a new password for a service
    void PasswordManager::addNewPassword(string serviceName, string serviceUsername, string password)
    {
        if (validate(password))
        {
            encrypt(password);
            credentials.emplace_back(serviceName, password); // Add to vector
            saveCredentialsToFile();
            cout << "Password successfully added for " << serviceName << " (Username: " << serviceUsername << ")." << endl;
        }
        else
        {
            cout << "Password is too weak!" << endl;
        }
    }

    // Show all stored passwords
    void PasswordManager::showAllPasswords()
    {
        if (credentials.empty())
        {
            cout << "No passwords stored." << endl;
        }
        else
        {
            for (const auto &entry : credentials)
            {
                cout << "Service: " << entry.first << ", Password: " << entry.second << endl;
            }
        }
    }

    // Delete a password by service name
    void PasswordManager::deletePassword(string serviceName)
    {
        auto it = remove_if(credentials.begin(), credentials.end(),
                            [&serviceName](const auto &entry)
                            { return entry.first == serviceName; });
        if (it != credentials.end())
        {
            credentials.erase(it, credentials.end());
            saveCredentialsToFile();
            cout << "Password for " << serviceName << " has been deleted." << endl;
        }
        else
        {
            cout << "Service " << serviceName << " not found." << endl;
        }
    }

    // Generate a simple random password
    string PasswordManager::generatePassword(int length)
    {
        string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
        string password = "";
        for (int i = 0; i < length; i++)
        {
            password += characters[rand() % characters.size()];
        }
        return password;
    }

    // Use generated password to create a new entry
    void PasswordManager::useGeneratedPasswordForNewEntry(const string &generatedPassword)
    {
        string serviceName, serviceUsername;
        cout << "Enter the service name: ";
        getline(cin, serviceName);
        cout << "Enter the username for this service: ";
        getline(cin, serviceUsername);
        addNewPassword(serviceName, serviceUsername, generatedPassword);
    }

    std::string PasswordManager::getCredential(const string &serviceName)
    {
        // Search for the service in the vector
        for (const auto &entry : credentials)
        {
            if (entry.first == serviceName)
            {
                return entry.second; // Return the password if found
            }
        }
        return ""; // Return an empty string if the service is not found
    }

    bool PasswordManager::hasPassword(const std::string &serviceName)
    {
        // Check if the service exists in the vector
        return any_of(credentials.begin(), credentials.end(),
                           [&serviceName](const auto &entry)
                           { return entry.first == serviceName; });
    }

}