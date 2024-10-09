#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

// Base class for managing common operations
class Manager {
public:
    void encrypt(const std::string& data);    // Encrypts data
    bool validate(const std::string& password);  // Validates password strength
};

// Derived class for managing passwords
class PasswordManager : public Manager {
private:
    map<string, pair<string, string>> credentials;  // Stores the credentials (service, username, password)
    string username;
    string mainPassword;

    void saveCredentialsToFile();       // Save individual account credentials to a user-specific file

public:
    PasswordManager();                  // Constructor for user login or registration
    void addNewPassword(string serviceName, string serviceUsername, string password); // Add a new password for a service
    void showAllPasswords();            // Show all stored passwords
    void deletePassword(string serviceName);  // Delete a password by service name
    string generatePassword(int length);  // Generate a simple random password
    void useGeneratedPasswordForNewEntry(const string& generatedPassword); // Use generated password to create a new entry
    void saveUserCredentialsToFile();   // Save main password and username to a CSV file
    bool loadUserCredentialsFromFile(); // Load main password from CSV file and verify username and password
    void loadCredentialsFromFile();     // Load the credentials from a user-specific file

    
    // New methods for testing
    string getCredential(const string& serviceName);  // Get the password for a specific service
    bool hasPassword(const string& serviceName);  // Check if a password exists for a specific service

    // Add this method
    void setTestCredentials(const std::string& testUsername, const std::string& testPassword); // For testing
};

#endif
