#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>

using namespace std;

namespace PasswordNS{
// Abstract base class for managing common operations
class BaseManager
{
public:
    virtual void encrypt(const string &data) const = 0;      // Pure virtual function that encrypts data
    virtual bool validate(const string &password) const = 0; // Pure virtual function that validates password length
    virtual ~BaseManager() = default;                        // Virtual deconstructor
};

// Derived class for managing passwords
class PasswordManager : public BaseManager
{
private:
    vector<pair<string, string>> credentials; // Container that stores the credentials (service, username, password)
    string username;
    string mainPassword;

    void saveCredentialsToFile(); // Save individual account credentials to a user-specific file
    static const string encryptionKey;

public:
    // Constructors and rule of 3/5 application
    PasswordManager();                                            // Constructor for user login or registration
    PasswordManager(const PasswordManager &other);                // Copy constructor
    PasswordManager(PasswordManager &&other) noexcept;            // Move Constructor
    PasswordManager &operator=(const PasswordManager &other);     // Copy assignment
    PasswordManager &operator=(PasswordManager &&other) noexcept; // Move assignment
    ~PasswordManager();                                           // Destructor

    // Encapsulation: Getters and Setters
    void setUsername(const string &name) { username = name; }
    const std::string &getUsername() const { return username; }
    void setMainPassword(const std::string &password) { mainPassword = password; }
    const std::string &getMainPassword() const { return mainPassword; }

    void addNewPassword(string serviceName, string serviceUsername, string password); // Add a new password for a service
    void showAllPasswords();                                                          // Show all stored passwords
    void deletePassword(string serviceName);                                          // Delete a password by service name
    string generatePassword(int length);                                              // Generate a simple random password
    void useGeneratedPasswordForNewEntry(const string &generatedPassword);            // Use generated password to create a new entry
    void saveUserCredentialsToFile();                                                 // Save main password and username to a CSV file
    bool loadUserCredentialsFromFile();                                               // Load main password from CSV file and verify username and password
    void loadCredentialsFromFile();                                                   // Load the credentials from a user-specific file

    // New methods for testing
    string getCredential(const string &serviceName); // Get the password for a specific service
    bool hasPassword(const string &serviceName);     // Check if a password exists for a specific service

    // Add this method
    void setTestCredentials(const std::string &testUsername, const std::string &testPassword); // For testing

    // Inline function
    inline size_t getPasswordCount() const { return credentials.size(); }
};

}

#endif